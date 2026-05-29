#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBase.generated.h"

UCLASS(Abstract)
class NAVELAB_API AEnemyBase : public AActor
{ 
	GENERATED_BODY()

public:
	AEnemyBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// --- Mesh ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UStaticMeshComponent* EnemyMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
		class UMaterialInterface* EnemyMaterial;

	UPROPERTY()
		FString MeshPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
		FRotator MeshBaseRotation = FRotator(0.f, 90.f, 0.f);

	// --- Movimiento ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Movement")
		float MoveSpeed = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Movement")
		float DirectionChangeInterval = 2.f;

	// --- Salud ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Combat")
		float Health = 100.f;

	UFUNCTION(BlueprintCallable, Category = "Enemy|Combat")
		void TakeDamage_Enemy(float DamageAmount);

	// --- Explosion Cascade (activo ahora) ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Explosion")
		class UParticleSystem* ExplosionEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Explosion")
		class USoundBase* ExplosionSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Explosion")
		float ExplosionRadius = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Explosion")
		float ExplosionDamage = 50.f;

	// --- Explosion Niagara (para asignar despues) ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Explosion|Niagara")
		class UNiagaraSystem* ExplosionEffectNiagara;

protected:
	virtual void SetupMesh();

private:
	void Die();
	void Explode();
	void PickNewDirection();

	FVector MoveDirection;
	float TimeSinceLastDirectionChange;
};