// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "ThirdPersonShooterCharacter.generated.h"

UCLASS()
class TPS_API AThirdPersonShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AThirdPersonShooterCharacter();

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shooter Character Mesh")
	USkeletalMeshComponent* Rifle;

	USkeletalMeshComponent* Mesh;
	UCharacterMovementComponent* CharacterMovementComponent;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shooter Character Mesh")
	FName RifleParentSocket;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Shooter Character Mesh")
	FName RifleMuzzleSocket;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
	float FireRate;

	/* Stores if character is Aiming. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= "Shooter Character")
	bool bIsAiming;

	bool bDoOnce;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void CameraAim();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayShootAnimation();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayShootFX();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayHitFX(const FVector Location, const FRotator Rotation);

	UFUNCTION(BlueprintCallable)
	void Shoot();

	virtual FVector GetShootEndLocation(FVector Start) { return FVector(); }

	void StartAiming();
	void EndAiming();
	void AimForward();
	void Aim();

	void StartShooting();
	void StopShooting();
	void ResetShoot();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
};
