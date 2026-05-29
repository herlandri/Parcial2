// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFast.h"

// Sets default values
AEnemyFast::AEnemyFast()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//MoveSpeed = 1200.f;
	//Health = 50.f;
}




// Called when the game starts or when spawned
void AEnemyFast::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyFast::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

