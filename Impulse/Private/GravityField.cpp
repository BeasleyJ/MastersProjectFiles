// Fill out your copyright notice in the Description page of Project Settings.


#include "GravityField.h"

// Sets default values
AGravityField::AGravityField()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OriginMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OriginMesh"));
	RootComponent = OriginMesh;

	OriginBeamLocation = CreateDefaultSubobject<USphereComponent>(TEXT("OriginBeamLocation"));
	OriginBeamLocation->SetupAttachment(RootComponent);

	FieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FieldMesh"));
	FieldMesh->SetupAttachment(OriginBeamLocation);

	TargetMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TargetMesh"));
	TargetMesh->SetupAttachment(RootComponent);

	TargetBeamLocation = CreateDefaultSubobject<USphereComponent>(TEXT("TargetBeamLocation"));
	TargetBeamLocation->SetupAttachment(TargetMesh);
}

// Called when the game starts or when spawned
void AGravityField::BeginPlay()
{
	Super::BeginPlay();

	//allows overlap events to be defined in C++
	FieldMesh->OnComponentBeginOverlap.AddDynamic(this, &AGravityField::OnFieldBeginOverlap);
	FieldMesh->OnComponentEndOverlap.AddDynamic(this, &AGravityField::OnFieldEndOverlap);
}

void AGravityField::InitialiseBeam()
{
	BeamLength = (TargetMesh->GetRelativeLocation().X + TargetBeamLocation->GetRelativeLocation().X) / BeamSectionLength; //distance to other side divided by length of mesh
	PanSpeedMatchModifier *= GetActorScale3D().X;

	FieldStart = OriginBeamLocation->GetComponentLocation();
	FieldEnd = TargetBeamLocation->GetComponentLocation();
	FieldImpulse = FieldMesh->GetForwardVector() * (PanSpeed * PanSpeedMatchModifier); 

	//Initialises field to Blueprint FieldOn value
	if (FieldOn)
	{
		SwitchFieldOn();
	}

	else
	{
		SwitchFieldOff();
	}
}

void AGravityField::SwitchFieldOn()
{
	FVector CurrentScale = FieldMesh->GetRelativeScale3D();
	FieldMesh->SetRelativeScale3D(FVector(BeamLength, CurrentScale.Y, CurrentScale.Z)); //scale beam to reach other side

	if (FieldOn == false)
	{
		FieldOn = true;
	}
}

void AGravityField::SwitchFieldOff()
{
	FVector CurrentScale = FieldMesh->GetRelativeScale3D();
	FieldMesh->SetRelativeScale3D(FVector(0.001f, CurrentScale.Y, CurrentScale.Z)); //remove beam

	if (FieldOn)
	{
		FieldOn = false;
	}
}

void AGravityField::ToggleField()
{
	if (FieldOn)
	{
		SwitchFieldOff();
	}

	else
	{
		SwitchFieldOn();
	}
}

//visual elements to this function are defined in Blueprints
void AGravityField::ReverseField_Implementation()
{
	FieldImpulse *= -1;
}

void AGravityField::OnFieldBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//if valid object, store in number of overlapping objects to determine if forces need to be applied
	if (AImpulseObject* Object = Cast<AImpulseObject>(OtherActor))
	{
		Object->SetInGravityField(true);
		OverlappingObjects += 1;
		Object->ObjectMesh->SetEnableGravity(false);
	}
}

void AGravityField::OnFieldEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//once out of beam, turn gravity back on and stop effects of the beam on the object
	if (AImpulseObject* Object = Cast<AImpulseObject>(OtherActor))
	{
		Object->SetInGravityField(false);
		OverlappingObjects -= 1;
		Object->ObjectMesh->SetEnableGravity(true);
	}
}

void AGravityField::ApplyGravityField()
{
	TArray<AActor*> Actors;

	FieldMesh->GetOverlappingActors(Actors);

	for (AActor* Actor : Actors)
	{
		if (AImpulseObject* Object = Cast<AImpulseObject>(Actor))
		{
			FVector CurrentVelocity = Object->ObjectMesh->GetPhysicsLinearVelocity();

			Object->ObjectMesh->SetPhysicsLinearVelocity(CurrentVelocity / 1.05f); //general decceleration of object

			FVector VectorToStart = (FieldStart - Object->ObjectMesh->CalcBounds(Object->GetActorTransform()).Origin); //vector from object to beam origin
			FVector VectorToEnd = (FieldEnd - Object->ObjectMesh->CalcBounds(Object->GetActorTransform()).Origin); //vector from origin to beam end
			FVector PullVector = VectorToStart.GetSafeNormal() + VectorToEnd.GetSafeNormal(); //should essentially cancel in beam direction to leave force towards beam centre line

			Object->ObjectMesh->AddImpulse(PullVector * FieldStrength, NAME_None, true);
			Object->ObjectMesh->AddImpulse(FieldImpulse, NAME_None, true);
		}
	}
}

// Called every frame
void AGravityField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (OverlappingObjects > 0)
	{
		ApplyGravityField();
	}
}

