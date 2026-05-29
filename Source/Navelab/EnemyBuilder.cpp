#include "EnemyBuilder.h"

FEnemyBuilder::FEnemyBuilder(AEnimigo* NewEnemy)
{
	Enemy = NewEnemy;
}

FEnemyBuilder& FEnemyBuilder::SetSpeed(float Speed)
{
	if (Enemy) {
		Enemy->SetMoveSpeed(Speed);
	}
	
	return *this;
}

FEnemyBuilder& FEnemyBuilder::SetHealth(float Health)
{
	if (Enemy) {
		Enemy->SetHealth(Health);
	}	
	return *this;
}

FEnemyBuilder& FEnemyBuilder::SetDamage(float Damage)
{
	if (Enemy) {
		Enemy->SetExplosionDamage(Damage);
	}	
	return *this;
}

FEnemyBuilder& FEnemyBuilder::SetType(ETipoEnemigo Tipo)
{
	if (Enemy) {
		Enemy->SetEnemyType(Tipo);
	}
	
	return *this;
}

FEnemyBuilder& FEnemyBuilder::SetEnemyMesh(UStaticMesh* Mesh)
{	
	if (Enemy) {
		Enemy->SetEnemyMesh(Mesh);
	}	
	return *this;
}
FEnemyBuilder& FEnemyBuilder::SetMeshRotation(FRotator Rotation)
{
	if (Enemy) {
		Enemy->SetMeshRotation(Rotation);
	}	
	return *this;
}
FEnemyBuilder& FEnemyBuilder::SetExplosionDamage(float Damage)
{
	if (Enemy) {
		Enemy->SetExplosionDamage(Damage);
	}
	return *this;
}

FEnemyBuilder& FEnemyBuilder::SetCanShoot(bool bShoot)
{
	if (Enemy)
	{
		Enemy->SetCanShoot(bShoot);
	}

	return *this;
}

FEnemyBuilder& FEnemyBuilder::SetFireRate(float Rate)
{
	if (Enemy)
	{
		Enemy->SetFireRate(Rate);
	}

	return *this;
}

FEnemyBuilder& FEnemyBuilder::SetProjectileDamage(float Damage)
{
	if (Enemy)
	{
		Enemy->SetProjectileDamage(Damage);
	}

	return *this;
}

FEnemyBuilder& FEnemyBuilder::SetRelativeScale3D(FVector NewScale)
{
	if (Enemy)
	{
		Enemy->SetRelativeScale3D(NewScale);
	}
	return *this;
}




AEnimigo* FEnemyBuilder::Build()
{
	return Enemy;
}

