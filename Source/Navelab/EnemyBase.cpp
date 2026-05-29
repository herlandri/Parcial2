#include "EnemyBase.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"
#include "DrawDebugHelpers.h"

AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	EnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyMesh"));
	RootComponent = EnemyMesh;

	EnemyMesh->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));
	EnemyMesh->SetCollisionProfileName(TEXT("BlockAll"));
	EnemyMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	MoveDirection = FVector::ZeroVector;
	TimeSinceLastDirectionChange = 0.f;

	// Carga automatica de P_Explosion (StarterContent)
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ExplosionFX(
		TEXT("/Game/StarterContent/Particles/P_Explosion.P_Explosion")
	);
	if (ExplosionFX.Succeeded())
	{
		ExplosionEffect = ExplosionFX.Object;
	}

	// Carga automatica del sonido de explosion (StarterContent)
	static ConstructorHelpers::FObjectFinder<USoundBase> ExplosionSFX(
		TEXT("/Game/StarterContent/Audio/Explosion_Cue.Explosion_Cue")
	);
	if (ExplosionSFX.Succeeded())
	{
		ExplosionSound = ExplosionSFX.Object;
	}
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	if (!MeshPath.IsEmpty())
	{
		UStaticMesh* LoadedMesh = LoadObject<UStaticMesh>(nullptr, *MeshPath);
		if (LoadedMesh)
		{
			EnemyMesh->SetStaticMesh(LoadedMesh);
		}
	}

	if (EnemyMaterial)
	{
		EnemyMesh->SetMaterial(0, EnemyMaterial);
	}

	PickNewDirection();
}

void AEnemyBase::SetupMesh()
{
	// Las clases hijas sobreescriben esto
}

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeSinceLastDirectionChange += DeltaTime;
	if (TimeSinceLastDirectionChange >= DirectionChangeInterval)
	{
		PickNewDirection();
		TimeSinceLastDirectionChange = 0.f;
	}

	FVector Delta = MoveDirection * MoveSpeed * DeltaTime;
	FHitResult Hit;
	AddActorWorldOffset(Delta, true, &Hit);

	if (Hit.IsValidBlockingHit())
	{
		PickNewDirection();
		TimeSinceLastDirectionChange = 0.f;
	}

	if (!MoveDirection.IsNearlyZero())
	{
		FRotator TargetRot = MoveDirection.Rotation() + FRotator(0.f, 180.f, 0.f);
		FRotator CurrentRot = GetActorRotation();
		FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaTime, 5.f);
		SetActorRotation(NewRot);
	}
}

void AEnemyBase::PickNewDirection()
{
	float Angle = FMath::RandRange(0.f, 2.f * PI);
	MoveDirection = FVector(FMath::Cos(Angle), FMath::Sin(Angle), 0.f);
	MoveDirection.Normalize();
}

void AEnemyBase::TakeDamage_Enemy(float DamageAmount)
{
	Health -= DamageAmount;
	UE_LOG(LogTemp, Warning, TEXT("%s recibio danio. Vida restante: %.1f"), *GetName(), Health);

	if (Health <= 0.f)
	{
		Die();
	}
}

void AEnemyBase::Die()
{
	UE_LOG(LogTemp, Warning, TEXT("%s destruido en: %s"), *GetName(), *GetActorLocation().ToString());
	Explode();
	Destroy();
}

void AEnemyBase::Explode()
{
	FVector Location = GetActorLocation();
	UWorld* World = GetWorld();
	if (!World) return;

	// 1. Niagara tiene prioridad si esta asignado
	if (ExplosionEffectNiagara)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			World,
			ExplosionEffectNiagara,
			Location,
			FRotator::ZeroRotator,
			FVector(1.f),
			true,
			true
		);
	}
	// Si no hay Niagara, usa Cascade (P_Explosion del StarterContent)
	else if (ExplosionEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			World,
			ExplosionEffect,
			Location,
			FRotator::ZeroRotator,
			FVector(1.f),
			true
		);
	}

	// 2. Sonido
	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, Location);
	}

	// 3. Danio radial (afecta enemigos cercanos y al jugador)
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);

	UGameplayStatics::ApplyRadialDamage(
		World,
		ExplosionDamage,
		Location,
		ExplosionRadius,
		UDamageType::StaticClass(),
		IgnoredActors,
		this,
		nullptr,
		true
	);

	// Debug: descomentar para ver el radio visualmente en pantalla
	// DrawDebugSphere(World, Location, ExplosionRadius, 16, FColor::Orange, false, 2.f);

	UE_LOG(LogTemp, Warning, TEXT("Explosion en %s | Radio: %.0f | Danio: %.0f"),
		*Location.ToString(), ExplosionRadius, ExplosionDamage);
}