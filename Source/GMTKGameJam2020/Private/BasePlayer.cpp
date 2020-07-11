// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"

// Sets default values
ABasePlayer::ABasePlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootCapsule = Cast<UCapsuleComponent>(RootComponent);
	RootCapsule->OnComponentHit.AddDynamic(this, &ABasePlayer::OnCompHit);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;//Stops the player rotating with the camera
	SpringArm->SocketOffset = FVector(0, 0, 50);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
	
	// set our turn rates for input
	BaseTurnRate = 45.f;
	BasePitchRate = 45.f;

	WallJumpDirection = FVector::ZeroVector;
}

// Called when the game starts or when spawned
void ABasePlayer::BeginPlay()
{
	Super::BeginPlay();

	PlayerGravity = GetWorld()->GetGravityZ();
}

// Called every frame
void ABasePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!GetCharacterMovement())
		return;

	if (!GetCharacterMovement()->IsFalling())
	{
		bCanJump = true;
		bCanWallJump = false;
		bDisableJumpOnce = true;
		WallJumpDirection = FVector::ZeroVector;
	}
	else
	{
		if (bDisableJumpOnce)
		{
			bDisableJumpOnce = false;
			GetWorldTimerManager().SetTimer(JumpBufferTimer, this, &ABasePlayer::DisableJump, 1, false, JumpBuffTime);
		}
		
	}

	if (GetVelocity().Z < 0)
	{
		GetCharacterMovement()->Velocity.Z += PlayerGravity * (fallMultipler - 1) * DeltaTime;
	}
	else if(GetVelocity().Z > 0 && !bJumpHeld)
	{
		GetCharacterMovement()->Velocity.Z += PlayerGravity * (lowJumpMultipler - 1) * DeltaTime;
	}
	
}

// Called to bind functionality to input
void ABasePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABasePlayer::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ABasePlayer::EndJump);

	PlayerInputComponent->BindAxis("Turn", this, &ABasePlayer::Turn);
	PlayerInputComponent->BindAxis("MoveForward", this, &ABasePlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABasePlayer::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ABasePlayer::LookUp);

}

void ABasePlayer::OnCompHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Called Hit Wall"));
	if (GetCharacterMovement() && GetCharacterMovement()->IsFalling())
	{
		if (Hit.Normal.Y < 0.1f)
		{
			UE_LOG(LogTemp, Warning, TEXT("Touching Wall"));
			WallJumpDirection = Hit.Normal;
			bCanWallJump = true;
			//DrawDebugLine(GetWorld(), Hit.ImpactPoint, Hit.ImpactNormal, FColor::Red, false, 3);
			
		}
		//UE_LOG(LogTemp, Warning, TEXT("Grounded: %s"), (grounded ? TEXT("True") : TEXT("False")));
	}
}

void ABasePlayer::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ABasePlayer::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void ABasePlayer::Turn(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ABasePlayer::LookUp(float Rate)
{
	AddControllerPitchInput(Rate * BasePitchRate * GetWorld()->GetDeltaSeconds());
}

void ABasePlayer::StartJump()
{
	//ACharacter::LaunchCharacter(FVector(0,0, jumpHeight), false, true);

	if (bCanJump)
	{
		UE_LOG(LogTemp, Warning, TEXT("Normal Jump"));
		Jumping();
	}
	else if(bCanWallJump)
	{
		UE_LOG(LogTemp, Warning, TEXT("Wall Jump"));
		WallJump();
	}
	
	bJumpHeld = true;
}

void ABasePlayer::EndJump()
{
	bJumpHeld = false;
}

void ABasePlayer::DisableJump()
{
	if (!GetCharacterMovement()->IsFalling())
		return;

	UE_LOG(LogTemp, Warning, TEXT("Disable Jump"));
	bCanJump = false;
}
void ABasePlayer::Jumping()
{
	bCanJump = false;
	ACharacter::LaunchCharacter(FVector(0, 0, jumpHeight), false, true);
}

void ABasePlayer::WallJump()
{
	if (WallJumpDirection == FVector::ZeroVector)
		return;

	bCanWallJump = false;
	
	FRotator LaunchRotation = WallJumpDirection.Rotation();
	LaunchRotation.Pitch += 45;
	FVector LaunchVelocity = LaunchRotation.Vector() * 800.f;
	
	//FVector LaunchVelocity = WallJumpDirection * 500.f;
	ACharacter::LaunchCharacter(LaunchVelocity, true, true);
	SetActorRotation(WallJumpDirection.Rotation());
}






