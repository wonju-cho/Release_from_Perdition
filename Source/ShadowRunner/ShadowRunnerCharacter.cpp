// Copyright Epic Games, Inc. All Rights Reserved.
#include "ShadowRunnerCharacter.h"
#include "ShadowRunnerProjectile.h"
#include "Animation/AnimInstance.h"
#include "ShadowRunnerGameMode.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "AmmoComponent.h"
#include "HealthComponent.h"
#include "ShadowRunnerHUD.h"
#include "Master_Interactable.h"
#include "DrawDebugHelpers.h"
#include "AIShadowCloneController.h"
#include "BaseWeapon.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "TimerManager.h"
#include "AITags.h"
#include "Engine/Classes/Engine/Engine.h"
#include "FirstDoorActor.h"
#include "EngineUtils.h"
#include "AImagician/AIMagicController.h"
#include "ShadowCloneCharacter.h"
#include "ShadowRunner/AImagician/AIMagic.h"
#include "LightsOutPuzzleActor.h"
#include "FlyingEnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AmmoWidget.h" //INFINITE
#include "Components/ArrowComponent.h"
#include "Materials/MaterialParameterCollectionInstance.h"
//#include "MediaAssets/Public/TimeSynchronizableMediaSource.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

float volumeControlEnemy = 1.5f;

//////////////////////////////////////////////////////////////////////////
// AShadowRunnerCharacter

AShadowRunnerCharacter::AShadowRunnerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	inputVector = FVector(0.f);
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	deathcheck = false;
	this->JumpMaxCount = 2;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->SetupAttachment(GetMesh());

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(false);			// otherwise won't be visible in the multiplayer
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);
	//FP_Gun->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.

	// Create VR Controllers.
	//R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	//R_MotionController->MotionSource = FXRMotionControllerBase::RightHandSourceId;
	//R_MotionController->SetupAttachment(RootComponent);
	//L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	//L_MotionController->SetupAttachment(RootComponent);

	// Create a gun and attach it to the right-hand VR controller.
	// Create a gun mesh component
	//VR_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VR_Gun"));
	//VR_Gun->SetOnlyOwnerSee(false);			// otherwise won't be visible in the multiplayer
	//VR_Gun->bCastDynamicShadow = false;
	//VR_Gun->CastShadow = false;
	//VR_Gun->SetupAttachment(R_MotionController);
	//VR_Gun->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	//VR_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("VR_MuzzleLocation"));
	//VR_MuzzleLocation->SetupAttachment(VR_Gun);
	//VR_MuzzleLocation->SetRelativeLocation(FVector(0.000004, 53.999992, 10.000000));
	//VR_MuzzleLocation->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));		// Counteract the rotation of the VR gun model.

	// Uncomment the following line to turn motion controllers on by default:
	//bUsingMotionControllers = true;

	ammo = CreateDefaultSubobject<UAmmoComponent>(TEXT("ammo"));
	HealthBar = CreateDefaultSubobject<UHealthComponent>(TEXT("healthbar"));

	//this->AddComponent("ammo");
	curAmmo = ammo->GetAmmoCount();

	//////////player controller
	jumpHeight = 800.f;

	jumpCount = 0;

	walkSpeed = 1600.f;

	crouchSpeed = 800.f;

	dashSpeed = 6000.0f;

	dashCount = 0.f;

	dashDistacne = 6000.f;

	dashColdown = 1.f;

	canDash = true;

	dashStop = 0.1f;

	mouse_sensitivity = 1.f;

	//SetUpStimulus();

	// Shadow clone variables.
	spawnedActorRef = nullptr;

	shadowIsActive = false;

	//weapon
	weapon = nullptr;
	play = true;

	pistolDamage = 25.0f;

	weaponIndex = 0;

	assaultRifleAmmo = 10;
	shotgunAmmo = 10;

	lifes = 3;

	// Shadow clone variables.
	shadowCloneCoolDown = 3.0f;
	shadowCloneCoolDownTimer = 0.0f;
	bShadowIsOnCoolDown = false;
	shadowCloneHealthSpent = 0.0f;
	shadowSpawnCoolDown = 3.0f;
	shadowSpawnCoolDownTimer = 0.0f;
	bShadowSpawnIsOnCoolDown = false;
	shadowSpawnRange = 600.0f;

	isItemSpawned = false;
	isDashItemSpawned = false;

	// Ability flags.
	bDashGained = false;
	jumpNumber = 1;

	// Survive time.
	surviveTime = 0.0f;
	surviveTimeSec = 0.0f;
	surviveTimeMin = 0;
	surviveTimeHour = 0;
	checkPointcheck = false;
	WalkSoundCoolDown = 0.f;

	currentHealth = 100.f;
	previousHealth = 100.f;

	shadowExplodeRadius = 250.0f;

	playerDied = false;

	muzzleParticleRef = nullptr;
	muzzleParticleDisplayTime = 8.0f;

	//sibal = GetGameInstance();

	//static ConstructorHelpers::FClassFinder<UGameInstance> hihi(TEXT("/Game/BluePrints/ShadowRunnerGameInstance_BP.ShadowRunnerGameInstance_BP_C"));
	//WidgetUIClass = WidgetUIBPClass.Class;
	//sibal->FindFunction(TEXT("Get Volume"));
	//FOutputDeviceNull hiww;
	//float his = sibal->CallFunctionByNameWithArguments(TEXT("Get Volume"), hiww, this);

	volumeControlEnemy = soundControl;

	ConstructorHelpers::FObjectFinder<UMaterialParameterCollection>objectType(TEXT("/Game/Assets/Materials/mpc_PostProcess"));
	if(objectType.Succeeded())
	{
		collection = objectType.Object;
	}
	
	hasSwapped = false;
}

float AShadowRunnerCharacter::GetMouseSensitivity()
{
	return mouse_sensitivity;
}

void AShadowRunnerCharacter::SetMouseSensitivity(float s)
{
	mouse_sensitivity = s;
}

void AShadowRunnerCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	// Show or hide the two versions of the gun based on whether or not we're using motion controllers.
	//if (bUsingMotionControllers)
	//{
	//	VR_Gun->SetHiddenInGame(false, true);
	//	Mesh1P->SetHiddenInGame(true, true);
	//}
	//else
	//{
	//	VR_Gun->SetHiddenInGame(true, true);
		Mesh1P->SetHiddenInGame(false, true);
	//}

	originMesh = GetActorLocation();

	AShadowRunnerHUD* shadowRunnerHUD = Cast<AShadowRunnerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	shadowRunnerHUD->UpdateLifeSystem(lifes);

	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;

	respawnFacing = FirstPersonCameraComponent->GetForwardVector().Rotation();
}

//////////////////////////////////////////////////////////////////////////
// Input

void AShadowRunnerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AShadowRunnerCharacter::Doublejump);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AShadowRunnerCharacter::OnFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AShadowRunnerCharacter::stopFire);

	PlayerInputComponent->BindAction("SwitchPrimary", IE_Pressed, this, &AShadowRunnerCharacter::SwitchToNextPrimaryWeapon);

	//PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AShadowRunnerCharacter::manualRelaod);

	//sprint
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AShadowRunnerCharacter::CrouchStart);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AShadowRunnerCharacter::CrouchEnd);

	//dash
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AShadowRunnerCharacter::Dash);

	// Shadow spawn.
	PlayerInputComponent->BindAction("ShadowSpawn", IE_Pressed, this, &AShadowRunnerCharacter::OnShadowSpawn);

	// Shadow swap.
	PlayerInputComponent->BindAction("ShadowSwap", IE_Pressed, this, &AShadowRunnerCharacter::OnShadowSwap);

	// interact
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AShadowRunnerCharacter::Interact);

	// Test_CloseDoor
	//PlayerInputComponent->BindAction("Test_CloseDoor", IE_Pressed, this, &AShadowRunnerCharacter::OnTest_CloseDoor);

	// Puzzle cheat code.
	//PlayerInputComponent->BindAction("PuzzleCheat", IE_Pressed, this, &AShadowRunnerCharacter::OnPuzzleCheat);

	// health cheat
	PlayerInputComponent->BindAction("JumpUnlimit", IE_Pressed, this, &AShadowRunnerCharacter::OnHealthCheat);
	PlayerInputComponent->BindAction("JumpUnlimitOff", IE_Pressed, this, &AShadowRunnerCharacter::OnHealthCheatOff);

	// jump cheat
	PlayerInputComponent->BindAction("JumpUnlimit", IE_Pressed, this, &AShadowRunnerCharacter::OnJumpUnlimit);
	PlayerInputComponent->BindAction("JumpUnlimitOff", IE_Pressed, this, &AShadowRunnerCharacter::OnJumpUnlimitOff);

	// Enable touchscreen input
	//EnableTouchscreenMovement(PlayerInputComponent);

	//PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AShadowRunnerCharacter::OnResetVR);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AShadowRunnerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShadowRunnerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Flying", this, &AShadowRunnerCharacter::MoveFlying);

	PlayerInputComponent->BindAxis("ZoomIn", this, &AShadowRunnerCharacter::Zoom);
	PlayerInputComponent->BindAxis("ZoomOut", this, &AShadowRunnerCharacter::Zoom);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AShadowRunnerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AShadowRunnerCharacter::LookUpAtRate);
}

void AShadowRunnerCharacter::startFire()
{
	if (weapons[weaponIndex])
	{
		if (weapons[weaponIndex]->GetDefaultObject<ABaseWeapon>()->clipAmmo > 0)
		{
			FHitResult Hit;
			const float WeaponRange = 20000.f;
			//const FVector startTrace = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + GetControlRotation().RotateVector(GunOffset);
			const FVector startTrace = FirstPersonCameraComponent->GetComponentLocation();
			//const FVector endTrace = (FirstPersonCameraComponent->GetForwardVector() * WeaponRange) + startTrace;
			FRotator controlRotation = GetControlRotation();
			FVector directionVector = controlRotation.Vector();
		  directionVector.Normalize();
			//FVector startTrace = (FP_MuzzleLocation ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + controlRotation.RotateVector(GunOffset);
			FVector endTrace = startTrace + (directionVector * WeaponRange);
			FCollisionQueryParams QueryParams = FCollisionQueryParams(SCENE_QUERY_STAT(WeaponTrace), false, this);
			//DrawDebugLine(GetWorld(), startTrace, endTrace, FColor::Green, false, 1, 0, 1);

			if (GetWorld()->LineTraceSingleByChannel(Hit, startTrace, endTrace, ECC_Pawn, QueryParams))		//weapons[weaponIndex]->GetDefaultObject<ABaseWeapon>()->clipAmmo -= 1;
			{
				AEnemyCharacter* enemy = Cast<AEnemyCharacter>(Hit.GetActor());

				if (enemy != nullptr)
				{
					enemy->TakeDamage(pistolDamage);

				}

				AFlyingEnemyCharacter* flyEnemy = Cast<AFlyingEnemyCharacter>(Hit.GetActor());

				if (flyEnemy)
				{
					UGameplayStatics::PlaySoundAtLocation(this, GethurtSound, GetActorLocation(), soundControl);
					flyEnemy->health -= pistolDamage;
				}

				AAIMagic* magicianEnemy = Cast<AAIMagic>(Hit.GetActor());

				if (magicianEnemy)
				{
					UGameplayStatics::PlaySoundAtLocation(this, GethurtSound, GetActorLocation(), soundControl);
					magicianEnemy->health -= pistolDamage;
				}

				if (ImpactParticles)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, FTransform(Hit.ImpactNormal.Rotation(), Hit.ImpactPoint));
				}

				ADestructiblePot* pot = Cast<ADestructiblePot>(Hit.GetActor());

				if (pot)
				{
					UE_LOG(LogTemp, Warning, TEXT("damage with second bullet"));
					pot->Destroy(pot->DefaultDamage, Hit.Location, this->GetActorForwardVector(), pot->DefaultImpulse);
				}
			}

			if (MuzzleParticles)
			{
				if (muzzleParticleRef)
				{
					muzzleParticleRef->DestroyComponent();
				}
				muzzleParticleRef = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleParticles, FP_Gun->GetSocketTransform(FName("Muzzle")));
				if (!GetWorld()->GetTimerManager().IsTimerActive(muzzleParticleTimerHandle))
				{
					GetWorld()->GetTimerManager().SetTimer(muzzleParticleTimerHandle, this, &AShadowRunnerCharacter::MuzzleParticleTimerCallback, muzzleParticleDisplayTime, false);
				}
				//weapons[weaponIndex]->GetDefaultObject<ABaseWeapon>()->clipAmmo -= 1;
			}

			if (FireSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation(), soundControl*0.4f);
			}

			// try and play a firing animation if specified
			if (FireAnimation != nullptr)
			{
				// Get the animation object for the arms mesh
				UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();

				if (AnimInstance != nullptr)
				{
					AnimInstance->Montage_Play(FireAnimation, 1.f);
				}
			}
		}

	}

	AShadowRunnerHUD* shadowRunnerHUD = Cast<AShadowRunnerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	shadowRunnerHUD->UpdateAmmo(INFINITE, weapons[0]->GetDefaultObject<ABaseWeapon>()->clipAmmo, 10, weaponIndex);
}

void AShadowRunnerCharacter::stopFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_HandleRefire);
}

void AShadowRunnerCharacter::OnFire()
{
	if (playerDied)
	{
		return;
	}

	if (weapons[weaponIndex]->GetDefaultObject<ABaseWeapon>()->weaponType == EWeaponType::E_AssaultRifle)
	{
		if (weapons[weaponIndex])
		{
			if (weapons[weaponIndex]->GetDefaultObject<ABaseWeapon>()->clipAmmo > 0)
			{
				if (ProjectileClass != nullptr)
				{
					UWorld* const World = GetWorld();

					if (World != nullptr)
					{
						//if (bUsingMotionControllers)
						//{
						//	const FRotator SpawnRotation = VR_MuzzleLocation->GetComponentRotation();
						//	const FVector SpawnLocation = VR_MuzzleLocation->GetComponentLocation();
						//	World->SpawnActor<AShadowRunnerProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
						//}
						//else
						//{
							const FRotator SpawnRotation = GetControlRotation();
							// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
							const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);
							//	const FVector SpawnLocation = FirstPersonCameraComponent->GetComponentLocation();
								//Set Spawn Collision Handling Override
							FActorSpawnParameters ActorSpawnParams;
							//ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
							ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
							// spawn the projectile at the 

							World->SpawnActor<AShadowRunnerProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
						//}
						weapons[weaponIndex]->GetDefaultObject<ABaseWeapon>()->clipAmmo -= 1;
					}
				}
				if (FireSound != nullptr)
				{

					UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation(), soundControl*0.4f);
				}

				// try and play a firing animation if specified
				if (FireAnimation != nullptr)
				{
					// Get the animation object for the arms mesh
					UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();

					if (AnimInstance != nullptr)
					{
						AnimInstance->Montage_Play(FireAnimation, 1.f);
					}
				}
			}
		}
		AShadowRunnerHUD* shadowRunnerHUD = Cast<AShadowRunnerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
		shadowRunnerHUD->UpdateAmmo(weapons[weaponIndex]->GetDefaultObject<ABaseWeapon>()->clipAmmo, INFINITE, 10, weaponIndex);
	}

	if (weapons[weaponIndex]->GetDefaultObject<ABaseWeapon>()->weaponType == EWeaponType::E_Shotgun)
	{
		startFire();
		GetWorldTimerManager().SetTimer(TimerHandle_HandleRefire, this, &AShadowRunnerCharacter::startFire, TimeBetweenShots, true);
	}
	// try and fire a projectile
	//original Ammo system
	/*
	if (curAmmo == 0)
   {
	  ammo->Empty();
   }
   else
   {
	  //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("HAHAHA")));
	  // try and fire a projectile
	  if (ProjectileClass != nullptr)
	  {
		 //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("In OnFire")));
		 UWorld* const World = GetWorld();
		 if (World != nullptr)
		 {
			if (bUsingMotionControllers)
			{
			   const FRotator SpawnRotation = VR_MuzzleLocation->GetComponentRotation();
			   const FVector SpawnLocation = VR_MuzzleLocation->GetComponentLocation();
			   World->SpawnActor<AShadowRunnerProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
			}
			else
			{
			   const FRotator SpawnRotation = GetControlRotation();
			   // MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			   const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

			   //Set Spawn Collision Handling Override
			   FActorSpawnParameters ActorSpawnParams;
			   ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			   // spawn the projectile at the muzzle
			   World->SpawnActor<AShadowRunnerProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			}
		 }
	  }

	  // try and play the sound if specified
	  if (FireSound != nullptr)
	  {
		 UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	  }

	  // try and play a firing animation if specified
	  if (FireAnimation != nullptr)
	  {
		 // Get the animation object for the arms mesh
		 UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		 if (AnimInstance != nullptr)
		 {
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		 }
	  }

	  //ammo->SetAmmoCount(ammo->GetAmmoCount() - 1);
	  //ammo->SetAmmoCount(curAmmo);

	  curAmmo -= 1;
	  AShadowRunnerHUD* shadowRunnerHUD = Cast<AShadowRunnerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	  shadowRunnerHUD->UpdateAmmo(curAmmo, 10);
   }
	*/
}

void AShadowRunnerCharacter::manualRelaod()
{
	if (weapons[weaponIndex])
	{
		ReloadWeapon(weapons[weaponIndex]->GetDefaultObject<ABaseWeapon>()->weaponType);
	}
}

void AShadowRunnerCharacter::ReloadWeapon(EWeaponType _weaponType)
{
	if (playerDied)
	{
		return;
	}

	if (weapons[weaponIndex])
	{
		switch (_weaponType)
		{
		case EWeaponType::E_AssaultRifle:
			assaultRifleAmmo = CalculateAmmo(assaultRifleAmmo);
			break;

		case EWeaponType::E_Shotgun:
			assaultRifleAmmo = CalculateAmmo(assaultRifleAmmo);
			//shotgunAmmo = CalculateAmmo(shotgunAmmo);
			break;

		default:
			break;
		}
	}
}

int AShadowRunnerCharacter::CalculateAmmo(int _ammoAmount)
{
	if (_ammoAmount > 0)
	{
		if (weapons[weaponIndex]->GetDefaultObject<ABaseWeapon>()->clipAmmo != weapons[weaponIndex]->GetDefaultObject<ABaseWeapon>()->maxClipAmmo)
		{
			_ammoAmount -= (weapons[weaponIndex]->GetDefaultObject<ABaseWeapon>()->maxClipAmmo - weapons[weaponIndex]->GetDefaultObject<ABaseWeapon>()->clipAmmo);
			weapons[weaponIndex]->GetDefaultObject<ABaseWeapon>()->clipAmmo = weapons[weaponIndex]->GetDefaultObject<ABaseWeapon>()->maxClipAmmo;
		}
		else
		{
			weapons[weaponIndex]->GetDefaultObject<ABaseWeapon>()->clipAmmo = +_ammoAmount;
			_ammoAmount = 0;
		}
	}
	return _ammoAmount;
}

void AShadowRunnerCharacter::SwitchToNextPrimaryWeapon()
{
	if (playerDied)
	{
		return;
	}
	switch (weaponIndex)
	{
	case 0:
		if (weapons.Num() > 1)
		{
			weaponIndex = 1;
			SwitchWeaponMesh(weaponIndex);
			weapons[weaponIndex]->GetDefaultObject<ABaseWeapon>()->changeType(weaponIndex);
			manualRelaod();
			AShadowRunnerHUD* shadowRunnerHUD = Cast<AShadowRunnerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
			shadowRunnerHUD->UpdateAmmo(INFINITE, weapons[0]->GetDefaultObject<ABaseWeapon>()->clipAmmo, 10, weaponIndex);
		}
		else
		{
			weaponIndex = 0;
			SwitchWeaponMesh(weaponIndex);
			weapons[weaponIndex]->GetDefaultObject<ABaseWeapon>()->changeType(weaponIndex);
			manualRelaod();
			AShadowRunnerHUD* shadowRunnerHUD = Cast<AShadowRunnerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
			shadowRunnerHUD->UpdateAmmo(weapons[weaponIndex]->GetDefaultObject<ABaseWeapon>()->clipAmmo, INFINITE, 10, weaponIndex);
		}
		break;
	case 1:
		if (weapons.Num() > 2)
		{
			weaponIndex = 2;
			SwitchWeaponMesh(weaponIndex);
			weapons[weaponIndex]->GetDefaultObject<ABaseWeapon>()->changeType(weaponIndex);
			manualRelaod();
			AShadowRunnerHUD* shadowRunnerHUD = Cast<AShadowRunnerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
			shadowRunnerHUD->UpdateAmmo(weapons[weaponIndex]->GetDefaultObject<ABaseWeapon>()->clipAmmo, INFINITE, 10, weaponIndex);
		}
		else
		{
			weaponIndex = 0;
			SwitchWeaponMesh(weaponIndex);
			weapons[weaponIndex]->GetDefaultObject<ABaseWeapon>()->changeType(weaponIndex);
			manualRelaod();
			AShadowRunnerHUD* shadowRunnerHUD = Cast<AShadowRunnerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
			shadowRunnerHUD->UpdateAmmo(weapons[weaponIndex]->GetDefaultObject<ABaseWeapon>()->clipAmmo, INFINITE, 10, weaponIndex);
		}
		break;
	default:
		break;
	}

	if (SwitchWeaponSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, SwitchWeaponSound, GetActorLocation(), soundControl);
	}
}

void AShadowRunnerCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AShadowRunnerCharacter::OnShadowSpawn()
{

	if (playerDied)
	{
		return;
	}


	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("A shadow has spawned behind you.")));

	if (spawnedActorRef) // Restore the shadow.
	{
		RestoreShadow();
	}
	else
	{
		if (!bShadowSpawnIsOnCoolDown)
		{
			SpawnShadow();

			// Set cooldown flag to true.
			bShadowSpawnIsOnCoolDown = true;
			shadowSpawnCoolDownTimer = shadowSpawnCoolDown;
			AShadowRunnerHUD* shadowRunnerHUD = Cast<AShadowRunnerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
			shadowRunnerHUD->UpdateShadowSpawnCooldown(shadowSpawnCoolDownTimer, shadowSpawnCoolDown);
		}
	}
}

void AShadowRunnerCharacter::OnShadowSwap()
{
	pci = GetWorld()->GetParameterCollectionInstance(collection);
	
	if (playerDied)
	{
		return;
	}

	if (spawnedActorRef)
	{
		if (!bShadowIsOnCoolDown)
		{
			FVector shadowPositionOld = spawnedActorRef->GetActorLocation();
			FRotator shadowRotationOld = spawnedActorRef->GetActorRotation();

			spawnedActorRef->SetActorLocation(this->GetActorLocation());
			spawnedActorRef->SetActorRotation(this->GetActorRotation());

			this->SetActorLocation(shadowPositionOld);
			this->GetController()->SetControlRotation(shadowRotationOld);

			if (ShadowSwapSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, ShadowSwapSound, GetActorLocation(), soundControl);
			}

			// Set cooldown flag to true.
			bShadowIsOnCoolDown = true;
			shadowCloneCoolDownTimer = shadowCloneCoolDown;
			AShadowRunnerHUD* shadowRunnerHUD = Cast<AShadowRunnerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
			shadowRunnerHUD->UpdateShadowCooldown(shadowCloneCoolDownTimer, shadowCloneCoolDown);

			if(pci)
			{
				bool found = pci->SetScalarParameterValue(FName("TPEffectAmount"), 1.0);
			}
			hasSwapped = true;
		}
		
	}
	
	
}

void AShadowRunnerCharacter::OnTest_CloseDoor()
{
#if 0 // Test code should be removed later.
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFirstDoorActor::StaticClass(), FoundActors);

	AFirstDoorActor* firstDoor = Cast<AFirstDoorActor>(FoundActors[0]);

	if (firstDoor)
	{
		firstDoor->Closing = true;
		firstDoor->CanBeOpened = false;
	}
#endif
}

void AShadowRunnerCharacter::OnPuzzleCheat()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALightsOutPuzzleActor::StaticClass(), FoundActors);

	ALightsOutPuzzleActor* puzzle = Cast<ALightsOutPuzzleActor>(FoundActors[0]);

	if (puzzle)
	{
		for (int i = 0; i < puzzle->cubes.Num(); ++i)
		{
			puzzle->TurnOffCube(puzzle->cubes[i]);
		}
	}
}

void AShadowRunnerCharacter::OnHealthCheat()
{
	HealthBar->isHealthCheat = true;
}

void AShadowRunnerCharacter::OnHealthCheatOff()
{
	HealthBar->ResetHealth();

	HealthBar->isHealthCheat = false;
}

void AShadowRunnerCharacter::OnJumpUnlimit()
{
	isUnlimitJump = true;
}

void AShadowRunnerCharacter::OnJumpUnlimitOff()
{
	isUnlimitJump = false;
}

void AShadowRunnerCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}

	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnFire();
	}

	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void AShadowRunnerCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}

	TouchItem.bIsPressed = false;
}

void AShadowRunnerCharacter::Zoom(float value)
{
	/*if (value)
	{
		float temp = arm->TargetArmLength + (value * -10);

		if (temp < 310 && temp > 140)
		{
			arm->TargetArmLength = temp;
		}

		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("zoom")));

	}*/
}

void AShadowRunnerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector checkPoint = GetActorLocation();

	previousHealth = currentHealth;

	volumeControlEnemy = soundControl;

	pci = GetWorld()->GetParameterCollectionInstance(collection);

	if(hasSwapped == true)
	{
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindLambda([&]
		{
			if(pci)
			{
				bool found = pci->SetScalarParameterValue(FName("TPEffectAmount"), 0.0);
			}
			hasSwapped = false;
		});
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 1, false);
		hasSwapped = false;
	}

	if (Controller)
	{
		float length = inputVector.Size();

		if (length > 1.f)
		{
			inputVector /= length;
		}
		inputVector /= 2;
		AddMovementInput(GetActorForwardVector(), inputVector.Y);
		AddMovementInput(GetActorRightVector(), inputVector.X);

		inputVector = FVector(0.f);
	}

	if (HealthBar->isHealthCheat)
	{
		HealthBar->SetHealth(INFINITE);
		currentHealth = HealthBar->GetHealth();
	}
	else
	{
		currentHealth = HealthBar->GetHealth();
	}

	//AShadowRunnerHUD* shadowRunnerHUD = Cast<AShadowRunnerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%f, %f, %f"), GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z));
	if (HealthBar->GetHealth() <= 0)
	{
		if (PlayerDieSound != nullptr)
		{
			//UGameplayStatics::PlaySoundAtLocation(this, PlayerDieSound, GetActorLocation());
		}

		if (lifes > 0)
		{
			if (deathcheck == false)
			{
				deathcheck = true;

				playerDied = true;
				//Respawn();
			}
		}
	}

	if (HealthBar->GetHealth() < 50)
	{
		AShadowRunnerHUD* shadowRunnerHUD = Cast<AShadowRunnerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
		shadowRunnerHUD->UpdatePlayerLowHealth(true);
	}
	else
	{
		AShadowRunnerHUD* shadowRunnerHUD = Cast<AShadowRunnerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
		shadowRunnerHUD->UpdatePlayerLowHealth(false);
	}

	if (currentHealth < previousHealth)
	{
		AShadowRunnerHUD* shadowRunnerHUD = Cast<AShadowRunnerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
		shadowRunnerHUD->UpdatePlayerOnHitEffect(true, DeltaTime);

		if (PlayerOnHitSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, PlayerOnHitSound, GetActorLocation(), soundControl);
		}
	}
	else
	{
		AShadowRunnerHUD* shadowRunnerHUD = Cast<AShadowRunnerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
		shadowRunnerHUD->UpdatePlayerOnHitEffect(false, DeltaTime);
	}

	if (spawnedActorRef == nullptr)
	{
		shadowIsActive = false;
	}
	else
	{
		shadowIsActive = true;
	}

	// Update shadow swap cooldown timer.
	if (bShadowIsOnCoolDown)
	{
		shadowCloneCoolDownTimer -= DeltaTime;

		AShadowRunnerHUD* shadowRunnerHUD = Cast<AShadowRunnerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
		shadowRunnerHUD->UpdateShadowCooldown(shadowCloneCoolDownTimer, shadowCloneCoolDown);

		if (shadowCloneCoolDownTimer <= 0.0f)
		{
			bShadowIsOnCoolDown = false;
		}
	}

	// Update shadow spawn cooldown timer.
	if (bShadowSpawnIsOnCoolDown)
	{
		shadowSpawnCoolDownTimer -= DeltaTime;

		AShadowRunnerHUD* shadowRunnerHUD = Cast<AShadowRunnerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
		shadowRunnerHUD->UpdateShadowSpawnCooldown(shadowSpawnCoolDownTimer, shadowSpawnCoolDown);

		if (shadowSpawnCoolDownTimer <= 0.0f)
		{
			bShadowSpawnIsOnCoolDown = false;
		}
	}

	// Check interactable cude status.
	gameMode = Cast<AShadowRunnerGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (gameMode->counter >= 3 && isItemSpawned == false)
	{
		FRotator rotator = this->GetActorRotation();
		FVector facing = rotator.Vector();
		facing.Normalize();
		FVector spawnLocation = this->GetActorLocation() + facing * 250.0f;

		gameMode->CheckBlueCube(spawnLocation, this->GetActorRotation());
		//gameMode->counter = 0;
		if (gameMode->counter == 3)
		{
			isItemSpawned = true;
		}
	}
	if (gameMode->counter >= 6 && isDashItemSpawned == false)
	{
		FRotator rotator = this->GetActorRotation();
		FVector facing = rotator.Vector();
		facing.Normalize();
		FVector spawnLocation = this->GetActorLocation() + facing * 250.0f;

		gameMode->CheckBlueCube(spawnLocation, this->GetActorRotation());

		if (gameMode->counter == 6)
		{
			isDashItemSpawned = true;
		}
	}

	// Check if the shadow is in range and if its health is lower than 0.
	if (spawnedActorRef)
	{
		// Check if shadow is still in range.
		float distance = FVector::Distance(spawnedActorRef->GetActorLocation(), this->GetActorLocation());

		if (distance > 1500.0f)
		{
			RestoreShadow();
		}

		// Check shadow's health.
		AShadowCloneCharacter* shadow = Cast<AShadowCloneCharacter>(spawnedActorRef);

		if (shadow)
		{
			if (shadow->health <= 0.0f)
			{
				RestoreShadow();
			}
		}
	}

	if (WalkSoundCoolDown > 0)
	{
		WalkSoundCoolDown -= DeltaTime;
	}

	// Timer.
	surviveTime = this->GetGameTimeSinceCreation(); // in second.
	// Convert to hh:mm:ss.00
	surviveTimeHour = (int32)(surviveTime / (60 * 60));
	surviveTime -= surviveTimeHour * (60 * 60);
	surviveTimeMin = (int32)(surviveTime / 60);
	surviveTime -= surviveTimeMin * 60;
	surviveTimeSec = surviveTime;
	AShadowRunnerHUD* shadowRunnerHUD = Cast<AShadowRunnerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	shadowRunnerHUD->UpdateTimer(surviveTimeHour, surviveTimeMin, surviveTimeSec);
}

void AShadowRunnerCharacter::Respawn()
{
	//weapons[0]->GetDefaultObject<ABaseWeapon>()->clipAmmo = weapons[0]->GetDefaultObject<ABaseWeapon>()->maxClipAmmo;
	//weapons[1]->GetDefaultObject<ABaseWeapon>()->clipAmmo = weapons[1]->GetDefaultObject<ABaseWeapon>()->maxClipAmmo;	//sdfammo
	AShadowRunnerHUD* shadowRunnerHUD = Cast<AShadowRunnerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	//if (weaponIndex == 0)
	//{
	//	shadowRunnerHUD->UpdateAmmo(weapons[weaponIndex]->GetDefaultObject<ABaseWeapon>()->clipAmmo, INFINITE, 10, weaponIndex);
	//}
	//else
	//{
	//	shadowRunnerHUD->UpdateAmmo(INFINITE, weapons[0]->GetDefaultObject<ABaseWeapon>()->clipAmmo, 10, weaponIndex);
	//}

	//AShadowRunnerHUD* shadowRunnerHUD = Cast<AShadowRunnerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	//shadowRunnerHUD->UpdateLifeSystem(lifes);

	HealthBar->ResetHealth();
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("current health is %f"), HealthBar->GetHealth() ));
	SetActorLocation(originMesh);
	deathcheck = false;
	HealthBar->UpdateHUD();

	playerDied = false;

	if (RespawnSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, RespawnSound, GetActorLocation(), soundControl);
	}

	// Destory the shadow when the player died.
	if (spawnedActorRef)
	{
		spawnedActorRef->Destroy();
		spawnedActorRef = nullptr;
	}

	// Set the facing to the arrow component.
	this->GetController()->SetControlRotation(respawnFacing);

	Restart();
	
	//AShadowRunnerHUD* shadowRunnerHUD = Cast<AShadowRunnerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	shadowRunnerHUD->UpdatePlayerOnHitEffect(true, 2.f);
}

//Commenting this section out to be consistent with FPS BP template.
//This allows the user to turn without using the right virtual joystick

//void AShadowRunnerCharacter::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
//{
//	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
//	{
//		if (TouchItem.bIsPressed)
//		{
//			if (GetWorld() != nullptr)
//			{
//				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
//				if (ViewportClient != nullptr)
//				{
//					FVector MoveDelta = Location - TouchItem.Location;
//					FVector2D ScreenSize;
//					ViewportClient->GetViewportSize(ScreenSize);
//					FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
//					if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.X * BaseTurnRate;
//						AddControllerYawInput(Value);
//					}
//					if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.Y * BaseTurnRate;
//						AddControllerPitchInput(Value);
//					}
//					TouchItem.Location = Location;
//				}
//				TouchItem.Location = Location;
//			}
//		}
//	}
//}

void AShadowRunnerCharacter::Landed(const FHitResult& Hit)
{
	jumpCount = 0;

	if (JumpSound != nullptr && IsCrouch == false)
	{
		UGameplayStatics::PlaySoundAtLocation(this, JumpSound, GetActorLocation(), soundControl);
	}
}

void AShadowRunnerCharacter::MoveForward(float Value)
{
	if (playerDied)
	{
		return;
	}

	if (Value != 0.0f)
	{
		//if (isSprint)
		//{
		//	Value *= 2;
		//}
		// add movement in that direction
		inputVector.Y += Value;

		//AddMovementInput(GetActorForwardVector(), Value / 2);

		if (WalkSound != nullptr && WalkSoundCoolDown <= 0 && IsCrouch == false && GetCharacterMovement()->IsFalling() == false)
		{
			UGameplayStatics::PlaySoundAtLocation(this, WalkSound, GetActorLocation(), soundControl*1.5f);
			OnDistract();
			WalkSoundCoolDown = 0.35f;
		}
	}
}

void AShadowRunnerCharacter::MoveRight(float Value)
{
	if (playerDied)
	{
		return;
	}

	if (Value != 0.0f)
	{
		//if (isSprint)
		//{
		//	Value *= 2;
		//}
		// add movement in that direction

		inputVector.X += Value;
		//AddMovementInput(GetActorRightVector(), Value / 2);

		if (WalkSound != nullptr && WalkSoundCoolDown <= 0 && IsCrouch == false && GetCharacterMovement()->IsFalling() == false)
		{
			UGameplayStatics::PlaySoundAtLocation(this, WalkSound, GetActorLocation(), soundControl);
			OnDistract();
			WalkSoundCoolDown = 0.35f;
		}
	}
}

void AShadowRunnerCharacter::MoveFlying(float Value)
{
	if (playerDied)
	{
		return;
	}

	/*if (isFalling())
	{
		isFlying = true;
	}
	else
	{
		if (GetCharacterMovement()->IsFalling())
		{
			UE_LOG(LogTemp, Warning, TEXT("You are flying!"))
		}
		else
		{
			LaunchCharacter(FVector(0, 0, 400), false, false);
		}
	}*/

	if (Value > 0.0f && !isFlying)
	{
		isFlying = true;

		walkSpeed = 16000.f;
		ACharacter::LaunchCharacter(FVector(0, 0, 800.f), false, true);
		//LaunchCharacter(FVector(0, 0, 400), false, false);
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	}
	if(isFlying)
	{
		if (&UCharacterMovementComponent::FindFloor)
		{
			walkSpeed = 1600.f;
			isFlying = false;
		}
		else
		{
			GetCharacterMovement()->SetMovementMode(MOVE_Falling);
			isFlying = false;
		}
	}


}

bool AShadowRunnerCharacter::isFalling()
{
	//false is on the floor

	if (&UCharacterMovementComponent::FindFloor)
	{
		return false;
	}
	else
	{
		return true;
	}
}


void AShadowRunnerCharacter::TurnAtRate(float Rate)
{
	if (playerDied)
	{
		return;
	}
	else
	{

		// calculate delta for this frame from the rate information
		AddControllerYawInput(mouse_sensitivity *Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
}

void AShadowRunnerCharacter::LookUpAtRate(float Rate)
{
	if (playerDied)
	{
		return;
	}
	else
	{
		// calculate delta for this frame from the rate information
		AddControllerPitchInput(mouse_sensitivity * Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());

	}
}

void AShadowRunnerCharacter::CrouchStart()
{
	if (playerDied)
	{
		return;
	}
	IsCrouch = true;
	GetCharacterMovement()->MaxWalkSpeed = crouchSpeed;
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 24.f));
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("sprintYes")));
}

void AShadowRunnerCharacter::CrouchEnd()
{
	if (playerDied)
	{
		return;
	}
	IsCrouch = false;
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f));
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("sprintNo")));
}

APawn* AShadowRunnerCharacter::GetClone()
{
	return spawnedActorRef;
}

bool AShadowRunnerCharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AShadowRunnerCharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &AShadowRunnerCharacter::EndTouch);

		//Commenting this out to be more consistent with FPS BP template.
		//PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AShadowRunnerCharacter::TouchUpdate);
		return true;
	}

	return false;
}

void AShadowRunnerCharacter::Interact()
{
	FHitResult OutHit;
	FVector Start = GetFirstPersonCameraComponent()->GetComponentLocation();
	FVector End = Start + GetFirstPersonCameraComponent()->GetForwardVector() * 380;

	//DrawDebugLine(GetWorld(), Start, End, FColor::Purple, false, 1, 0, 1);

	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility))
	{
		AMaster_Interactable* Obj = Cast<AMaster_Interactable>(OutHit.Actor);

		if (Obj)
		{
			Obj->Interact();

			if (InteractSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, InteractSound, GetActorLocation(), soundControl);
			}
		}
	}

	// Interaction with doors.
	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility))
	{
		// Interact with first door.
		AFirstDoorActor* firstDoor = Cast<AFirstDoorActor>(OutHit.Actor);
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Actor hit: %d"), firstDoor));
		if (firstDoor)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("TEST: door dot %f"), firstDoor->dotProduct));
			FVector facing = End - Start;
			facing.Normalize();
			firstDoor->ToggleDoor(facing);
		}
	}

	// Interact with lights out puzzle.
	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility))
	{
		ALightsOutPuzzleActor* lightsOutPuzzle = Cast<ALightsOutPuzzleActor>(OutHit.Actor);

		if (lightsOutPuzzle)
		{
			UStaticMeshComponent* staticMeshComponent = Cast<UStaticMeshComponent>(OutHit.GetComponent());
			if (staticMeshComponent)
			{
				lightsOutPuzzle->ToggleCubes(staticMeshComponent);
			}
		}
	}
}

void AShadowRunnerCharacter::SpawnShadow()
{

	//FActorSpawnParameters spawnParams;

	// Spawn character.
	//spawnedActorRef = GetWorld()->SpawnActor<APawn>(shadowClone, location, rotation, spawnParams);

	// Set an AIController to possess the shadow.
	//spawnedActorRef->SpawnDefaultController(); // Don't need it if set the "Auto Possess AI" in blueprint to "Spawned".

	// Cast a ray, and spawn a shadow at the end of the ray.
	FRotator controlRotation = GetControlRotation();
	FVector directionVector = controlRotation.Vector();
	directionVector.Normalize();
	//FVector start = (FP_MuzzleLocation ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + controlRotation.RotateVector(GunOffset);
	const FVector start = FirstPersonCameraComponent->GetComponentLocation();
	FVector end = start + (directionVector * shadowSpawnRange);
	DrawDebugLine(GetWorld(), start, end, FColor::Green, false, 1, 0, 1);

	FHitResult hitResult;
	FCollisionQueryParams collisionParams;
	bool IsHit = GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECC_Visibility, collisionParams);

	if (IsHit)
	{
		// Spawn in front of the obsticle.
		//SpawnShadow(start + directionVector * (hitResult.Distance - 75), this->GetActorRotation());

		// Spawn character.
		FActorSpawnParameters spawnParams;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		FVector spawnLocation = start + directionVector * (hitResult.Distance - 75) + FVector(0.0f, 0.0f, 0.0f);
		spawnedActorRef = GetWorld()->SpawnActor<APawn>(shadowClone, spawnLocation, this->GetActorRotation(), spawnParams);
	}
	else
	{
		//SpawnShadow(end, this->GetActorRotation());

		// Spawn at the end of the ray.
		FActorSpawnParameters spawnParams;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		FVector spawnLocation = end + FVector(0.0f, 0.0f, 0.0f);
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Working 5")));
		spawnedActorRef = GetWorld()->SpawnActor<APawn>(shadowClone, spawnLocation, this->GetActorRotation(), spawnParams);
	}

	if (ShadowSpawnSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ShadowSpawnSound, GetActorLocation(), soundControl);
	}

	// Spend health to spawn shadow.
	TSubclassOf<UDamageType> damageType;
	shadowCloneHealthSpent = HealthBar->GetHealth() / 2;
	UGameplayStatics::ApplyDamage(this, shadowCloneHealthSpent, nullptr, this, damageType);

	/* avoid display playeronhitwidget while summoning shadow */


	if (HealthBar->isHealthCheat)
	{
		HealthBar->SetHealth(INFINITE);
		currentHealth = HealthBar->GetHealth();
	}
	else
	{
		currentHealth = HealthBar->GetHealth();
	}

	previousHealth = currentHealth;
}

void AShadowRunnerCharacter::RestoreShadow()
{
	// Cast a ray, from the shadow to the player.
	FRotator controlRotation = GetControlRotation();
	FVector directionVector = controlRotation.Vector();
	directionVector.Normalize();
	FVector start = spawnedActorRef->GetActorLocation();
	FVector end = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + controlRotation.RotateVector(GunOffset);
	DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 1, 0, 1);

	// Apply damage to enemies nearby.
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyCharacter::StaticClass(), FoundActors);

	for (int i = 0; i < FoundActors.Num(); ++i)
	{
		float distance = FVector::Distance(spawnedActorRef->GetActorLocation(), FoundActors[i]->GetActorLocation());

		if (distance <= shadowExplodeRadius)
		{
			//TSubclassOf<UDamageType> damageType;
			//UGameplayStatics::ApplyDamage(FoundActors[i], 100, nullptr, this, damageType);

			AEnemyCharacter* enemy = Cast<AEnemyCharacter>(FoundActors[i]);
			if (enemy)
			{
				enemy->TakeDamage(20.0f);
			}
		}
	}

	//if (weapons[weaponIndex]->GetDefaultObject<ABaseWeapon>()->clipAmmo == 0)
	{
		// Apply damage to enemies nearby.
		TArray<AActor*> FoundActors2;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAIMagic::StaticClass(), FoundActors2);

		for (int i = 0; i < FoundActors2.Num(); ++i)
		{
			float distance2 = FVector::Distance(spawnedActorRef->GetActorLocation(), FoundActors2[i]->GetActorLocation());

			if (distance2 <= shadowExplodeRadius)
			{
				//TSubclassOf<UDamageType> damageType;
				//UGameplayStatics::ApplyDamage(FoundActors[i], 100, nullptr, this, damageType);

				AAIMagic* magic_enemy = Cast<AAIMagic>(FoundActors2[i]);
				if (magic_enemy)
				{
					magic_enemy->health -= 20.f;
				}
			}
		}

		// Emit explode particles.
		if (shadowExplodeParticles)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), shadowExplodeParticles, spawnedActorRef->GetActorLocation());
		}

		// Explosion sound effect.
		if (ShadowExplodeSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, ShadowExplodeSound, spawnedActorRef->GetActorLocation(), soundControl);
		}
	}

	// Restore health from shadow.
	TSubclassOf<UDamageType> damageType;
	UGameplayStatics::ApplyDamage(this, -shadowCloneHealthSpent, nullptr, this, damageType);

	// Destroy the shadow.
	spawnedActorRef->Destroy();
	spawnedActorRef = nullptr;

	if (ShadowRestoreSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ShadowRestoreSound, GetActorLocation(), soundControl);
	}
}

void AShadowRunnerCharacter::StartJump()
{
	if (playerDied)
	{
		return;
	}
	bPressedJump = true;
}
void AShadowRunnerCharacter::StopJump()
{
	bPressedJump = false;
}

void AShadowRunnerCharacter::Doublejump()
{
	if (playerDied)
	{
		return;
	}
	if (isUnlimitJump)
	{
		ACharacter::LaunchCharacter(FVector(0, 0, jumpHeight), false, true);
	}
	else
	{
		if (jumpCount < jumpNumber)
		{
			ACharacter::LaunchCharacter(FVector(0, 0, jumpHeight), false, true);
			jumpCount++;
		}
	}

	UCharacterMovementComponent* moveComp = Cast<UCharacterMovementComponent>(this->GetMovementComponent());
	if (moveComp)
	{
		moveComp->AirControl = 0.5f;
	}
}

void AShadowRunnerCharacter::Dash()
{
	if (playerDied)
	{
		return;
	}
	if (bDashGained)
	{
		if (canDash)
		{
			GetCharacterMovement()->BrakingFrictionFactor = 0.f;

			// Modified by Alex.
			//LaunchCharacter(FVector(FirstPersonCameraComponent->GetForwardVector().X, FirstPersonCameraComponent->GetForwardVector().Y, 0).GetSafeNormal() * dashDistacne, true, true);
			UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(this->GetCharacterMovement());
			if (MovementComponent)
			{
				FVector direction = MovementComponent->Velocity;
				direction.Normalize();
				LaunchCharacter(direction * dashSpeed, true, true);
			}

			canDash = false;
			GetWorldTimerManager().SetTimer(unusedHandle, this, &AShadowRunnerCharacter::stopDash, dashStop, false);

			if (DashSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, DashSound, GetActorLocation(), soundControl);
			}
		}
	}
}

void AShadowRunnerCharacter::stopDash()
{
	// Modified by Alex.
	GetCharacterMovement()->StopMovementImmediately();
	//GetCharacterMovement()->StopActiveMovement();
	GetWorldTimerManager().SetTimer(unusedHandle, this, &AShadowRunnerCharacter::resetDash, dashColdown, false);
	GetCharacterMovement()->BrakingFrictionFactor = 2.f;
}

void AShadowRunnerCharacter::resetDash()
{
	canDash = true;
}

void AShadowRunnerCharacter::OnDistract()
{
	if (DistractionSound)
	{
		FVector const PlayerLocation = GetActorLocation();
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), DistractionSound, PlayerLocation, soundControl);
		UAISense_Hearing::ReportNoiseEvent(GetWorld(), PlayerLocation, 1.0f, this, 0.0f, tags::NoiseTag);
	}
}

void AShadowRunnerCharacter::SetUpStimulus()
{
	stimulus = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
	stimulus->RegisterForSense(TSubclassOf<UAISense_Sight>());
	stimulus->RegisterWithPerceptionSystem();
}

void AShadowRunnerCharacter::MuzzleParticleTimerCallback()
{
	if (muzzleParticleRef)
	{
		muzzleParticleRef->DestroyComponent();
		muzzleParticleRef = nullptr;
	}
}