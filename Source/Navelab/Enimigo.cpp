#include "Enimigo.h"
#include "NavelabProjectile.h"
#include "EnemyFactory.h"
#include "TimerManager.h"
#include "Engine/World.h"

#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "MovimientoNormalStrategy.h"
#include "MovimientoKamikaseStrategy.h"
#include "MovimientoTanqueStrategy.h"
#include "MovimientoRapidoStrategy.h"
#include "MovimientoManhosoStrategy.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "NiagaraSystem.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"

AEnimigo::AEnimigo()
{
	PrimaryActorTick.bCanEverTick = true;

	ExplosionEffectNiagara = nullptr;
	ExplosionEffect = nullptr;
	ExplosionSound = nullptr;
	MovimientoStrategy = nullptr;

	CurrentDirection = FVector::ForwardVector;
	TargetDirection = FVector::ForwardVector;

	// CollisionRoot
	CollisionRoot = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionRoot"));
	RootComponent = CollisionRoot;
	CollisionRoot->SetSphereRadius(40.f);
	CollisionRoot->SetCollisionProfileName(TEXT("Pawn"));
	CollisionRoot->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionRoot->SetEnableGravity(false);

	// Mesh visual
	EnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyMesh"));
	EnemyMesh->SetupAttachment(CollisionRoot);
	EnemyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshBaseScale = FVector(1.f);
	EnemyMesh->SetRelativeScale3D(MeshBaseScale);
	EnemyMesh->SetRelativeLocation(FVector::ZeroVector);

	MeshPath = TEXT("/Game/StarterContent/Shapes/Shape_Cone.Shape_Cone");
	MeshBaseRotation = FRotator(0.f, 180.f, 0.f);

	MoveSpeed = 600.f;
	DirectionChangeInterval = 2.f;
	Health = 100.f;
	ExplosionRadius = 300.f;
	ExplosionDamage = 50.f;
	TimeSinceLastDirectionChange = 0.f;

	bCanShoot = false;
	FireRate = 2.f;
	ProjectileDamage = 20.f;
	TimeSinceLastShot = 0.f;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ExplosionFX(
		TEXT("/Game/StarterContent/Particles/P_Explosion.P_Explosion")
	);
	if (ExplosionFX.Succeeded()) ExplosionEffect = ExplosionFX.Object;

	static ConstructorHelpers::FObjectFinder<USoundBase> ExplosionSFX(
		TEXT("/Game/StarterContent/Audio/Explosion_Cue.Explosion_Cue")
	);
	if (ExplosionSFX.Succeeded()) ExplosionSound = ExplosionSFX.Object;
}

void AEnimigo::BeginPlay()
{
	Super::BeginPlay();

	if (EnemyMesh)
	{
		EnemyMesh->SetRelativeRotation(MeshBaseRotation);
	}


	PickNewDirection();
}

void AEnimigo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Salir inmediatamente si el actor ya murio o esta siendo destruido
	if (bIsDead || IsActorBeingDestroyed()) return;

	if (MovimientoStrategy.Get() != nullptr)
	{
		MovimientoStrategy->Mover(this, DeltaTime);
	}

	// Verificar de nuevo: Mover() podria haber matado al enemigo (colision/explosion)
	if (bIsDead || IsActorBeingDestroyed()) return;

	if (bCanShoot)
	{
		TimeSinceLastShot += DeltaTime;
		if (TimeSinceLastShot >= FireRate)
		{
			Shoot();
			TimeSinceLastShot = 0.f;
		}
	}
}

void AEnimigo::MoveRandom(float DeltaTime)
{
	TimeSinceLastDirectionChange += DeltaTime;

	if (TimeSinceLastDirectionChange >= DirectionChangeInterval)
	{
		PickNewDirection();
		TimeSinceLastDirectionChange = 0.f;
	}

	CurrentDirection = FMath::VInterpTo(
		CurrentDirection, TargetDirection, DeltaTime, 2.f
	);
	CurrentDirection.Normalize();

	FVector Delta = CurrentDirection * MoveSpeed * DeltaTime;

	FHitResult Hit;
	AddActorWorldOffset(Delta, true, &Hit);

	if (Hit.IsValidBlockingHit())
	{
		PickNewDirection();
		TimeSinceLastDirectionChange = 0.f;
	}

	if (!CurrentDirection.IsNearlyZero())
	{
		FRotator TargetRot = CurrentDirection.Rotation();
		SetActorRotation(
			FMath::RInterpTo(GetActorRotation(), TargetRot, DeltaTime, 5.f)
		);
	}
}

void AEnimigo::PickNewDirection()
{
	float Angle = FMath::RandRange(0.f, 2.f * PI);
	TargetDirection = FVector(FMath::Cos(Angle), FMath::Sin(Angle), 0.f);
	TargetDirection.Normalize();
}

void AEnimigo::TakeDamage_Enemy(float DamageAmount)
{
	if (bIsDead) return;
	Health -= DamageAmount;
	UE_LOG(LogTemp, Warning, TEXT("Vida restante: %.1f"), Health);
	if (Health <= 0.f) Die();
}

void AEnimigo::Die()
{
	if (bIsDead) return;
	bIsDead = true;

	// Desactivar tick INMEDIATAMENTE antes de cualquier otra cosa
	// Esto evita que Tick() siga ejecutandose mientras Destroy() procesa
	SetActorTickEnabled(false);

	// Liberar la strategy antes de destruir para evitar accesos invalidos
	MovimientoStrategy.Reset();

	Explode();
	Destroy();
}

void AEnimigo::Explode()
{
	UWorld* World = GetWorld();
	if (!World) return;

	FVector Location = GetActorLocation();

	if (ExplosionEffectNiagara)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(World, ExplosionEffectNiagara, Location);
	}
	else if (ExplosionEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(World, ExplosionEffect, Location);
	}

	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, Location);
	}

	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);

	UGameplayStatics::ApplyRadialDamage(
		World, ExplosionDamage, Location, ExplosionRadius,
		UDamageType::StaticClass(), IgnoredActors, this, nullptr, true
	);
}

void AEnimigo::Shoot()
{
	UWorld* World = GetWorld();
	if (!World) return;

	AActor* Player = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!Player) return;

	FVector Direction = (Player->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	FRotator Rotation = Direction.Rotation();
	FVector SpawnLocation = GetActorLocation() + Direction * 150.f;

	ANavelabProjectile* Projectile = World->SpawnActor<ANavelabProjectile>(SpawnLocation, Rotation);
	if (Projectile)
	{
		Projectile->SetDamage(ProjectileDamage);
		Projectile->SetIsEnemyProjectile(true);
	}
}

void AEnimigo::SetEnemyMesh(UStaticMesh* NewMesh)
{
	if (EnemyMesh && NewMesh) EnemyMesh->SetStaticMesh(NewMesh);
}

void AEnimigo::SetMoveSpeed(float Speed) { MoveSpeed = Speed; }
void AEnimigo::SetHealth(float NewHealth) { Health = NewHealth; }
void AEnimigo::SetExplosionDamage(float Damage) { ExplosionDamage = Damage; }
void AEnimigo::SetExplosionRadius(float Radius) { ExplosionRadius = Radius; }
void AEnimigo::SetCanShoot(bool bShoot) { bCanShoot = bShoot; }
void AEnimigo::SetFireRate(float Rate) { FireRate = Rate; }
void AEnimigo::SetProjectileDamage(float Damage) { ProjectileDamage = Damage; }
float AEnimigo::GetMoveSpeed() const { return MoveSpeed; }
void AEnimigo::SetRelativeScale3D(FVector NewScale) {
	MeshBaseScale = NewScale;
	if (EnemyMesh) EnemyMesh->SetRelativeScale3D(MeshBaseScale);
}

void AEnimigo::SetMeshRotation(FRotator NewRotation)
{
	MeshBaseRotation = NewRotation;
	if (EnemyMesh) EnemyMesh->SetRelativeRotation(MeshBaseRotation);
}

void AEnimigo::InvocarAliados()
{
	UWorld* World = GetWorld();
	if (!World) return;

	// Posiciones alrededor del jefe
	TArray<FVector> SpawnOffsets =
	{
		FVector(200.f, 200.f, 0.f),
		FVector(-200.f, 200.f, 0.f),
		FVector(200.f, -200.f, 0.f),
	};

	for (FVector Offset : SpawnOffsets)
	{
		FVector SpawnLocation = GetActorLocation() + Offset;

		FEnemyFactory Factory;

		AEnimigo* Ally = Factory.CrearEnemigoNormal(World, SpawnLocation);

		if (Ally)
		{
			UE_LOG(LogTemp, Warning, TEXT("Aliado invocado por jefe"));
		}
	}
}

void AEnimigo::SetEnemyType(ETipoEnemigo NewType)
{
	TipoEnemigo = NewType;
	MovimientoStrategy.Reset();

	switch (TipoEnemigo)
	{
	case ETipoEnemigo::Normal:
		MovimientoStrategy = MakeUnique<FMovimientoNormalStrategy>();   break;
	case ETipoEnemigo::Kamikase:
		MovimientoStrategy = MakeUnique<FMovimientoKamikaseStrategy>(); break;
	case ETipoEnemigo::Tanque:
		MovimientoStrategy = MakeUnique<FMovimientoTanqueStrategy>();   break;
	case ETipoEnemigo::Rapido:
		MovimientoStrategy = MakeUnique<FMovimientoRapidoStrategy>();   break;
	case ETipoEnemigo::Manhoso:
		MovimientoStrategy = MakeUnique<FMovimientoManhosoStrategy>();  break;
	case ETipoEnemigo::JefeNormal:
	{
		MovimientoStrategy = MakeUnique<FMovimientoNormalStrategy>();

		// activar timer de invocación desde aquí
		if (UWorld* World = EnemyMesh->GetWorld())
		{
			World->GetTimerManager().SetTimer(
				TimerInvocacion,
				this,
				&AEnimigo::InvocarAliados,
				30.f,
				true
			);
		}
		break;
	}
	default:
		MovimientoStrategy = MakeUnique<FMovimientoNormalStrategy>();   break;
	}
}

float AEnimigo::TakeDamage(
	float DamageAmount,
	FDamageEvent const& DamageEvent,
	AController* EventInstigator,
	AActor* DamageCauser)
{
	if (bIsDead) return 0.f;

	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Health -= ActualDamage;
	UE_LOG(LogTemp, Warning,
		TEXT("Enemigo recibio danio: %.1f | Vida restante: %.1f"),
		ActualDamage, Health);

	if (Health <= 0.f) Die();

	return ActualDamage;
}