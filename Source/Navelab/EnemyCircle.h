#pragma once
#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "EnemyCircle.generated.h"

UCLASS()
class NAVELAB_API AEnemyCircle : public AEnemyBase
{
	GENERATED_BODY()

public:
	AEnemyCircle();

protected:
	virtual void SetupMesh() override;
	virtual void Tick(float DeltaTime) override;
};