// Fill out your copyright notice in the Description page of Project Settings.


#include "Despawner.h"

// Sets default values
ADespawner::ADespawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DespawnVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("DespawnVolume"));
	RootComponent = DespawnVolume;
}

// Called when the game starts or when spawned
void ADespawner::BeginPlay()
{
	Super::BeginPlay();

	DespawnVolume->OnComponentBeginOverlap.AddDynamic(this, &ADespawner::OnBoxBeginOverlap); //allows definition of overlap events in c++
	
}

void ADespawner::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//if valid object, store as int to trigger function in tick
	if (AImpulseObject* ImpulseObject = Cast<AImpulseObject>(OtherActor))
	{
		OverlappingObjects += 1;
	}
}

void ADespawner::DespawnObjects()
{
	TArray<AActor*> Actors;
	
	DespawnVolume->GetOverlappingActors(Actors);

	for (AActor* Actor : Actors)
	{
		//destroy all valid objects
		if (AImpulseObject* Object = Cast<AImpulseObject>(Actor))
		{
			Object->Destroy();

			if (OverlappingObjects > 0)
			{
				OverlappingObjects -= 1;
			}
		}
	}
}


// Called every frame
void ADespawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (OverlappingObjects > 0)
	{
		DespawnObjects();
	}
}