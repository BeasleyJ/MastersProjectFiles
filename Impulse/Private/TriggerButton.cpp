// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerButton.h"

// Sets default values
ATriggerButton::ATriggerButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StandMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StandMesh"));
	RootComponent = StandMesh;

	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMesh"));
	ButtonMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ATriggerButton::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATriggerButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<AGravityField*> ATriggerButton::GetGravityFields()
{
	return TArray<AGravityField*>(Fields);
}

