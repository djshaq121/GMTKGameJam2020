// Fill out your copyright notice in the Description page of Project Settings.

#include "Spawner.h"
#include "TimerManager.h"
#include "Enemy.h"
#include "..\Public\Spawner.h"
#include "HealthComponent.h"
#include "BasePlayer.h"
#include "GameFramework/CharacterMovementComponent.h"


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
	
	Target = Cast<ABasePlayer>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (Target)
	{
		TargetHealthComponent = Target->FindComponentByClass<UHealthComponent>();
		if (TargetHealthComponent)
		{
			TargetHealthComponent->OnHealthChange.AddDynamic(this, &ASpawner::OnTargetHealthChange);
		}
		else
		{
			Target = nullptr;
			UE_LOG(LogTemp, Error, TEXT("Target in %s needs to have HealthComponent "), *GetName());
		}
	}
		
}

void ASpawner::OnTargetHealthChange(UHealthComponent * OwningHealthComp, float Health, float HealthDelta, FVector HitDirection, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	if (Health <= 0)
		ResetRecording();
}

void ASpawner::ResetRecording()
{
	StopRecording();
	// Stop Spawning
	GetWorldTimerManager().ClearTimer(SpawnRateHandler);
	PlayerPoints.Empty();
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bRecording)
	{
		Record();
	}

	if (bCanRemoveRecords)
	{
		RemoveFirstRecord();
	}
}

void ASpawner::BeginSpawning()
{
	StartRecordingTargetPoint();

	if (bSpawning || !EnemyToSpawn)
		return;

	bSpawning = true;
	GetWorldTimerManager().SetTimer(SpawnRateHandler, this, &ASpawner::Spawn, TimeBetweenSpawningEnemy, true, FirstSpawnDelay);
}

void ASpawner::StartRecordingTargetPoint()
{
	if (bRecording)
		return;

	bRecording = true;
}

void ASpawner::Record()
{
	if (Target && bCanRecord)
	{
		PlayerPoints.AddTail(new FPlayerPoint(Target->GetActorLocation(), Target->GetActorRotation(), Target->GetVelocity(), Target->GetCharacterMovement()->IsFalling()));
	}
		
}

void ASpawner::Spawn()
{

	if (EnemiesSpawned.Num() >= SpawnAmount)
	{
		GetWorldTimerManager().ClearTimer(SpawnRateHandler);
	
		// Remove recording after a period of time 
		if(!bCanRemoveRecords)
			GetWorldTimerManager().SetTimer(RemoveRecordingHandler, this, &ASpawner::StartRemovingRecords, RemoveRecordTimerDelay, false);

		return;
	}
		
	//UE_LOG(LogTemp, Warning, TEXT("Keep Spawning "));
	// Spawn eNEMY
	auto EnemySpanwed = GetWorld()->SpawnActor<AEnemy>(EnemyToSpawn, GetActorLocation(), GetActorRotation());
	if (EnemySpanwed)
	{
		EnemySpanwed->SpawnerOwner = this;
		EnemiesSpawned.Add(EnemySpanwed);
	}
}

void ASpawner::StopRecording()
{
	bCanRecord = false;
	bRecording = false;
}

void ASpawner::StartRemovingRecords()
{
	bCanRemoveRecords = true;
}

void ASpawner::RemoveFirstRecord()
{
	if (PlayerPoints.Num() <= 0)
		return;

	PlayerPoints.RemoveNode(PlayerPoints.GetHead());
}

void ASpawner::DestroySpawnedEnemies()
{

}

