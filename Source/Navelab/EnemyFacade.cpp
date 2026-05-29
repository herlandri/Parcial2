#include "EnemyFacade.h"

void FEnemyFacade::CrearOleadaBasica(UWorld* World)
{
	FEnemyFactory::CrearEnemigoNormal(
		World,
		FVector(0, 0, 200)
	);

	FEnemyFactory::CrearEnemigoNormal(
		World,
		FVector(300, 0, 200)
	);

	FEnemyFactory::CrearEnemigoTanque(
		World,
		FVector(-300, 0, 200)
	);
}

void FEnemyFacade::CrearAtaqueKamikase(UWorld* World)
{
	FEnemyFactory::CrearEnemigoKamikase(
		World,
		FVector(500, 200, 200)
	);

	FEnemyFactory::CrearEnemigoKamikase(
		World,
		FVector(-500, 200, 200)
	);
}

void FEnemyFacade::CrearOleadaMixta(UWorld* World)
{
	FEnemyFactory::CrearEnemigoNormal(
		World,
		FVector(0, 0, 200)
	);
	FEnemyFactory::CrearEnemigoTanque(
		World,
		FVector(300, 0, 200)
	);
	FEnemyFactory::CrearEnemigoKamikase(
		World,
		FVector(-300, 0, 200)
	);
	FEnemyFactory::CrearEnemigoManhoso(
		World,
		FVector(0, 300, 200)
	);
	FEnemyFactory::CrearEnemigoRapido(
		World,
		FVector(-200, -200, 200)
	);
}

void FEnemyFacade::CrearJefeFinal(UWorld* World)
{
	FEnemyFactory::CrearEnemigoJefeNormal(
		World,
		FVector(0, 0, 200)
	);
}