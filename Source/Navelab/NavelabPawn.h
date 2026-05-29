// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "HealthObserver.h"
#include "NavelabPawn.generated.h"

UCLASS(Blueprintable)
class ANavelabPawn : public APawn
{
	GENERATED_BODY()

		UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* ShipMeshComponent;

	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* CameraComponent;

	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

public:
	ANavelabPawn();

	// ─── Gameplay ────────────────────────────────────────────────
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		FVector GunOffset;

	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float FireRate;

	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float MoveSpeed;

	// ─── Salud del jugador ───────────────────────────────────────
	UPROPERTY(Category = "Player|Combat", EditAnywhere, BlueprintReadWrite)
		float Health = 100.f;

	UPROPERTY(Category = "Player|Combat", EditAnywhere, BlueprintReadWrite)
		float MaxHealth = 100.f;

		UPROPERTY()
		TArray<TScriptInterface<IHealthObserver>> Observers;

		void AddObserver(TScriptInterface<IHealthObserver> Observer);
		void RemoveObserver(TScriptInterface<IHealthObserver> Observer);

		void NotifyHealthChanged();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;

	// ─── Audio ───────────────────────────────────────────────────
	UPROPERTY(Category = Audio, EditAnywhere, BlueprintReadWrite)
		class USoundBase* FireSound;

	// ─── Interface ───────────────────────────────────────────────
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	void FireShot(FVector FireDirection);
	void ShotTimerExpired();

	static const FName MoveForwardBinding;
	static const FName MoveRightBinding;
	static const FName FireForwardBinding;
	static const FName FireRightBinding;

private:
	uint32 bCanFire : 1;
	FTimerHandle TimerHandle_ShotTimerExpired;

public:
	FORCEINLINE class UStaticMeshComponent* GetShipMeshComponent() const { return ShipMeshComponent; }
	FORCEINLINE class UCameraComponent* GetCameraComponent() const { return CameraComponent; }
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
};