// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"
#include "Enemy.h"
#include "BasePlayer.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FollowTarget();
}

void AEnemy::FollowTarget()
{

	if (!SpawnerOwner || SpawnerOwner->PlayerPoints.Num() <= 0)
		return;
	
	
	//SpawnerOwner->PlayerPoints
	int32 index = SpawnerOwner->PlayerPoints.Num() - PointIndex - 1;
	if (index < 0)
		return;

	auto playerPoint = SpawnerOwner->PlayerPoints[index];
	auto loc = playerPoint->position;
	auto rot = playerPoint->rotation; 

	SetActorLocationAndRotation(loc, rot);
	//UE_LOG(LogTemp, Warning, TEXT("Set Transform "));
	PointIndex++;
	/*FPlayerPoint* playerPoint = SpawnerOwner->PlayerPoints.Peek();
	auto loc = playerPoint->position;
	auto rot = playerPoint->rotation;*/


	//SetActorLocationAndRotation()
}


