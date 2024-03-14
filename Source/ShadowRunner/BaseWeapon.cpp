// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	addClipAmmo = 10;
	//maxTotalAmmo = 100;
	maxClipAmmo = 10;
	//totalAmmo = 60;
	clipAmmo = 10;
	//reloadTime = 1.0f;

	weaponType = EWeaponType::E_AssaultRifle;
}

void ABaseWeapon::changeType(int index)
{
	if(index == 0)
	{
		weaponType = EWeaponType::E_AssaultRifle;
	}
	else if(index == 1)
	{
		weaponType = EWeaponType::E_Shotgun;
	}
}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

