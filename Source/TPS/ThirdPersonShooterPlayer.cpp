// Fill out your copyright notice in the Description page of Project Settings.


#include "ThirdPersonShooterPlayer.h"
#include "EnhancedInputComponent.h"
#include "Kismet/KismetMathLibrary.h"

AThirdPersonShooterPlayer::AThirdPersonShooterPlayer() : Super()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(FName("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	CameraFollow = CreateDefaultSubobject<UCameraComponent>(FName("FollowCamera"));
	CameraFollow->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraFollow->bUsePawnControlRotation = false;
}

void AThirdPersonShooterPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (CameraFollow == nullptr)
	{
		CameraFollow = FindComponentByClass<UCameraComponent>();
	}
}

FVector AThirdPersonShooterPlayer::GetShootEndLocation(FVector Start)
{
	FVector CameraForwardVector = UKismetMathLibrary::GetForwardVector(CameraFollow->GetComponentRotation());
	return FVector(CameraForwardVector.X * 3000, CameraForwardVector.Y * 3000, CameraForwardVector.Z * 3000) + Start;
}

// Called to bind functionality to input
void AThirdPersonShooterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	Input->BindAction(AimAction, ETriggerEvent::Started, this, &AThirdPersonShooterPlayer::StartAiming);
	Input->BindAction(AimAction, ETriggerEvent::Completed, this, &AThirdPersonShooterPlayer::EndAiming);
	Input->BindAction(ShootAction, ETriggerEvent::Started, this, &AThirdPersonShooterPlayer::StartShooting);
	Input->BindAction(ShootAction, ETriggerEvent::Completed, this, &AThirdPersonShooterPlayer::StopShooting);
	Input->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AThirdPersonShooterPlayer::Shoot);
}

