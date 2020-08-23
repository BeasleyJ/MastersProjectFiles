// Fill out your copyright notice in the Description page of Project Settings.


#include "Puzzle.h"

// Sets default values
APuzzle::APuzzle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	PuzzleStart = CreateDefaultSubobject<UBoxComponent>(TEXT("PuzzleStart"));
	PuzzleStart->SetupAttachment(RootComponent);

	PuzzleEnd = CreateDefaultSubobject<UBoxComponent>(TEXT("PuzzleEnd"));
	PuzzleEnd->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void APuzzle::BeginPlay()
{
	Super::BeginPlay();

	//allows definition of overlap events in c++
	PuzzleStart->OnComponentBeginOverlap.AddDynamic(this, &APuzzle::OnPuzzleBeginOverlap);
	PuzzleEnd->OnComponentBeginOverlap.AddDynamic(this, &APuzzle::OnPuzzleEndOverlap);
	
}

bool APuzzle::CheckObjectiveButtons()
{
	if (ObjectiveButtons.Num() > 0)
	{
		int ButtonsPressed = 0;

		for (AObjectiveButton* Button : ObjectiveButtons)
		{
			bool ButtonPressed = Button->GetIsPressed();

			if (ButtonPressed)
			{
				ButtonsPressed += 1;
			}
		}

		if (ObjectiveButtons.Num() == ButtonsPressed) //if all buttons are pressed, set level complete
		{
			if (LevelComplete == false)
			{
				LevelComplete = true;
			}
		}

		//if all buttons are not pressed
		else
		{
			if (LevelComplete)
			{
				LevelComplete = false;
			}
		}

		return this->LevelComplete;
	}

	else
	{
		return this->LevelComplete;
	}

	
}

void APuzzle::OnPuzzleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//overlap triggers the puzzle to start
	if (APlayerCharacter* Character = Cast<APlayerCharacter>(OtherActor))
	{
		if (PuzzleActive == false)
		{
			PuzzleActive = true;

			if (ToggleSpawnersWithPuzzleState) //can set spawners to all trigger on puzzle start
			{
				ToggleSpawners();
			}
		}
	}
}

void APuzzle::OnPuzzleEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//overlap triggers the puzzle to end
	if (APlayerCharacter* Character = Cast<APlayerCharacter>(OtherActor))
	{
		if (PuzzleActive)
		{
			PuzzleActive = false;

			if (ToggleSpawnersWithPuzzleState) //can set spawners to all trigger on puzzle end
			{
				ToggleSpawners();
			}
		}
	}
}

void APuzzle::ToggleSpawners()
{
	if (PuzzleActive)
	{	
		//set all spawners to active
		for (ASpawner* Spawner : Spawners)
		{
			Spawner->SetIsActive(true);
		}
	}

	else
	{
		//set all spawners to inactive
		for (ASpawner* Spawner : Spawners) 
		{
			Spawner->SetIsActive(false);
		}
	}
}

// Called every frame
void APuzzle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

