#include "MovimientoNormalStrategy.h"
#include "Enimigo.h"

void FMovimientoNormalStrategy::Mover(
	AEnimigo * Enemy,
	float DeltaTime
)
{
	if (!Enemy || !IsValid(Enemy)) return;

	// Inicializar LastPos en el primer frame
	if (!bInitialized)
	{
		LastPos = Enemy->GetActorLocation();
		bInitialized = true;
		PickNewDirection();
		CurrentDir = TargetDir;
	}

	// --- Cambio periodico de direccion ---
	TimeAccum += DeltaTime;
	if (TimeAccum >= ChangeInterval)
	{
		PickNewDirection();
		TimeAccum = 0.f;
		ChangeInterval = FMath::RandRange(2.f, 3.5f);
	}

	// Interpolar suavemente hacia la direccion objetivo
	CurrentDir = FMath::VInterpConstantTo(
		CurrentDir, TargetDir, DeltaTime, 280.f
	);

	// Siempre validar antes de normalizar
	if (CurrentDir.IsNearlyZero()) CurrentDir = TargetDir;
	CurrentDir.Normalize();

	FVector Delta = CurrentDir * Enemy->GetMoveSpeed() * DeltaTime;

	FHitResult Hit;
	Enemy->AddActorWorldOffset(Delta, true, &Hit);

	if (Hit.IsValidBlockingHit())
	{
		FVector Normal = Hit.ImpactNormal;
		Normal.Z = 0.f;

		// Solo usar la normal si es valida en el plano XY
		if (!Normal.IsNearlyZero())
		{
			Normal.Normalize();
			FVector Reflected = CurrentDir.MirrorByVector(Normal);
			if (!Reflected.IsNearlyZero())
			{
				CurrentDir = Reflected;
				CurrentDir.Normalize();
			}
			else
			{
				// Normal casi paralela: girar 90 grados
				CurrentDir = FVector(-CurrentDir.Y, CurrentDir.X, 0.f);
			}
		}
		else
		{
			// Normal apunta al techo/suelo (Z puro): simplemente rebotar
			CurrentDir = FVector(-CurrentDir.Y, CurrentDir.X, 0.f);
		}

		TargetDir = CurrentDir;
		TimeAccum = 0.f;
		StuckTime = 0.f;
		ChangeInterval = FMath::RandRange(1.5f, 2.5f);
	}

	// --- Anti-atasco ---
	FVector Pos = Enemy->GetActorLocation();
	if ((Pos - LastPos).SizeSquared() < 0.5f)
	{
		StuckTime += DeltaTime;
		if (StuckTime > 0.5f)
		{
			PickNewDirection();
			CurrentDir = TargetDir;
			StuckTime = 0.f;
			TimeAccum = 0.f;
		}
	}
	else
	{
		StuckTime = 0.f;
	}
	LastPos = Pos;

	// Rotacion suave
	if (!CurrentDir.IsNearlyZero())
	{
		FRotator TargetRot = CurrentDir.Rotation();
		Enemy->SetActorRotation(
			FMath::RInterpTo(Enemy->GetActorRotation(), TargetRot, DeltaTime, 6.f)
		);
	}
}

void FMovimientoNormalStrategy::PickNewDirection()
{
	float Angle = FMath::RandRange(0.f, 2.f * PI);
	TargetDir = FVector(FMath::Cos(Angle), FMath::Sin(Angle), 0.f);
	TargetDir.Normalize();
}