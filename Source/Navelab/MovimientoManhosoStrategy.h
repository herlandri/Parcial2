#pragma once

#include "CoreMinimal.h"
#include "IMovimientoStrategy.h"

class NAVELAB_API FMovimientoManhosoStrategy
	: public IMovimientoStrategy
{
public:

	FMovimientoManhosoStrategy()
		: bRetreating(false)
		, EscapeSide(1.f)
		, OrbitPhase(0.f)
		, HysteresisTimer(0.f)
	{
	}

	virtual void Mover(AEnimigo* Enemy, float DeltaTime) override;

private:

	bool  bRetreating;
	float EscapeSide;
	float OrbitPhase;
	float HysteresisTimer;
};