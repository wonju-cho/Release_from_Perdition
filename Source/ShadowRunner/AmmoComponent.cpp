// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoComponent.h"

// Sets default values for this component's properties
UAmmoComponent::UAmmoComponent()
	: ammo(10)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAmmoComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAmmoComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	
}

int UAmmoComponent::GetAmmoCount()
{
	return ammo;
}

void UAmmoComponent::SetAmmoCount(int curAmmo)
{
	curAmmo = ammo;
}

void UAmmoComponent::Fire()
{
	//ammo - 1;
}

void UAmmoComponent::Empty()
{
	return;
}

