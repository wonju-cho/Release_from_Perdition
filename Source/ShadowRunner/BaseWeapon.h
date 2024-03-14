// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	E_AssaultRifle UMETA(Displayname = "ASSAULT_RIFLE"),
	E_Shotgun UMETA(DisplayName = "SHOTGUN")
};

UCLASS()
class SHADOWRUNNER_API ABaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseWeapon();

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	//	int maxTotalAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		int addClipAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		int maxClipAmmo;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		int totalAmmo;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		int clipAmmo;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	//	float reloadTime;

	//type of weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		EWeaponType weaponType;

	void changeType(int index);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
