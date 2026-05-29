#include "EnemyFacade.h"
#include "NavelabGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "NavelabHUD.h"
#include "NavelabPawn.h"


ANavelabGameMode::ANavelabGameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = ANavelabPawn::StaticClass();

	HUDClass = ANavelabHUD::StaticClass();

}
void ANavelabGameMode::BeginPlay()
{
	Super::BeginPlay();
	ANavelabPawn* Pawn = Cast<ANavelabPawn>(
		UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (!Pawn) return;

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC) return;

	ANavelabHUD* HUD = Cast<ANavelabHUD>(PC->GetHUD());

	if (HUD)
	{
		Pawn->AddObserver(HUD);

		HUD->OnHealthChanged(Pawn->Health, Pawn->MaxHealth);
	}

	

	FVector PosicionKamikase(500.f, 300.f, 100.f);
/*
	FEnemyFacade::CrearAtaqueKamikase(
		GetWorld()
	);
	
	FEnemyFacade::CrearOleadaMixta(
		GetWorld()
	);
	*/

	FEnemyFacade::CrearJefeFinal(
		GetWorld()
	);
}

