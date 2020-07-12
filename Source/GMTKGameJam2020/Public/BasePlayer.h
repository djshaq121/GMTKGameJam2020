// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BasePlayer.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UCapsuleComponent;
class UHealthComponent;

UCLASS()
class GMTKGAMEJAM2020_API ABasePlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABasePlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void OnCompHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);


protected:
	UFUNCTION()
		void OnHealthChanged(UHealthComponent* OwningHealthComp, float Health, float HealthDelta, FVector HitDirection, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UHealthComponent* HealthComponent;
private:
	void MoveForward(float Value);

	void MoveRight(float Value);

	void Turn(float Rate);

	void LookUp(float Rate);

	void StartJump();

	void EndJump();

	void Jumping();

	void Dashing();

	void WallJump();

	void WallSlide();

	void CanJump();

	void DisableJump();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* Camera;

private:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float BaseTurnRate;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float BasePitchRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float jumpHeight = 600.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float fallMultipler = 2.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float lowJumpMultipler = 2.f;

	bool bCanJump = false;

	bool bCanWallJump = false;

	bool bJumpHeld = false;

	bool bDisableJumpOnce = true;

	FTimerHandle JumpBufferTimer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float JumpBuffTime = 0.1;

	float PlayerGravity = 1.f;

	UCapsuleComponent* RootCapsule;

	FVector WallJumpDirection;

	
};
