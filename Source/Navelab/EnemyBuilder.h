#pragma once

#include "CoreMinimal.h"
#include "Enimigo.h"

class FEnemyBuilder
{
private:
	AEnimigo* Enemy;

public:
	FEnemyBuilder(AEnimigo* NewEnemy);

	FEnemyBuilder& SetSpeed(float Speed);
	FEnemyBuilder& SetHealth(float Health);
	FEnemyBuilder& SetDamage(float Damage);
	FEnemyBuilder& SetEnemyMesh(UStaticMesh* Mesh);
	FEnemyBuilder& SetMeshRotation(FRotator Rotation);
	FEnemyBuilder& SetExplosionDamage(float Damage);
	FEnemyBuilder& SetCanShoot(bool bShoot);
	FEnemyBuilder& SetFireRate(float Rate);
	FEnemyBuilder& SetProjectileDamage(float Damage);
	FEnemyBuilder& SetType(ETipoEnemigo Tipo);
	FEnemyBuilder& SetRelativeScale3D(FVector NewScale);
	

	AEnimigo* Build();
};