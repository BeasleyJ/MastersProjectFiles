// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectiveButton.h"

// Sets default values
AObjectiveButton::AObjectiveButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Holder = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Holder"));
	RootComponent = Holder;
	
	//set as physics object and disable gravity to prevent pressing without object
	Button = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Button"));
	Button->SetupAttachment(RootComponent);
	Button->SetMobility(EComponentMobility::Movable);
	Button->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
	Button->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Button->SetSimulatePhysics(true);
	Button->SetEnableGravity(false);

	ButtonCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ButtonCollision"));
	ButtonCollision->SetupAttachment(RootComponent);

	ButtonConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("ButtonConstraint"));
	ButtonConstraint->SetupAttachment(RootComponent);
	
}

bool AObjectiveButton::GetIsPressed()
{
	return IsPressed;
}

// Called when the game starts or when spawned
void AObjectiveButton::BeginPlay()
{
	Super::BeginPlay();

	ButtonConstraint->SetConstrainedComponents(Holder, NAME_None, Button, NAME_None); //attach constraint to the two meshes

	//allows definition of overlap events in c++
	ButtonCollision->OnComponentBeginOverlap.AddDynamic(this, &AObjectiveButton::OnBoxBeginOverlap);
	ButtonCollision->OnComponentEndOverlap.AddDynamic(this, &AObjectiveButton::OnBoxEndOverlap);
	
}

void AObjectiveButton::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AImpulseObject* ImpulseObject = Cast<AImpulseObject>(OtherActor))
	{
		if (OverlappingObjectsCount == 0) //if no objects already overlapping, change state
		{
			IsPressed = true;
		}

		OverlappingObjectsCount += 1;
	}
}

void AObjectiveButton::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AImpulseObject* ImpulseObject = Cast<AImpulseObject>(OtherActor))
	{
		if (OverlappingObjectsCount == 1) //if last object to end overlap, change state
		{
			IsPressed = false;
		}

		OverlappingObjectsCount -= 1;
	}
}

// Called every frame
void AObjectiveButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

