// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Spawner.h"
#include "Conveyor.h"
#include "SpawnerConveyor.generated.h"

/**
 * 
 */
UCLASS()
class IMPULSE_API ASpawnerConveyor : public ASpawner
{
	GENERATED_BODY()

private:

	UFUNCTION(BlueprintCallable, Category = "Spawn", meta = (AllowPrivateAccess = "true"))
	void SetInitialVelocityAndRotation(AImpulseObject* SpawnedObject);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn", meta = (AllowPrivateAccess = "true"))
	bool AddVelocity = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn", meta = (EditCondition = "AddVelocity"), meta = (AllowPrivateAccess = "true")) //only editable if AddVelocity is true
	float IntialVelocityMagnitude = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn", meta = (AllowPrivateAccess = "true"))
	AConveyor* ConveyorRef;
	
};
