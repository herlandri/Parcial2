#include "EnemyCone.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

AEnemyCone::AEnemyCone()
{
	MeshPath = TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cone.Shape_Cone'");
}

void AEnemyCone::SetupMesh()
{
}