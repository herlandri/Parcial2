#include "EnemySpawner.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Components/SceneComponent.h"

AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	// Crear un RootComponent explícito — sin esto Unreal puede ignorar la posición del actor
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;
}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	// Confirmacion visual en pantalla
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Yellow,
			TEXT("EnemySpawner: BeginPlay OK"));
	}

	SpawnEnemies();
}

void AEnemySpawner::SpawnEnemies()
{
	// --- Validaciones ---
	if (!EnemyClass)
	{
		UE_LOG(LogTemp, Error, TEXT("EnemySpawner: EnemyClass es NULL!"));
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
				TEXT("ERROR: Asigna EnemyCube en EnemyClass desde el editor!"));
		return;
	}

	UWorld* World = GetWorld();
	if (!World) return;

	SpawnedEnemies.Empty();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn; // Spawnea siempre, ignora colision

	int32 SpawnedCount = 0;

	for (int32 i = 0; i < EnemyCount; i++)
	{
		FVector SpawnLocation = GetRandomSpawnLocation();
		FRotator SpawnRotation = FRotator(0.f, FMath::RandRange(0.f, 360.f), 0.f);

		AActor* NewEnemy = World->SpawnActor<AActor>(EnemyClass, SpawnLocation, SpawnRotation, SpawnParams);

		if (NewEnemy)
		{
			SpawnedEnemies.Add(NewEnemy);
			SpawnedCount++;
		}
	}

	// Mensaje en pantalla con resultado
	FString ResultMsg = FString::Printf(TEXT("Spawneados: %d / %d enemigos"), SpawnedCount, EnemyCount);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *ResultMsg);
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Green, ResultMsg);
}

FVector AEnemySpawner::GetRandomSpawnLocation() const
{
	float Angle = FMath::RandRange(0.f, 2.f * PI);
	float Radius = FMath::RandRange(100.f, SpawnRadius);

	FVector Offset(
		Radius * FMath::Cos(Angle),
		Radius * FMath::Sin(Angle),
		50.f // elevado para evitar que queden bajo el suelo
	);

	return GetActorLocation() + Offset;
}