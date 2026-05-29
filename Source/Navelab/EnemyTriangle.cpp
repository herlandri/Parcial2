#include "EnemyTriangle.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

AEnemyTriangle::AEnemyTriangle()
{
	MeshPath = TEXT("/Game/Geometry/naveenemy3/base.base");
}

void AEnemyTriangle::SetupMesh()
{
	UStaticMesh* LoadedMesh = LoadObject<UStaticMesh>(
		nullptr, TEXT("/Game/Geometry/naveenemy3/base")
		);
	if (LoadedMesh)
	{
		EnemyMesh->SetStaticMesh(LoadedMesh);
	}
}