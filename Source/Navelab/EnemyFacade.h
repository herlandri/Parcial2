#pragma once

#include "CoreMinimal.h"
#include "EnemyFactory.h"

class FEnemyFacade
{
public:

	static void CrearOleadaBasica(UWorld* World);

	static void CrearAtaqueKamikase(UWorld* World);

	static void CrearOleadaMixta(UWorld* World);

	static void CrearJefeFinal(UWorld* World);
};