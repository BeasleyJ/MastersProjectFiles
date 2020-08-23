// Fill out your copyright notice in the Description page of Project Settings.


#include "ResetField.h"

// Sets default values
AResetField::AResetField()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FieldMesh"));
	RootComponent = FieldMesh;
}

// Called when the game starts or when spawned
void AResetField::BeginPlay()
{
	Super::BeginPlay();

	//allows overlap events to be defined in C++
	FieldMesh->OnComponentBeginOverlap.AddDynamic(this, &AResetField::OnFieldBeginOverlap);
	
}

void AResetField::OnFieldBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//if player walks through field, reset values stored in device
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		ResetDevice(Player->GetDevice());
	}
}

//resets all current stored values, can be extended in Blueprints for new device rules
void AResetField::ResetDevice_Implementation(AImpulseDevice* Device)
{
	Device->SetStoredLinearVelocity(FVector(0.0f, 0.0f, 0.0f));
	Device->SetStoredAngularVelocity(FVector(0.0f, 0.0f, 0.0f));
}

