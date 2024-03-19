// Fill out your copyright notice in the Description page of Project Settings.

#include "AIController.h"
#include "NavigationSystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "ThirdPersonShooterAI.h"
#include "ThirdPersonShooterPlayer.h"

AThirdPersonShooterAI::AThirdPersonShooterAI() : Super()
{
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(FName("Pawn Sensing"));
}


FVector AThirdPersonShooterAI::GetShootEndLocation(FVector Start)
{
	FVector RightVector = UKismetMathLibrary::GetRightVector(Mesh->GetComponentRotation());
	FVector RifleMuzzleLocation = Rifle->GetSocketTransform(RifleMuzzleSocket).GetLocation();


	return FVector(RightVector.X * 3000.0f, RightVector.Y * 3000.0f, RightVector.Z * 3000.0f) + RifleMuzzleLocation;
}

void AThirdPersonShooterAI::BeginPlay()
{
	Super::BeginPlay();

	NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
	AIController = Cast<AAIController>(GetController());
	
	AIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &AThirdPersonShooterAI::OnAIMoveCompleted);
	
	StartSearching();
}

void AThirdPersonShooterAI::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PawnSensing->OnSeePawn.AddDynamic(this, &AThirdPersonShooterAI::OnSeePawn);
}

void AThirdPersonShooterAI::StartSearching()
{
	FTimerHandle SearchTimer;
	GetWorldTimerManager().SetTimer(SearchTimer, this, &AThirdPersonShooterAI::SearchAround, 0.25f, false);

	bHasSeenTarget = false;
}

void AThirdPersonShooterAI::SearchAround()
{
	if (NavArea && !bHasSeenTarget)
	{
		if (NavArea->K2_GetRandomReachablePointInRadius(GetWorld(), GetActorLocation(), RandomTargetLocation, 2000.0f))
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), RandomTargetLocation);
		}
	}
}

void AThirdPersonShooterAI::OnAIMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (Result.IsSuccess())
	{
		FTimerHandle WaitTime;
		GetWorldTimerManager().SetTimer(WaitTime, this, &AThirdPersonShooterAI::SearchAround, 1.0f, false);
	}
}

void AThirdPersonShooterAI::ChaseTarget(APawn* TargetPawn)
{
	FVector Distance = TargetPawn->GetActorLocation() - GetActorLocation();
	
	if (Distance.Length() <= 300.0f)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Shooting Player"));
		bIsShootingPlayer = true;
		OrientToTarget(TargetPawn);
	}
	else
	{
		bIsShootingPlayer = false;
		UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), TargetPawn);
	}
}

void AThirdPersonShooterAI::Tick(float DeltaTime)
{
	if (bIsShootingPlayer && !bIsDeath)
	{
		Shoot();
	}
}

void AThirdPersonShooterAI::OnSeePawn(APawn* OtherPawn)
{
	AThirdPersonShooterPlayer* Player = Cast<AThirdPersonShooterPlayer>(OtherPawn);

	FVector Distance = OtherPawn->GetActorLocation() - GetActorLocation();

	if (Distance.Length() > 300.0f)
	{
		bIsShootingPlayer = false;
		StopShooting();
	}

	if (Player != nullptr)
	{
		bHasSeenTarget = true;
		ChaseTarget(OtherPawn);
	}
	else
	{
		bHasSeenTarget = false;
		bIsShootingPlayer = false;
	}
}

void AThirdPersonShooterAI::OnHearNoise(APawn* OtherPawn, const FVector& Location, float Volume)
{
}

void AThirdPersonShooterAI::OrientToTarget(APawn* TargetPawn)
{
	FRotator DesiredRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetPawn->GetActorLocation());

	GetController()->SetControlRotation(DesiredRotation);
}