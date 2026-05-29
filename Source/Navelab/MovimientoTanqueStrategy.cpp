#include "MovimientoTanqueStrategy.h"
#include "Enimigo.h"

void FMovimientoTanqueStrategy::Mover(
	AEnimigo* Enemy,
	float DeltaTime
)
{
	if (!Enemy || !IsValid(Enemy)) return;

	if (!bInitialized)
	{
		PickNewDirection();
		bInitialized = true;
	}

	DirTimer += DeltaTime;
	CurvePhase += DeltaTime * 0.8f;

	if (DirTimer >= DirInterval)
	{
		PickNewDirection();
		DirTimer = 0.f;
		DirInterval = FMath::RandRange(3.5f, 6.f);
		CurvePhase = 0.f;
	}

	FVector Perp(-GlobalDir.Y, GlobalDir.X, 0.f);
	FVector MoveDir = GlobalDir + Perp * FMath::Sin(CurvePhase) * 0.45f;

	if (MoveDir.IsNearlyZero()) MoveDir = GlobalDir;
	MoveDir.Normalize();

	float   Speed = Enemy->GetMoveSpeed() * 0.65f;
	FVector Delta = MoveDir * Speed * DeltaTime;

	FHitResult Hit;
	Enemy->AddActorWorldOffset(Delta, true, &Hit);

	if (Hit.IsValidBlockingHit())
	{
		FVector Normal = Hit.ImpactNormal;
		Normal.Z = 0.f;

		if (!Normal.IsNearlyZero())
		{
			Normal.Normalize();
			FVector Reflected = GlobalDir.MirrorByVector(Normal);
			if (!Reflected.IsNearlyZero())
				GlobalDir = Reflected;
			else
				GlobalDir = FVector(-GlobalDir.Y, GlobalDir.X, 0.f);
		}
		else
		{
			GlobalDir = FVector(-GlobalDir.Y, GlobalDir.X, 0.f);
		}

		GlobalDir.Normalize();
		CurvePhase = 0.f;
		DirTimer = 0.f;
		DirInterval = FMath::RandRange(3.f, 5.f);
	}

	// Rotacion pesada y lenta
	FRotator TargetRot = MoveDir.Rotation();
	Enemy->SetActorRotation(
		FMath::RInterpTo(Enemy->GetActorRotation(), TargetRot, DeltaTime, 1.8f)
	);
}

void FMovimientoTanqueStrategy::PickNewDirection()
{
	float Angle = FMath::RandRange(0.f, 2.f * PI);
	GlobalDir = FVector(FMath::Cos(Angle), FMath::Sin(Angle), 0.f);
	GlobalDir.Normalize();
}