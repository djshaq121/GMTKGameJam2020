// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spawner.generated.h"

class AEnemy;
class UHealthComponent;
class ABasePlayer;

USTRUCT(BlueprintType)
struct FPlayerPoint
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector position;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator rotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector targetVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsFalling;

	FPlayerPoint()
	{

	}

	FPlayerPoint(FVector pos, FRotator rot, FVector vel, bool falling)
	{
		position = pos;
		rotation = rot;
		targetVelocity = vel;
		bIsFalling = falling;
	}

};

UCLASS()
class GMTKGAMEJAM2020_API ASpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TDoubleLinkedList<FPlayerPoint*> PlayerPoints;
		
	ABasePlayer* Target;

	UPROPERTY(EditDefaultsOnly, Category = "Spawner")
	TSubclassOf<AEnemy> EnemyToSpawn;

	UFUNCTION(BlueprintCallable, Category = "Spawner")
		void BeginSpawning();

	UFUNCTION(BlueprintCallable, Category = "Spawner")
	void StartRecordingTargetPoint();
	
	UFUNCTION()
	void OnTargetHealthChange(UHealthComponent * OwningHealthComp, float Health, float HealthDelta, FVector HitDirection, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser);

	void Record();

	void StopRecording();

	void StartRemovingRecords();

	void RemoveFirstRecord();

	void DestroySpawnedEnemies();

	void ResetRecording();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawner", meta = (AllowPrivateAccess = "true"))
	float FirstSpawnDelay = 2.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawner", meta = (AllowPrivateAccess = "true"))
	float TimeBetweenSpawningEnemy = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawner", meta = (AllowPrivateAccess = "true"))
	float RemoveRecordTimerDelay = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner", meta = (AllowPrivateAccess = "true"))
	int32 SpawnAmount = 5;

	int32 CurrentNumberOfSpawn = 0;

	FTimerHandle SpawnRateHandler;

	FTimerHandle RemoveRecordingHandler;

	void Spawn();

	bool bSpawning = false;

	bool bRecording = false;

	bool bCanRecord = true;

	bool bCanRemoveRecords = false;

	TArray<AEnemy*> EnemiesSpawned;

	UHealthComponent* TargetHealthComponent;
};
