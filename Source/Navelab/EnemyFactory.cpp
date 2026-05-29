#include "EnemyFactory.h"
#include "EnemyBuilder.h"

AEnimigo* FEnemyFactory::CrearEnemigoNormal(UWorld* World, FVector Posicion)
{
	if (!World) return nullptr;

	AEnimigo* Enemy = World->SpawnActor<AEnimigo>(
		Posicion,
		FRotator::ZeroRotator
	);

	if (!Enemy) return nullptr;

	UStaticMesh* Mesh = LoadObject<UStaticMesh>(
		nullptr,
		TEXT("StaticMesh'/Game/Geometry/naveenemy/base.base'")
	);

	return FEnemyBuilder(Enemy)
		.SetEnemyMesh(Mesh)
		.SetSpeed(600.f)
		.SetHealth(100.f)
		.SetDamage(50.f)
		.SetType(ETipoEnemigo::Normal)
		.SetMeshRotation(FRotator(0.f, 180.f, 0.f))
		.SetCanShoot(true)
		.SetFireRate(2.f)
		.SetProjectileDamage(15.f)
		.Build();
}
/*
void AEnimigo::MoveWithCollision(
	FVector Direction,
	float DeltaTime
)
{
	if (Direction.IsNearlyZero())
	{
		return;
	}

	Direction.Normalize();

	FVector Movement =
		Direction *
		MoveSpeed *
		DeltaTime;

	FRotator NewRotation =
		Direction.Rotation();

	FHitResult Hit(1.f);

	RootComponent->MoveComponent(
		Movement,
		NewRotation,
		true,
		&Hit
	);

	// Deslizamiento contra muro
	if (Hit.IsValidBlockingHit())
	{
		FVector Normal2D =
			Hit.Normal.GetSafeNormal2D();

		FVector Deflection =
			FVector::VectorPlaneProject(
				Movement,
				Normal2D
			) * (1.f - Hit.Time);

		RootComponent->MoveComponent(
			Deflection,
			NewRotation,
			true
		);
	}
}
*/

AEnimigo* FEnemyFactory::CrearEnemigoKamikase(UWorld* World, FVector Posicion)
{
	if (!World) return nullptr;

	AEnimigo* Enemy = World->SpawnActor<AEnimigo>(
		Posicion,
		FRotator::ZeroRotator
	);

	if (!Enemy) return nullptr;

	UStaticMesh* Mesh = LoadObject<UStaticMesh>(
		nullptr,
		TEXT("StaticMesh'/Game/Geometry/naveenemy4/base_basic_shaded.base_basic_shaded'")
	);

	return FEnemyBuilder(Enemy)
		.SetEnemyMesh(Mesh)
		.SetSpeed(300.f)
		.SetHealth(40.f)
		.SetDamage(250.f)
		.SetType(ETipoEnemigo::Kamikase)
		.SetMeshRotation(FRotator(90.f, 0.f, 0.f))
		.SetExplosionDamage(500.f)
		.SetCanShoot(false)
		.Build();
}

AEnimigo* FEnemyFactory::CrearEnemigoTanque(UWorld* World, FVector Posicion)
{
	if (!World) return nullptr;

	AEnimigo* Enemy = World->SpawnActor<AEnimigo>(
		Posicion,
		FRotator::ZeroRotator
	);

	if (!Enemy) return nullptr;

	UStaticMesh* Mesh = LoadObject<UStaticMesh>(
		nullptr,
		TEXT("StaticMesh'/Game/Geometry/Naveespacial/base_basic_pbr.base_basic_pbr'")
	);

	return FEnemyBuilder(Enemy)
		.SetEnemyMesh(Mesh)
		.SetSpeed(300.f)
		.SetHealth(300.f)
		.SetDamage(100.f)
		.SetType(ETipoEnemigo::Tanque)
		.SetMeshRotation(FRotator(0.f, 0.f, 0.f))
		.SetCanShoot(true)
		.SetFireRate(0.5f)
		.SetProjectileDamage(40.f)
		.Build();
}
AEnimigo* FEnemyFactory::CrearEnemigoManhoso(UWorld* World, FVector Posicion)
{
	if (!World) return nullptr;
	AEnimigo* Enemy = World->SpawnActor<AEnimigo>(
		Posicion,
		FRotator::ZeroRotator
	);
	if (!Enemy) return nullptr;
	UStaticMesh* Mesh = LoadObject<UStaticMesh>(
		nullptr,
		TEXT("StaticMesh'/Game/Geometry/naveenemy2/base.base'")
	);
	return FEnemyBuilder(Enemy)
		.SetEnemyMesh(Mesh)
		.SetSpeed(400.f)
		.SetHealth(80.f)
		.SetDamage(30.f)
		.SetType(ETipoEnemigo::Manhoso)
		.SetMeshRotation(FRotator(0.f, 180.f, 0.f))
		.SetCanShoot(true)
		.SetFireRate(1.5f)
		.SetProjectileDamage(20.f)
		.Build();
}
AEnimigo* FEnemyFactory::CrearEnemigoRapido(UWorld* World, FVector Posicion)
{
	if (!World) return nullptr;
	AEnimigo* Enemy = World->SpawnActor<AEnimigo>(
		Posicion,
		FRotator::ZeroRotator
	);
	if (!Enemy) return nullptr;
	UStaticMesh* Mesh = LoadObject<UStaticMesh>(
		nullptr,
		TEXT("StaticMesh'/Game/Geometry/naveenemy3/base.base'")
	);
	return FEnemyBuilder(Enemy)
		.SetEnemyMesh(Mesh)
		.SetSpeed(400.f)
		.SetHealth(60.f)
		.SetDamage(20.f)
		.SetType(ETipoEnemigo::Rapido)
		.SetMeshRotation(FRotator(0.f, 180.f, 0.f))
		.SetCanShoot(true)
		.SetFireRate(1.f)
		.SetProjectileDamage(10.f)
		.Build();
}



AEnimigo* FEnemyFactory::CrearEnemigoJefeNormal(UWorld* World, FVector Posicion)
{
	

	if (!World) return nullptr;
	AEnimigo* Enemy = World->SpawnActor<AEnimigo>(
		Posicion,
		FRotator::ZeroRotator
	);
	if (!Enemy) return nullptr;
	UStaticMesh* Mesh = LoadObject<UStaticMesh>(
		nullptr,
		TEXT("StaticMesh'/Game/Geometry/naveenemy/base.base'")
	);
	return FEnemyBuilder(Enemy)
		.SetEnemyMesh(Mesh)
		.SetSpeed(200.f)
		.SetHealth(500.f)
		.SetDamage(100.f)
		.SetType(ETipoEnemigo::JefeNormal)
		.SetMeshRotation(FRotator(0.f, 180.f, 0.f))
		.SetCanShoot(true)
		.SetFireRate(0.5f)
		.SetProjectileDamage(30.f)
		.SetRelativeScale3D(FVector(3.f))
		.Build();
}