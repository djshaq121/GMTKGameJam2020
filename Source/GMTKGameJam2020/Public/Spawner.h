// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spawner.generated.h"

class AEnemy;

USTRUCT(BlueprintType)
struct FPlayerPoint
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector position;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator rotation;

	FPlayerPoint()
	{

	}

	FPlayerPoint(FVector pos, FRotator rot)
	{
		position = pos;
		rotation = rot;
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

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TArray<float> itemArray;

	TArray<FPlayerPoint*> PlayerPoints;
		
	APawn* Target;

	UPROPERTY(EditDefaultsOnly, Category = "Spawner")
	TSubclassOf<AEnemy> EnemyToSpawn;

	UFUNCTION(BlueprintCallable, Category = "Spawner")
		void BeginSpawning();

	UFUNCTION(BlueprintCallable, Category = "Spawner")
	void StartRecordingTargetPoint();

	void Record();

	void StopRecording();

	void DestroySpawnedEnemies();


private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawner", meta = (AllowPrivateAccess = "true"))
	float FirstSpawnDelay = 2.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawner", meta = (AllowPrivateAccess = "true"))
	float TimeBetweenSpawningEnemy = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawner", meta = (AllowPrivateAccess = "true"))
	int32 SpawnAmount = 5;

	int32 CurrentNumberOfSpawn = 0;

	FTimerHandle SpawnRateHandler;

	void Spawn();

	bool bSpawning = false;

	bool bRecording = false;

	bool bCanRecord = true;

	TArray<AEnemy*> EnemiesSpawned;
};
