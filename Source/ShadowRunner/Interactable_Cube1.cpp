// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable_Cube1.h"
#include "Kismet/GameplayStatics.h"

AInteractable_Cube1::AInteractable_Cube1()
{
	//mat = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, TEXT("/Material/Game/Assets/Materials/BlueCube")));
	//mat = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, TEXT("/Materials/BlueCube")));
	//mat = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, TEXT("/Game/ShadowRunner/Assets/Materials/BlueCube")));
	static ConstructorHelpers::FObjectFinder <UMaterialInterface>Material_Blue(TEXT("Material'/Game/Assets/Materials/BlueCube'"));
	mat = Material_Blue.Object;
	//cube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("cube"));
	State = false;
	
	// Initialize spawn location.
	spawnLocation = FVector(0.0f, 0.0f, 0.0f);
}

void AInteractable_Cube1::Interact()
{
	if (mat)
	{
		//UMaterialInstanceDynamic* dynamicMat = UMaterialInstanceDynamic::Create(mat, this);
		//dynamicMat->SetVectorParameterValue("TestColour", FLinearColor(1, 0, 0));
		//cube->SetMaterial(0, mat);
		this->Mesh->SetMaterial(0, mat);
		
		if (!State)
		{
			gameMode = Cast<AShadowRunnerGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
			gameMode->counter++;
		}
		State = true;
	}

	//cube->SetMaterial(0, Material_Blue.Object);
}