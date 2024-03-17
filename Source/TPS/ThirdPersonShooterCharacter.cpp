// Fill out your copyright notice in the Description page of Project Settings.


#include "ThirdPersonShooterCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AThirdPersonShooterCharacter::AThirdPersonShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = FindComponentByClass<USkeletalMeshComponent>();
	Rifle = CreateDefaultSubobject<USkeletalMeshComponent>(FName("Rifle"));

	Rifle->SetupAttachment(Mesh, "RH_Rifle");

	CharacterMovementComponent = GetCharacterMovement();

	Health = 100.0f;
	bIsAiming = false;
	bDoOnce = true;
}

// Called when the game starts or when spawned
void AThirdPersonShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AThirdPersonShooterCharacter::Shoot()
{
	if (bDoOnce)
	{
		bDoOnce = false;

		FTimerHandle FireRateTimeHandle;
		GetWorldTimerManager().SetTimer(FireRateTimeHandle, this, &AThirdPersonShooterCharacter::ResetShoot, FireRate, false);

		PlayShootAnimation();
		PlayShootFX();

		FHitResult HitResult;
		FVector Start = Rifle->GetSocketTransform(RifleMuzzleSocket, ERelativeTransformSpace::RTS_World).GetLocation();
		FVector End = GetShootEndLocation(Start);

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility))
		{
			AActor* HitActor = HitResult.GetActor();
			UGameplayStatics::ApplyDamage(HitActor, 5.0f, nullptr, this, UDamageType::StaticClass());
			FRotator Rotation = UKismetMathLibrary::MakeRotFromX(HitResult.Normal);

			PlayHitFX(HitResult.ImpactPoint, Rotation);
		}
	}
}

// Called every frame
void AThirdPersonShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AThirdPersonShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Health > 0.0f)
	{
		Health -= DamageAmount;
	}
	else
	{
		Mesh->SetSimulatePhysics(true);
		DisableInput(nullptr);
	}

	return Health;
}

void AThirdPersonShooterCharacter::StartAiming()
{
	bIsAiming = true;
	Aim();
}

void AThirdPersonShooterCharacter::EndAiming()
{
	bIsAiming = false;
	Aim();
}

void AThirdPersonShooterCharacter::AimForward()
{
	CharacterMovementComponent->bOrientRotationToMovement = !bIsAiming;
	CharacterMovementComponent->bUseControllerDesiredRotation = bIsAiming;
}

void AThirdPersonShooterCharacter::Aim()
{
	AimForward();
	CameraAim();
}

void AThirdPersonShooterCharacter::StartShooting()
{
	AimForward();
}

void AThirdPersonShooterCharacter::StopShooting()
{
	AimForward();
}

void AThirdPersonShooterCharacter::ResetShoot()
{
	bDoOnce = true;
}
