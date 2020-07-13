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
	
	FindFirstPlayerPoint();
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FollowTarget();
}

void AEnemy::FindFirstPlayerPoint()
{
	if (bFoundFirstPlayerPoint || CurrentPoint)
		return;

	if (!SpawnerOwner || SpawnerOwner->PlayerPoints.Num() <= 0)
		return;

	CurrentPoint = SpawnerOwner->PlayerPoints.GetHead();
	bFoundFirstPlayerPoint = true;
}

void AEnemy::FollowTarget()
{
	FindFirstPlayerPoint();

	if (!SpawnerOwner || SpawnerOwner->PlayerPoints.Num() <= 0)
		return;
	
	if (!CurrentPoint)
		return;


	//auto TargetList = SpawnerOwner->PlayerPoints.GetTail;
	//SpawnerOwner->PlayerPoints
	//int32 index = SpawnerOwner->PlayerPoints.Num() - PointIndex - 1;
	//if (index < 0)
		//return;

	auto playerPoint = CurrentPoint->GetValue();
	if (!playerPoint)
		return;

	auto loc = playerPoint->position;
	auto rot = playerPoint->rotation; 

	SetActorLocationAndRotation(loc, rot);

	CurrentPoint = CurrentPoint->GetNextNode();
	////UE_LOG(LogTemp, Warning, TEXT("Set Transform "));
	
	
}


