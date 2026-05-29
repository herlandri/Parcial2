#pragma once

#include "CoreMinimal.h"
#include "IMovimientoStrategy.h"

class NAVELAB_API FMovimientoTanqueStrategy
	: public IMovimientoStrategy
{
public:

	FMovimientoTanqueStrategy()
		: bInitialized(false)
		, GlobalDir(FVector::ForwardVector)
		, CurvePhase(0.f)
		, DirTimer(0.f)
		, DirInterval(4.f)
	{
	}

	virtual void Mover(AEnimigo* Enemy, float DeltaTime) override;

private:

	bool    bInitialized;
	FVector GlobalDir;
	float   CurvePhase;
	float   DirTimer;
	float   DirInterval;

	void PickNewDirection();
};