#include "MovimientoRapidoStrategy.h"
#include "Enimigo.h"

void FMovimientoRapidoStrategy::Mover(
	AEnimigo* Enemy,
	float DeltaTime
)
{
	if (!Enemy || !IsValid(Enemy)) return;

	// Inicializar en el primer frame
	if (!bInitialized)
	{
		PickNewDirection();
		bInitialized = true;
	}

	// Cambio periodico de direccion global
	DirTimer += DeltaTime;
	if (DirTimer >= DirInterval)
	{
		PickNewDirection();
		DirTimer = 0.f;
		DirInterval = FMath::RandRange(1.2f, 2.5f);
	}

	// Fase del zigzag acumulada manualmente
	ZigzagPhase += DeltaTime * 6.f;

	// Perpendicular a la direccion global en el plano XY
	FVector Perp(-GlobalDir.Y, GlobalDir.X, 0.f);

	FVector MoveDir = GlobalDir + Perp * FMath::Sin(ZigzagPhase) * 0.6f;

	if (MoveDir.IsNearlyZero()) MoveDir = GlobalDir;
	MoveDir.Normalize();

	float   Speed = Enemy->GetMoveSpeed() * 1.4f;
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
		DirTimer = 0.f;
		ZigzagPhase = 0.f;
		return;
	}

	// Rotacion suave
	FRotator TargetRot = MoveDir.Rotation();
	Enemy->SetActorRotation(
		FMath::RInterpTo(Enemy->GetActorRotation(), TargetRot, DeltaTime, 10.f)
	);
}

void FMovimientoRapidoStrategy::PickNewDirection()
{
	float Angle = FMath::RandRange(0.f, 2.f * PI);
	GlobalDir = FVector(FMath::Cos(Angle), FMath::Sin(Angle), 0.f);
	GlobalDir.Normalize();
}