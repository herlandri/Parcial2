#pragma once

#include "CoreMinimal.h"

class AEnimigo;

class IMovimientoStrategy
{
public:
	virtual ~IMovimientoStrategy() {}

	virtual void Mover(
		AEnimigo* Enemy,
		float DeltaTime
	) = 0;
};