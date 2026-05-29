#include "EnemyCircle.h"
#include "Components/StaticMeshComponent.h"

AEnemyCircle::AEnemyCircle()
{
	MeshPath = TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'");
}

void AEnemyCircle::SetupMesh()
{
	UStaticMesh* LoadedMesh = LoadObject<UStaticMesh>(
		nullptr, TEXT("/Game/Geometry/naveenemy4/base")
		);
	if (LoadedMesh)
	{
		EnemyMesh->SetStaticMesh(LoadedMesh);
	}
}

void AEnemyCircle::Tick(float DeltaTime)
{
	// Heredar todo el movimiento del padre
	Super::Tick(DeltaTime);

	// Mantener el mesh echado independientemente de la rotacion del actor
	EnemyMesh->SetRelativeRotation(FRotator(150.f, 0.f, 0.f));
}