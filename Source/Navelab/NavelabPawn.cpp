// Copyright Epic Games, Inc. All Rights Reserved.

#include "NavelabPawn.h"
#include "NavelabProjectile.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

const FName ANavelabPawn::MoveForwardBinding("MoveForward");
const FName ANavelabPawn::MoveRightBinding("MoveRight");
const FName ANavelabPawn::FireForwardBinding("FireForward");
const FName ANavelabPawn::FireRightBinding("FireRight");

ANavelabPawn::ANavelabPawn()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMesh(
		TEXT("StaticMesh'/Game/Geometry/Navecolorida/base_basic_shaded.base_basic_shaded'"));

	ShipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	RootComponent = ShipMeshComponent;
	ShipMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	ShipMeshComponent->SetStaticMesh(ShipMesh.Object);

	static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(
		TEXT("/Game/TwinStick/Audio/TwinStickFire.TwinStickFire"));
	FireSound = FireAudio.Object;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	// distancia de la camara al jugador
	CameraBoom->TargetArmLength = 1500.f;
	CameraBoom->SetRelativeRotation(FRotator(-80.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;

	MoveSpeed = 1000.0f;
	GunOffset = FVector(120.f, 0.f, 0.f);
	FireRate = 0.1f;
	bCanFire = true;
}

void ANavelabPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis(MoveForwardBinding);
	PlayerInputComponent->BindAxis(MoveRightBinding);
	PlayerInputComponent->BindAxis(FireForwardBinding);
	PlayerInputComponent->BindAxis(FireRightBinding);
}

void ANavelabPawn::Tick(float DeltaSeconds)
{
	const float ForwardValue = GetInputAxisValue(MoveForwardBinding);
	const float RightValue = GetInputAxisValue(MoveRightBinding);
	const FVector MoveDirection = FVector(ForwardValue, RightValue, 0.f).GetClampedToMaxSize(1.0f);
	const FVector Movement = MoveDirection * MoveSpeed * DeltaSeconds;

	if (Movement.SizeSquared() > 0.0f)
	{
		const FRotator NewRotation = Movement.Rotation();
		FHitResult Hit(1.f);
		RootComponent->MoveComponent(Movement, NewRotation, true, &Hit);

		if (Hit.IsValidBlockingHit())
		{
			const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
			const FVector Deflection = FVector::VectorPlaneProject(Movement, Normal2D) * (1.f - Hit.Time);
			RootComponent->MoveComponent(Deflection, NewRotation, true);
		}
	}

	const float FireForwardValue = GetInputAxisValue(FireForwardBinding);
	const float FireRightValue = GetInputAxisValue(FireRightBinding);
	const FVector FireDirection = FVector(FireForwardValue, FireRightValue, 0.f);
	FireShot(FireDirection);
}

void ANavelabPawn::FireShot(FVector FireDirection)
{
	if (bCanFire == true)
	{
		if (FireDirection.SizeSquared() > 0.0f)
		{
			const FRotator FireRotation = FireDirection.Rotation();
			const FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);

			UWorld* const World = GetWorld();
			if (World != nullptr)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride =
					ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				World->SpawnActor<ANavelabProjectile>(SpawnLocation, FireRotation, SpawnParams);
			}

			bCanFire = false;
			World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this,
				&ANavelabPawn::ShotTimerExpired, FireRate);

			if (FireSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
			}

			bCanFire = false;
		}
	}
}

void ANavelabPawn::NotifyHealthChanged()
{
	for (auto& Obs : Observers)
	{
		if (Obs)
		{
			Obs->OnHealthChanged(Health, MaxHealth);
		}
	}
}

void ANavelabPawn::AddObserver(TScriptInterface<IHealthObserver> Observer)
{
	Observers.Add(Observer);
}

void ANavelabPawn::RemoveObserver(TScriptInterface<IHealthObserver> Observer)
{
	Observers.Remove(Observer);
}


void ANavelabPawn::ShotTimerExpired()
{
	bCanFire = true;
}

float ANavelabPawn::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Health -= ActualDamage;
	Health = FMath::Clamp(Health, 0.f, MaxHealth);

	UE_LOG(LogTemp, Warning, TEXT("Jugador recibio danio por explosion: %.1f | Vida restante: %.1f"),
		ActualDamage, Health);

	

	if (Health <= 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT(">>> JUGADOR DESTRUIDO <<<"));
	}

	NotifyHealthChanged();

	return ActualDamage;
	
	
}