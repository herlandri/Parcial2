// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NavelabProjectile.generated.h"

class UProjectileMovementComponent;
class UStaticMeshComponent;

UCLASS(config = Game)
class ANavelabProjectile : public AActor
{
	GENERATED_BODY()

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		UProjectileMovementComponent* ProjectileMovement;

public:
	ANavelabProjectile();

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	FORCEINLINE UStaticMeshComponent* GetProjectileMesh() const { return ProjectileMesh; }
	FORCEINLINE UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	void SetDamage(float NewDamage);

	//bool bIsEnemyProjectile;

	void SetIsEnemyProjectile(bool bEnemyProjectile);

private:
	float Damage;

	bool bIsEnemyProjectile;

	//void SetIsEnemyProjectile(bool bEnemyProjectile);
};