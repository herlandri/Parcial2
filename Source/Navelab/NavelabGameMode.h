// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NavelabGameMode.generated.h"


UCLASS(MinimalAPI)
class ANavelabGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ANavelabGameMode();

protected:
	virtual void BeginPlay() override;
};



