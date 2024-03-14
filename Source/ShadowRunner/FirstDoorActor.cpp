// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstDoorActor.h"
#include "DrawDebugHelpers.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "ShadowMonsterSpwan.h"
#include "ShadowRunnerCharacter.h"
#include "Components/ArrowComponent.h"

extern float volumeControlEnemy;

// Sets default values
AFirstDoorActor::AFirstDoorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Root
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

	// Collider.
	//boxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("boxCollider"));
	//boxCollider->InitBoxExtent(FVector(250.0f, 100.0f, 200.0f));
	//boxCollider->SetCollisionProfileName("Trigger");
	//boxCollider->SetupAttachment(RootComponent);
	
	// Arrow component.
	arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	arrow->SetupAttachment(RootComponent);

	// ID.
	keyId = 0; // ID not set.

	// Meshes.
	leftDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("leftDoorMesh"));
	leftDoorMesh->SetupAttachment(RootComponent);
	rightDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("rightDoorMesh"));
	rightDoorMesh->SetupAttachment(RootComponent);

	// Door state.
	doorState = DoorState::CLOSED;

	// Math.
	dotProduct = 0.0f;
	maxDegree = 0.0f;
	minDegree = -90.0f;
	leftSign = 1.0f;
	rightSign = -1.0f;
	directionSign = 1.0f;
	leftDoorCurrentRotation = 0.0f;
	rightDoorCurrentRotation = 0.0f;
}

// Called when the game starts or when spawned
void AFirstDoorActor::BeginPlay()
{
	Super::BeginPlay();
	
	//DrawDebugbox(GetWorld(), GetActorLocation(), boxCollider->GetScaledBoxExtent(), FQuat(GetActorLocation, FColor::Turquoise, true, -1, 0, 2));
}

// Called every frame
void AFirstDoorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// New approach using enum state.
	if (doorState == DoorState::OPENING)
	{
		OpenDoor(DeltaTime);
	}

	if (doorState == DoorState::CLOSING)
	{
		CloseDoor(DeltaTime);
	}
}

void AFirstDoorActor::ToggleDoor(FVector characterFacing)
{
	if (!keyId) // If keyId is not set, it means the door is not locked.
	{
		// Setup door open direction.
		 float dotP = FVector::DotProduct(arrow->GetForwardVector(), characterFacing);

		 if (dotP >= 0)
		 {
			 directionSign = 1.0f;
		 }
		 else
		 {
			 directionSign = -1.0f;
		 }

		if (doorState == AFirstDoorActor::DoorState::CLOSED)
		{
			doorState = AFirstDoorActor::DoorState::OPENING;
			if (DoorOpenSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, DoorOpenSound, GetActorLocation(), GetActorRotation(), volumeControlEnemy);
			}
		}
		if (doorState == AFirstDoorActor::DoorState::OPENED)
		{
			doorState = AFirstDoorActor::DoorState::CLOSING;
			if (DoorOpenSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, DoorOpenSound, GetActorLocation(), GetActorRotation(), volumeControlEnemy);
			}
		}
	}
	else // Door locked.
	{
		AShadowRunnerHUD* shadowRunnerHUD = Cast<AShadowRunnerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
		if (shadowRunnerHUD)
		{
			shadowRunnerHUD->DisplayLocked();
		}
    if(DoorLockedSound != nullptr)
    {
      UGameplayStatics::PlaySoundAtLocation(this, DoorLockedSound, GetActorLocation(), volumeControlEnemy);
    }
	}
}

void AFirstDoorActor::UnlockDoor()
{
	if (doorState == AFirstDoorActor::DoorState::CLOSED)
	{
		keyId = 0; // keyId == 0 means the door is not locked.
		doorState = AFirstDoorActor::DoorState::OPENING;
	}
}

void AFirstDoorActor::CloseDoor(float DeltaTime)
{

	leftDoorCurrentRotation = leftDoorMesh->GetRelativeRotation().Yaw;
	rightDoorCurrentRotation = rightDoorMesh->GetRelativeRotation().Yaw;

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("In CloseDoor leftDoorCurrentRotation: %f"), leftDoorCurrentRotation));

	if (leftDoorCurrentRotation < -90.0f)
	{
		leftAddYaw = leftSign * DeltaTime * 80;
		rightAddYaw = rightSign * DeltaTime * 80;
	}
	else
	{
		leftAddYaw = -leftSign * DeltaTime * 80;
		rightAddYaw = -rightSign * DeltaTime * 80;
	}

	// New approach using enum state.
	if (FMath::IsNearlyEqual(leftDoorCurrentRotation, minDegree, 1.5f)) // minDegree = -90.0f
	{
		doorState = DoorState::CLOSED;
	}
	else if (doorState == DoorState::CLOSING) // leftDoor: (0.0f(opened) ~ -90.0f(closed)) or ()
	{
		FRotator leftNewRotation = FRotator(0.0f, leftAddYaw, 0.0f);
		FRotator rightNewRotation = FRotator(0.0f, rightAddYaw, 0.0f);

		leftDoorMesh->AddRelativeRotation(FQuat(leftNewRotation), false, 0, ETeleportType::None);
		rightDoorMesh->AddRelativeRotation(FQuat(rightNewRotation), false, 0, ETeleportType::None);
	}
	
}

void AFirstDoorActor::OpenDoor(float DeltaTime)
{
	leftDoorCurrentRotation = leftDoorMesh->GetRelativeRotation().Yaw;
	rightDoorCurrentRotation = rightDoorMesh->GetRelativeRotation().Yaw;

	leftAddYaw = directionSign * leftSign * DeltaTime * 80;
	rightAddYaw = directionSign * rightSign * DeltaTime * 80;

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%f"), leftDoorCurrentRotation));

	if (directionSign == -1.0f) // character facing and arrow are different direction.
	{
		maxDegree = -180.0f;
	}
	else // directionSign == 1.0f // character facing and arrow are the same direction.
	{
		maxDegree = 0.0f;
	}

	// New approach using enum state.
	if (FMath::IsNearlyEqual(leftDoorCurrentRotation, maxDegree, 1.5f)) // maxDegree = 0.0f
	{
		doorState = DoorState::OPENED;
	}
	else if (doorState == DoorState::OPENING) // leftDoor: -90.0f(closed) ~ 0.0f(opened)
	{
		FRotator leftNewRotation = FRotator(0.0f, leftAddYaw, 0.0f);
		FRotator rightNewRotation = FRotator(0.0f, rightAddYaw, 0.0f);

		leftDoorMesh->AddRelativeRotation(FQuat(leftNewRotation), false, 0, ETeleportType::None);
		rightDoorMesh->AddRelativeRotation(FQuat(rightNewRotation), false, 0, ETeleportType::None);
	}
}