#pragma once

#include "CoreMinimal.h"
#include "Enimigo.h"

class FEnemyFactory
{
public:

	static AEnimigo* CrearEnemigoNormal(UWorld* World, FVector Posicion);

	static AEnimigo* CrearEnemigoKamikase(UWorld* World, FVector Posicion);

	static AEnimigo* CrearEnemigoTanque(UWorld* World, FVector Posicion);

	static AEnimigo* CrearEnemigoManhoso(UWorld* World, FVector Posicion);

	static AEnimigo* CrearEnemigoRapido(UWorld* World, FVector Posicion);

	static AEnimigo* CrearEnemigoJefeNormal(UWorld* World, FVector Posicion);
};
