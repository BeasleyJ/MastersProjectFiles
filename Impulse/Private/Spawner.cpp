// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"

// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	OriginMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OriginMesh"));
	OriginMesh->SetupAttachment(RootComponent);

	OriginSpawn = CreateDefaultSubobject<USphereComponent>(TEXT("OriginSpawn"));
	OriginSpawn->SetupAttachment(RootComponent);

	TargetMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TargetMesh"));
	TargetMesh->SetupAttachment(RootComponent);

	TargetDespawn = CreateDefaultSubobject<UBoxComponent>(TEXT("TargetDespawn"));
	TargetDespawn->SetupAttachment(RootComponent);

}

void ASpawner::SetIsActive(bool IsSpawnerActive)
{
	IsActive = IsSpawnerActive;
}



// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();

	TargetDespawn->OnComponentBeginOverlap.AddDynamic(this, &ASpawner::OnBoxBeginOverlap); //allows definition of overlap events in c++
	
}

AImpulseObject* ASpawner::SpawnImpulseObject()
{
	if (ImpulseObjectToSpawn) //if valid object set to spawn
	{
		FVector SpawnLoc = OriginSpawn->GetComponentLocation();
		FRotator SpawnRot = { 0,0,0 };

		AImpulseObject* SpawnedObject = GetWorld()->SpawnActor<AImpulseObject>(ImpulseObjectToSpawn, SpawnLoc, SpawnRot);

		return SpawnedObject;
	}

	else
	{
		return nullptr;
	}
	
}

//on overlap with the despawn volume, and objects set to be despawned, delete objects
void ASpawner::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AImpulseObject* ImpulseObject = Cast<AImpulseObject>(OtherActor))
	{
		if (DespawnOnTargetReached)
		{
			Object = ImpulseObject;

			GetWorldTimerManager().SetTimer(DelayHandle, this, &ASpawner::DespawnObject, DespawnDelay, false); //runs function once to despawn object, with a delay defined by DespawnDelay
		}
	}
}

void ASpawner::DespawnObject()
{
	GetWorld()->DestroyActor(Object);

	GetWorldTimerManager().ClearTimer(DelayHandle);
}


