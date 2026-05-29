#pragma once

#include "CoreMinimal.h"
#include "IMovimientoStrategy.h"

class NAVELAB_API FMovimientoKamikaseStrategy
	: public IMovimientoStrategy
{
public:

	virtual void Mover(
		AEnimigo* Enemy,
		float DeltaTime
	) override;
};