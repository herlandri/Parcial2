#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

UCLASS()
class NAVELAB_API AEnemySpawner : public AActor
{
	GENERATED_BODY()

public:
	AEnemySpawner();

protected:
	virtual void BeginPlay() override;

public:
	// Componente raiz visible en el editor
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class USceneComponent* SceneRoot;

	// Clase del enemigo a spawnear (asignar AEnemyCube desde el editor)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
		TSubclassOf<AActor> EnemyClass;

	// Cantidad de enemigos a spawnear
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
		int32 EnemyCount = 10;

	// Radio del area de spawn alrededor del spawner
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
		float SpawnRadius = 500.f;

	// Separacion minima entre enemigos
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
		float MinSpawnSeparation = 150.f;

	// Funcion principal de spawn
	UFUNCTION(BlueprintCallable, Category = "Spawner")
		void SpawnEnemies();

private:
	// Genera una posicion aleatoria valida dentro del radio
	FVector GetRandomSpawnLocation() const;

	// Array de referencias a los enemigos spawneados
	TArray<AActor*> SpawnedEnemies;
};