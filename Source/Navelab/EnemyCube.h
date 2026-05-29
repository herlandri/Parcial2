#pragma once
#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "EnemyCube.generated.h"

UCLASS()
class NAVELAB_API AEnemyCube : public AEnemyBase
{
	GENERATED_BODY()

public:
	AEnemyCube();

protected:
	virtual void SetupMesh() override;
};