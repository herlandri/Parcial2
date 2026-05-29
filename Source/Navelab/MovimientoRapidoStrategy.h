#pragma once

#include "CoreMinimal.h"
#include "IMovimientoStrategy.h"

class NAVELAB_API FMovimientoRapidoStrategy
	: public IMovimientoStrategy
{
public:

	FMovimientoRapidoStrategy()
		: bInitialized(false)
		, GlobalDir(FVector::ForwardVector)
		, DirTimer(0.f)
		, DirInterval(1.8f)
		, ZigzagPhase(0.f)
	{
	}

	virtual void Mover(AEnimigo* Enemy, float DeltaTime) override;

private:

	bool    bInitialized;
	FVector GlobalDir;
	float   DirTimer;
	float   DirInterval;
	float   ZigzagPhase;

	void PickNewDirection();
};