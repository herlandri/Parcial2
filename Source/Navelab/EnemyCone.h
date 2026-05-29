#pragma once
#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "EnemyCone.generated.h"

UCLASS()
class NAVELAB_API AEnemyCone : public AEnemyBase
{
	GENERATED_BODY()

public:
	AEnemyCone();

protected:
	virtual void SetupMesh() override;
};