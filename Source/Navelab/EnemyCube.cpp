#include "EnemyCube.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

AEnemyCube::AEnemyCube()
{
	MeshPath = TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'");
}

void AEnemyCube::SetupMesh()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(
		TEXT("/Game/Geometry/naveenemy/base")
	);
	if (MeshAsset.Succeeded())
	{
		EnemyMesh->SetStaticMesh(MeshAsset.Object);
	}
}