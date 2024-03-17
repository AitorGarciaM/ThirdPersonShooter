// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "ThirdPersonShooterCharacter.h"
#include "ThirdPersonShooterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API AThirdPersonShooterPlayer : public AThirdPersonShooterCharacter
{
	GENERATED_BODY()
public:
	AThirdPersonShooterPlayer();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	UInputAction* AimAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	UInputAction* ShootAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	USpringArmComponent* CameraBoom;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	UCameraComponent* CameraFollow;

	virtual void BeginPlay() override;

	virtual FVector GetShootEndLocation(FVector Start) override;

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
