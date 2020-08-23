// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerCharacter.h"
#include "Spawner.h"
#include "ObjectiveButton.h"
#include "Components/BoxComponent.h"
#include "Puzzle.generated.h"

UCLASS()
class IMPULSE_API APuzzle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APuzzle();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Bounds")
	class UBoxComponent* PuzzleStart;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Bounds")
	class UBoxComponent* PuzzleEnd;

private:

	UFUNCTION(BlueprintCallable, Category = "Objectives", meta = (AllowPrivateAccess = "true"))
	virtual UPARAM(DisplayName="Level Complete") bool CheckObjectiveButtons();  //uparam allows custom function output title

	UFUNCTION()
	void OnPuzzleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPuzzleEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void ToggleSpawners();

	bool PuzzleActive = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Objectives", meta = (AllowPrivateAccess = "true"))
	bool LevelComplete = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Objectives", meta = (AllowPrivateAccess = "true"))
	TArray<AObjectiveButton*> ObjectiveButtons;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawners", meta = (AllowPrivateAccess = "true"))
	bool ToggleSpawnersWithPuzzleState = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawners", meta = (AllowPrivateAccess = "true"))
	TArray<ASpawner*> Spawners;

	


};
