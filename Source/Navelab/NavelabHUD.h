#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HealthObserver.h"
#include "NavelabHUD.generated.h"

UCLASS()
class ANavelabHUD : public AHUD, public IHealthObserver
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

	virtual void OnHealthChanged(float NewHealth, float MaxHealth) override;

protected:
	UPROPERTY()
	class ANavelabPawn* CachedPawn;

private:
	float CachedHealth;
	float CachedMaxHealth;
};