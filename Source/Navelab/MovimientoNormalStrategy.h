#pragma once

#include "CoreMinimal.h"
#include "IMovimientoStrategy.h"

class NAVELAB_API FMovimientoNormalStrategy
	: public IMovimientoStrategy
{
public:

	FMovimientoNormalStrategy()
		: bInitialized(false)
		, CurrentDir(FVector::ForwardVector)
		, TargetDir(FVector::ForwardVector)
		, TimeAccum(0.f)
		, ChangeInterval(2.f)
		, StuckTime(0.f)
		, LastPos(FVector::ZeroVector)
	{
	}

	virtual void Mover(AEnimigo* Enemy, float DeltaTime) override;

private:

	bool    bInitialized;   // para inicializar LastPos en el primer frame
	FVector CurrentDir;
	FVector TargetDir;
	float   TimeAccum;
	float   ChangeInterval;
	float   StuckTime;
	FVector LastPos;

	void PickNewDirection();
};