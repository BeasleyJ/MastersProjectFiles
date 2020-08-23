// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"
#include "ImpulseObject.generated.h"

UCLASS()
class IMPULSE_API AImpulseObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AImpulseObject();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
	class UStaticMeshComponent* ObjectMesh;

	void SetInGravityField(const bool IsInGravityField);
	
protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "State")
	bool InGravityField = false;
};
