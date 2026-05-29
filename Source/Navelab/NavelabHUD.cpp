
#include "NavelabHUD.h"
#include "NavelabPawn.h"
#include "Engine/Canvas.h"
#include "HealthObserver.h"
#include "Kismet/GameplayStatics.h"


void ANavelabHUD::DrawHUD()
{
	Super::DrawHUD();

	if (!Canvas) return;

	float Percent = CachedMaxHealth > 0 ? CachedHealth / CachedMaxHealth : 0.f;

	float X = 50.f;
	float Y = 50.f;

	Canvas->DrawText(GEngine->GetLargeFont(),
		FString::Printf(TEXT("HP: %.0f"), CachedHealth),
		X, Y);

	FCanvasBoxItem Background(FVector2D(X, Y + 20), FVector2D(200.f, 20.f));
	Background.SetColor(FLinearColor::Gray);
	Canvas->DrawItem(Background);

	FCanvasBoxItem Bar(FVector2D(X, Y + 20), FVector2D(200.f * Percent, 20.f));
	Bar.SetColor(FLinearColor::Green);
	Canvas->DrawItem(Bar);
}

void ANavelabHUD::OnHealthChanged(float NewHealth, float MaxHealth)
{
	UE_LOG(LogTemp, Warning, TEXT("Observer: %f / %f"), NewHealth, MaxHealth);

	CachedHealth = NewHealth;
	CachedMaxHealth = MaxHealth;
}

