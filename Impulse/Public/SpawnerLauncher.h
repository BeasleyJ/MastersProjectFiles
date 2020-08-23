// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Spawner.h"
#include "Kismet/KismetMathLibrary.h"
#include "SpawnerLauncher.generated.h"

/**
 * 
 */
UCLASS()
class IMPULSE_API ASpawnerLauncher : public ASpawner
{
	GENERATED_BODY()

private:

	UFUNCTION(BlueprintCallable, Category = "Spawn", meta = (AllowPrivateAccess = "true"))
	void FireObject(AImpulseObject* SpawnedObject);

	void FireAtTarget(AImpulseObject* ImpulseObject);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn", meta = (AllowPrivateAccess = "true"))
	bool UseCustomVelocity = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn", meta = (EditCondition = "UseCustomVelocity"), meta = (AllowPrivateAccess = "true")) //only editable if UseCustomVelocity is true
	float CustomVelocityMagnitude = 0.0f;
	
};
