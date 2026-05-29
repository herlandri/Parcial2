#include "MovimientoKamikaseStrategy.h"
#include "Enimigo.h"
#include "Kismet/GameplayStatics.h"

void FMovimientoKamikaseStrategy::Mover(
	AEnimigo* Enemy,
	float DeltaTime
)
{
	if (!Enemy) return;

	AActor* Player =
		UGameplayStatics::GetPlayerPawn(Enemy, 0);

	if (!Player) return;

	FVector Direction =
		(Player->GetActorLocation() - Enemy->GetActorLocation()).GetSafeNormal();

	FVector Delta = Direction * Enemy->GetMoveSpeed() * DeltaTime;

	FHitResult Hit;

	Enemy->AddActorWorldOffset(
		Delta,
		true,
		&Hit
	);

	if (Hit.IsValidBlockingHit())
	{
		return;
	}

	FRotator TargetRot =
		Direction.Rotation();

	Enemy->SetActorRotation(
		FMath::RInterpTo(
			Enemy->GetActorRotation(),
			TargetRot,
			DeltaTime,
			8.f
		)
	);
}