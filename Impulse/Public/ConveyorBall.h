// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "ConveyorBall.generated.h"

UCLASS()
class IMPULSE_API AConveyorBall : public AActor
{
	GENERATED_BODY()

	//virtual void OnConstruction(const FTransform& Transform) override;
	
public:	
	// Sets default values for this actor's properties
	AConveyorBall();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Spline")
	class USplineComponent* ConveyorPath;

//protected rather than priavte to facilitate future children of this class
protected:

	UFUNCTION(BlueprintCallable, Category = "Generation")
	void SpawnConveyorSection(USplineMeshComponent* SplineComponent, int LoopIndex);

	UFUNCTION(BlueprintCallable, Category = "Generation")
	void SpawnLegs(USplineMeshComponent* SplineComponent, int LoopIndex);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMesh* ConveyorMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMesh* LegsMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MeshDimensions")
	float ConveyorSectionLength = 12.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MeshDimensions")
	float LegSectionLength = 12.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MeshDimensions")
	float LegLengthZ = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Conveyor")
	float ConveyorSectionSpacing = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Leg")
	bool AddLegs = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Leg", meta = (EditCondition = "AddLegs")) //only editable if AddLegs is true
	float LegSpacing = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Leg", meta = (EditCondition = "AddLegs"))
	float FloorHeightAdjustment = 6.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	bool ViewMesh = true;
};
