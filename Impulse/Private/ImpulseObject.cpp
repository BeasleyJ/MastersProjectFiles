// Fill out your copyright notice in the Description page of Project Settings.


#include "ImpulseObject.h"

// Sets default values
AImpulseObject::AImpulseObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObjectMesh"));
	RootComponent = ObjectMesh;
	ObjectMesh->SetSimulatePhysics(true);

}

void AImpulseObject::SetInGravityField(const bool IsInGravityField)
{
	InGravityField = IsInGravityField;
}


