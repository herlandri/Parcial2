#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "IMovimientoStrategy.h"
#include "Components/SphereComponent.h"
#include "Enimigo.generated.h"

class ANavelabProjectile;
class UNiagaraSystem;

UENUM()
enum class ETipoEnemigo : uint8
{
	Normal,
	Rapido,
	Tanque,
	Kamikase,
	Manhoso,
	JefeNormal
};

UCLASS()
class NAVELAB_API AEnimigo : public AActor
{
	GENERATED_BODY()

public:
	AEnimigo();

protected:
	virtual void BeginPlay() override;
	FTimerHandle TimerInvocacion;

public:
	virtual void Tick(float DeltaTime) override;
	void TakeDamage_Enemy(float DamageAmount);

	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		class AActor* DamageCauser
	) override;

	// SETTERS
	void SetMoveSpeed(float Speed);
	void SetHealth(float NewHealth);
	void SetExplosionDamage(float Damage);
	void SetEnemyType(ETipoEnemigo NewType);
	void SetExplosionRadius(float Radius);
	void SetEnemyMesh(UStaticMesh* NewMesh);
	void SetMeshRotation(FRotator NewRotation);
	void SetRelativeScale3D(FVector NewScale);

	// Proyectiles
	void SetCanShoot(bool bShoot);
	void SetFireRate(float Rate);
	void SetProjectileDamage(float Damage);

	// GETTERS
	float GetMoveSpeed() const;

	// Movimiento publico (usado por strategies)
	void MoveRandom(float DeltaTime);
	// Invocar alidados
	void InvocarAliados();


private:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* CollisionRoot;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* EnemyMesh;

	UPROPERTY()
	UParticleSystem* ExplosionEffect;

	UPROPERTY()
	USoundBase* ExplosionSound;

	UPROPERTY()
	UNiagaraSystem* ExplosionEffectNiagara;

	// Configuracion
	FString MeshPath;
	FRotator MeshBaseRotation;

	float MoveSpeed;
	float DirectionChangeInterval;
	float Health;
	float ExplosionRadius;
	float ExplosionDamage;

	ETipoEnemigo TipoEnemigo;

	// Movimiento
	FVector CurrentDirection;
	FVector TargetDirection;
	FVector MeshBaseScale;
	float TimeSinceLastDirectionChange;

	// Proyectiles
	bool bCanShoot;
	float FireRate;
	float ProjectileDamage;
	float TimeSinceLastShot;

	bool bIsDead = false;

	TUniquePtr<IMovimientoStrategy> MovimientoStrategy;

	void PickNewDirection();
	void Die();
	void Explode();
	void Shoot();
};