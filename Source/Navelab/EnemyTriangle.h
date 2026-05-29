#pragma once
#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "EnemyTriangle.generated.h"

UCLASS()
class NAVELAB_API AEnemyTriangle : public AEnemyBase
{
	GENERATED_BODY()

public:
	AEnemyTriangle();

protected:
	virtual void SetupMesh() override;
};