// Fill out your copyright notice in the Description page of Project Settings.

#include "Spawner.h"
#include "Containers/CircularQueue.h"
#include "TimerManager.h"
#include "Enemy.h"


// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();
	
	Target = GetWorld()->GetFirstPlayerController()->GetPawn();

	if (Target)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Some warning message %s "), *Target->GetName());
	}
		
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bRecording)
	{
		Record();
	}
}

void ASpawner::BeginSpawning()
{
	if (bSpawning || !EnemyToSpawn)
		return;

	bSpawning = true;
	GetWorldTimerManager().SetTimer(SpawnRateHandler, this, &ASpawner::Spawn, TimeBetweenSpawningEnemy, true, 1.f);
}

void ASpawner::StartRecordingTargetPoint()
{
	if (bRecording)
		return;

	bRecording = true;
}

void ASpawner::Record()
{
	if (Target)
	{
		PlayerPoints.EmplaceAt(0, new FPlayerPoint(Target->GetActorLocation(), Target->GetActorRotation()));
		//UE_LOG(LogTemp, Warning, TEXT("Pos %s "), *Target->GetActorLocation().ToString());
	}
}

void ASpawner::Spawn()
{

	if (EnemiesSpawned.Num() >= SpawnAmount)
	{
		GetWorldTimerManager().ClearTimer(SpawnRateHandler);
		UE_LOG(LogTemp, Warning, TEXT("No more "));
		return;
	}
		
	UE_LOG(LogTemp, Warning, TEXT("Keep Spawning "));
	// Spawn eNEMY
	auto EnemySpanwed = GetWorld()->SpawnActor<AEnemy>(EnemyToSpawn, GetActorLocation(), GetActorRotation());
	EnemySpanwed->SpawnerOwner = this;
	EnemiesSpawned.Add(EnemySpanwed);
}

