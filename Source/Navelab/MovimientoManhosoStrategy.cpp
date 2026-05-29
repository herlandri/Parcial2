#include "MovimientoManhosoStrategy.h"
#include "Enimigo.h"
#include "Kismet/GameplayStatics.h"

static constexpr float DIST_RETREAT_ENTER = 650.f;
static constexpr float DIST_RETREAT_EXIT = 1100.f;

void FMovimientoManhosoStrategy::Mover(
	AEnimigo* Enemy,
	float DeltaTime
)
{
	if (!Enemy || !IsValid(Enemy)) return;

	UWorld* World = Enemy->GetWorld();
	if (!World) return;

	AActor* Player = UGameplayStatics::GetPlayerPawn(Enemy, 0);
	if (!Player || !IsValid(Player)) return;

	FVector EnemyLoc = Enemy->GetActorLocation();
	FVector PlayerLoc = Player->GetActorLocation();
	FVector ToPlayer = PlayerLoc - EnemyLoc;
	float   Distance = ToPlayer.Size();

	if (Distance < 1.f) return;
	FVector ToPlayerN = ToPlayer / Distance;

	// --- Maquina de estados con histeresis ---
	HysteresisTimer -= DeltaTime;
	if (HysteresisTimer < 0.f) HysteresisTimer = 0.f;

	if (HysteresisTimer <= 0.f)
	{
		if (!bRetreating && Distance <= DIST_RETREAT_ENTER)
		{
			bRetreating = true;
			EscapeSide = (FMath::RandBool()) ? 1.f : -1.f;
			OrbitPhase = 0.f;
			HysteresisTimer = 0.7f;
		}
		else if (bRetreating && Distance >= DIST_RETREAT_EXIT)
		{
			bRetreating = false;
			HysteresisTimer = 0.7f;
		}
	}

	FVector MoveDir;

	if (!bRetreating)
	{
		// Perseguir directamente
		MoveDir = ToPlayerN;
	}
	else
	{
		// Orbitar: alejarse + lateral
		OrbitPhase += DeltaTime * 1.2f;

		FVector Away(-ToPlayerN.X, -ToPlayerN.Y, 0.f);
		FVector Orbit(-ToPlayerN.Y * EscapeSide,
			ToPlayerN.X * EscapeSide, 0.f);

		// Cuanto mas lejos estamos, mas orbitamos y menos huimos
		float t = FMath::Clamp(
			(Distance - DIST_RETREAT_ENTER) /
			(DIST_RETREAT_EXIT - DIST_RETREAT_ENTER),
			0.f, 1.f
		);
		float AwayW = FMath::Lerp(0.85f, 0.1f, t);
		float OrbitW = 1.f - AwayW;

		MoveDir = Away * AwayW + Orbit * OrbitW;

		if (MoveDir.IsNearlyZero()) MoveDir = Away;
		MoveDir.Normalize();
	}

	FVector Delta = MoveDir * Enemy->GetMoveSpeed() * DeltaTime;

	FHitResult Hit;
	Enemy->AddActorWorldOffset(Delta, true, &Hit);

	if (Hit.IsValidBlockingHit())
	{
		FVector Normal = Hit.ImpactNormal;
		Normal.Z = 0.f;

		if (!Normal.IsNearlyZero())
		{
			Normal.Normalize();
			FVector Reflected = MoveDir.MirrorByVector(Normal);
			if (!Reflected.IsNearlyZero())
				MoveDir = Reflected;
			else
				MoveDir = FVector(-MoveDir.Y, MoveDir.X, 0.f);
		}
		else
		{
			MoveDir = FVector(-MoveDir.Y, MoveDir.X, 0.f);
		}

		EscapeSide *= -1.f;
		HysteresisTimer = 0.4f;
	}

	if (!MoveDir.IsNearlyZero())
	{
		FRotator TargetRot = MoveDir.Rotation();
		Enemy->SetActorRotation(
			FMath::RInterpTo(Enemy->GetActorRotation(), TargetRot, DeltaTime, 5.f)
		);
	}
}