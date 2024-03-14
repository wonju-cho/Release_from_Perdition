// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "BaseWeapon.h"
#include "BaseAmmo.h"
#include "ShadowRunnerGameMode.h"
#include "Sound/SoundBase.h"
#include "Interactable_Cube1.h"
#include "EnemyCharacter.h"
#include "DestructiblePot.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"

#include "ShadowRunnerCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UMotionControllerComponent;
class UAnimMontage;
class USoundBase;
class UAmmoComponent;
class UHealthComponent;
class ABaseWeapon;
class AEnemyCharacter;

UCLASS(config = Game)
class AShadowRunnerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerEvent)
		bool playerDied;

private:



		UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UAmmoComponent* ammo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UHealthComponent* HealthBar;

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* Mesh1P;

	/** Gun mesh: 1st person view (seen only by self) */
	//UPROPERTY(VisibleDefaultsOnly, Category = Mesh)


	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		USceneComponent* FP_MuzzleLocation;

	/** Gun mesh: VR view (attached to the VR controller directly, no arm, just the actual gun) */
	//UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	//	USkeletalMeshComponent* VR_Gun;

	/** Location on VR gun mesh where projectiles should spawn. */
	//UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	//	USceneComponent* VR_MuzzleLocation;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* FirstPersonCameraComponent;

	/** Motion controller (right hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UMotionControllerComponent* R_MotionController;

	/** Motion controller (left hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UMotionControllerComponent* L_MotionController;

	// Shadow clone variables.
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> shadowClone;

	APawn* spawnedActorRef; // Clone reference for destroying the clone.

	// Game mode var
	UPROPERTY()
		AShadowRunnerGameMode* gameMode;

	// blue cube var
	UPROPERTY()
		AInteractable_Cube1* blueCube;

	// MPC reference for the teleport post process effect
	UMaterialParameterCollection* collection;

	bool hasSwapped;
	UMaterialParameterCollectionInstance* pci;

public:
	AShadowRunnerCharacter();
	bool deathcheck = false;
	FVector originMesh;
	int curAmmo;
	bool play;
	bool isItemSpawned;
	bool isDashItemSpawned;
	float WalkSoundCoolDown;
	float pistolDamage;

	float mouse_sensitivity;




	UFUNCTION(BlueprintCallable)
		float GetMouseSensitivity();

	UFUNCTION(BlueprintCallable)
		void SetMouseSensitivity(float s);


	UPROPERTY(BlueprintReadWrite, Category = Mesh)
		USkeletalMeshComponent* FP_Gun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		USoundBase* GethurtSound;
protected:
	virtual void BeginPlay();
	virtual void Tick(float deltaTime);

public:
	UFUNCTION(BlueprintCallable)
		void Respawn();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector GunOffset;

	/** Projectile class to spawn */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
		TSubclassOf<class AShadowRunnerProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		USoundBase* FireSound;

	/** Sound to play each time we spawn shadow */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		USoundBase* ShadowSpawnSound;

	/** Sound to play each time we restore shadow */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		USoundBase* ShadowRestoreSound;

	/** Sound to play when the clone explodes */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		USoundBase* ShadowExplodeSound;

	/** Sound to play each time we swap position with shadow */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		USoundBase* ShadowSwapSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		USoundBase* WalkSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		USoundBase* DashSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		USoundBase* JumpSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		USoundBase* InteractSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		USoundBase* SwitchWeaponSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		USoundBase* RespawnSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		USoundBase* PlayerDieSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		USoundBase* PlayerOnHitSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		UAnimMontage* FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UParticleSystem* MuzzleParticles;

	class UParticleSystemComponent* muzzleParticleRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UParticleSystem* ImpactParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float TimeBetweenShots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float dashSpeed;

	/** Whether to use motion controller location for aiming. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		uint8 bUsingMotionControllers : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		TSubclassOf<ABaseWeapon> weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		TArray<TSubclassOf<ABaseWeapon>> weapons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		int weaponIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		int assaultRifleAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		int shotgunAmmo;

	// Shadow clone variables.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float shadowCloneCoolDown; // For swaping.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float shadowCloneCoolDownTimer; // For swaping.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float shadowSpawnCoolDown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float shadowSpawnCoolDownTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float shadowSpawnRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		int lifes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		bool shadowIsActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		bool bShadowIsOnCoolDown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		bool bShadowSpawnIsOnCoolDown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float muzzleParticleDisplayTime;

	// Health spent on spawning shadow.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float shadowCloneHealthSpent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		bool bDashGained;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = JumpCheat)
		bool isUnlimitJump = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		int jumpNumber;

	// Timer variables.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Timer)
		float surviveTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Timer)
		float surviveTimeSec;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Timer)
		int32 surviveTimeMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Timer)
		int32 surviveTimeHour;

	FRotator respawnFacing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Timer)
		float soundControl = 1.5f;

protected:

	//Reload current weapon
	void manualRelaod();

	//Reload current weapon
	void ReloadWeapon(EWeaponType _weaponType);

	//Calculate the ammo in the weapon's clip
	int CalculateAmmo(int _ammoAmount);

	void SwitchToNextPrimaryWeapon();

	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
		void SwitchWeaponMesh(int index);

	/** Fires a projectile. */
	void OnFire();

	void startFire();
	void stopFire();

	FTimerHandle TimerHandle_HandleRefire;
	FTimerHandle muzzleParticleTimerHandle;

	/** Resets HMD orientation and position in VR. */
	void OnResetVR();

	// Spawn a shadow.
	void OnShadowSpawn();

	// Swap position with the shadow.
	void OnShadowSwap();

	// Test key for door closing.
	void OnTest_CloseDoor();

	// Test key for door closing.
	void OnPuzzleCheat();

	// HealthCheat.
	void OnHealthCheat();
	void OnHealthCheatOff();

	// jumpCheat.
	void OnJumpUnlimit();
	void OnJumpUnlimitOff();
	
	FVector inputVector;
	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/** Handles moving flying */
	void MoveFlying(float Val);

	bool isFalling();

	bool isFlying = false;
	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	////
	void CrouchStart();
	void CrouchEnd();

	//	void ReloadWeapon();
	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
		void TriggerOutOfAmmoPopUp();

	//UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	//	void SwitchWeaponMesh(int index);

	//UFUNCTION(BlueprintCallable)
	//	void AddAmmo(EAmmoType _ammoType, int _ammoAmount);
	bool checkPointcheck;
	bool IsCrouch = false;
	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	struct TouchData
	{
		TouchData() { bIsPressed = false; Location = FVector::ZeroVector; }
		bool bIsPressed;
		ETouchIndex::Type FingerIndex;
		FVector Location;
		bool bMoved;
	};
	void BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location);

	TouchData	TouchItem;

	virtual void Landed(const FHitResult& Hit) override;

	UFUNCTION()
		void SpawnShadow();

	UFUNCTION()
		void RestoreShadow();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	/*
	 * Configures input for touchscreen devices if there is a valid touch interface for doing so
	 *
	 * @param	InputComponent	The input component pointer to bind controls to
	 * @returns true if touch controls were enabled.
	 */
	bool EnableTouchscreenMovement(UInputComponent* InputComponent);

	void Interact();

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	void Zoom(float value);

	UPROPERTY()
		UCameraComponent* cam;

	APawn* GetClone();

	UPROPERTY()
		USpringArmComponent* arm;

	UFUNCTION()
		void StartJump();
	//clears jump flag when key is released
	UFUNCTION()
		void StopJump();

	UFUNCTION()
		void Doublejump();

	UFUNCTION()
		void Dash();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float jumpHeight;

	UPROPERTY()
		float jumpCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float walkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float crouchSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float dashCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float dashDistacne;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float dashColdown;

	UPROPERTY()
		bool canDash;

	UPROPERTY()
		float dashStop;

	UPROPERTY()
		FTimerHandle unusedHandle;

	UFUNCTION()
		void stopDash();

	UFUNCTION()
		void resetDash();

	UPROPERTY()
		float currentHealth;

	UPROPERTY()
		float previousHealth;

	// Shadow explode.
	UPROPERTY()
		float shadowExplodeRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShadowClone)
		class UParticleSystem* shadowExplodeParticles;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		USoundBase* DistractionSound;

	class UAIPerceptionStimuliSourceComponent* stimulus;

	UFUNCTION()
		void SetUpStimulus();

	void OnDistract();

	void MuzzleParticleTimerCallback();
};