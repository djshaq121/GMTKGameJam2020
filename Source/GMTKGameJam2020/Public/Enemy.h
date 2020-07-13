// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

class ASpawner;
struct FPlayerPoint;

UCLASS()
class GMTKGAMEJAM2020_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void FindFirstPlayerPoint();

	ASpawner* SpawnerOwner;
private:

	void FollowTarget();

	int32 PointIndex = 0;

	TDoubleLinkedList<FPlayerPoint*>::TDoubleLinkedListNode* CurrentPoint;

	bool bFoundFirstPlayerPoint = false;


};
