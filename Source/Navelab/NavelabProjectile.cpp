// Copyright Epic Games, Inc. All Rights Reserved.

#include "NavelabProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Enimigo.h"
#include "NavelabPawn.h"
#include "Kismet/GameplayStatics.h"

#include "EnemyBase.h"

ANavelabProjectile::ANavelabProjectile()
{
	// ─── 1. Crear componente y asignar Root PRIMERO ───────────────────────────
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh0"));
	RootComponent = ProjectileMesh;

	// ─── 2. Cargar mesh ───────────────────────────────────────────────────────
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileMeshAsset(
		TEXT("StaticMesh'/Game/TwinStick/Meshes/TwinStickProjectile.TwinStickProjectile'")
	);
	if (ProjectileMeshAsset.Succeeded())
	{
		ProjectileMesh->SetStaticMesh(ProjectileMeshAsset.Object);
	}

	// ─── 3. Escala ────────────────────────────────────────────────────────────
	//        CAMBIÁ ESTE NÚMERO para ajustar el tamaño:
	//        0.1f = muy chico | 0.5f = chico | 1.0f = normal | 2.0f = grande
	const float Escala = 1.15f;
	ProjectileMesh->SetRelativeScale3D(FVector(Escala));

	// ─── 4. Colisión y eventos ────────────────────────────────────────────────
	ProjectileMesh->BodyInstance.SetCollisionProfileName("Projectile");
	ProjectileMesh->OnComponentHit.AddDynamic(this, &ANavelabProjectile::OnHit);

	// ─── 5. Movimiento ────────────────────────────────────────────────────────
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement0"));
	ProjectileMovement->UpdatedComponent = ProjectileMesh;
	ProjectileMovement->InitialSpeed = 1000.f;
	ProjectileMovement->MaxSpeed = 1000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f;

	InitialLifeSpan = 3.0f;

	Damage = 20.f;
	bIsEnemyProjectile = false;
}
/*
void ANavelabProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this)
	{
		AEnemyBase* Enemy = Cast<AEnemyBase>(OtherActor);
		if (Enemy)
		{
			Enemy->TakeDamage_Enemy(Damage);
		}
		else if (OtherComp && OtherComp->IsSimulatingPhysics())
		{
			OtherComp->AddImpulseAtLocation(GetVelocity() * 20.0f, GetActorLocation());
		}
	}

	Destroy();
}
*/

void ANavelabProjectile::OnHit(
	UPrimitiveComponent* HitComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this)
	{
		// ==========================
		// PROYECTIL ENEMIGO
		// ==========================
		if (bIsEnemyProjectile)
		{
			ANavelabPawn* Player =
				Cast<ANavelabPawn>(OtherActor);

			if (Player)
			{
				UGameplayStatics::ApplyDamage(
					Player,
					Damage,
					nullptr,
					this,
					UDamageType::StaticClass()
				);
			}
		}
		// ==========================
		// PROYECTIL DEL JUGADOR
		// ==========================
		else
		{
			AEnimigo* Enemy =
				Cast<AEnimigo>(OtherActor);

			if (Enemy)
			{
				UGameplayStatics::ApplyDamage(
					Enemy,
					Damage,
					nullptr,
					this,
					UDamageType::StaticClass()
				);
			}
		}
	}

	Destroy();
}


void ANavelabProjectile::SetDamage(float NewDamage)
{
	Damage = NewDamage;
}

void ANavelabProjectile::SetIsEnemyProjectile(bool bEnemyProjectile)
{
	bIsEnemyProjectile = bEnemyProjectile;
}
