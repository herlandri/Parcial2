#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HealthObserver.generated.h"

UINTERFACE(MinimalAPI)
class UHealthObserver : public UInterface
{
	GENERATED_BODY()
};

class IHealthObserver
{
	GENERATED_BODY()

public:
	virtual void OnHealthChanged(float NewHealth, float MaxHealth) = 0;
};