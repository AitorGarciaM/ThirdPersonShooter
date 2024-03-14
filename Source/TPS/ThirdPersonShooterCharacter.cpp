// Fill out your copyright notice in the Description page of Project Settings.


#include "ThirdPersonShooterCharacter.h"

// Sets default values
AThirdPersonShooterCharacter::AThirdPersonShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AThirdPersonShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AThirdPersonShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AThirdPersonShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

