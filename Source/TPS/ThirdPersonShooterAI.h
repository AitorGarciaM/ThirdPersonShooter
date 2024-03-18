// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AITypes.h"
#include "CoreMinimal.h"
#include "Perception/PawnSensingComponent.h"
#include "Navigation/PathFollowingComponent.h"

#include "ThirdPersonShooterCharacter.h"
#include "ThirdPersonShooterAI.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API AThirdPersonShooterAI : public AThirdPersonShooterCharacter
{
	GENERATED_BODY()
public:
	AThirdPersonShooterAI();
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UPawnSensingComponent* PawnSensing;
	class UNavigationSystemV1* NavArea;
	class AAIController* AIController;

	FVector RandomTargetLocation;

	bool bHasSeenTarget;
	bool bHasReachedLocation;
	bool bIsShootingPlayer;

	
	virtual FVector GetShootEndLocation(FVector Start) override;
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	void StartSearching();
	void SearchAround();
	void OnAIMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result);
	void ChaseTarget(APawn* TargetPawn);
	void OrientToTarget(APawn* TargetPawn);

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnSeePawn(APawn* OtherPawn);

	UFUNCTION()
	void OnHearNoise(APawn* OtherPawn, const FVector& Location, float Volume);
};
