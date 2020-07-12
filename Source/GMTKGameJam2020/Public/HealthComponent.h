// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_SevenParams(FOnHealthChangeSignature, UHealthComponent*, HealthComp, float, Health, float, HealthChange, FVector, HitDirection, const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GMTKGAMEJAM2020_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f), Category = "HealthComponent")
		float MaxHealth = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f), Category = "HealthComponent")
		float CurrentHealth;

public:	
	UFUNCTION()
	void OnHandleTakeDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable, Category = "Character Condition")
		float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Character Condition")
		float GetMaxHealth() const;

	UPROPERTY(BlueprintAssignable, Category = "Event")
		FOnHealthChangeSignature OnHealthChange;

};
