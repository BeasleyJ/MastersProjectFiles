// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Conveyor.generated.h"


UCLASS()
class IMPULSE_API AConveyor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AConveyor();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Spline")
	class USplineComponent* ConveyorPath;

private:

	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	USplineMeshComponent* SpawnRoller(USplineMeshComponent* SplineComponent, int LoopIndex);

	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	void SpawnSideSectionWithConstraint(USplineMeshComponent* SplineComponent, UPhysicsConstraintComponent* Constraint, int LoopIndex, USplineMeshComponent* RollerSplineMesh, bool IsLeftSide);

	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	void SpawnLeg(USplineMeshComponent* SplineComponent, int LoopIndex, bool IsLeftLeg);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	UStaticMesh* RollerMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	UStaticMesh* LeftSideMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	UStaticMesh* RightSideMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	UStaticMesh* LeftLegMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	UStaticMesh* RightLegMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MeshDimensions", meta = (AllowPrivateAccess = "true"))
	float RollerSectionLength = 12.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MeshDimensions", meta = (AllowPrivateAccess = "true"))
	float RollerHalfWidth = 36.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MeshDimensions", meta = (AllowPrivateAccess = "true"))
	float SideSectionLength = 15.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MeshDimensions", meta = (AllowPrivateAccess = "true"))
	float LegSectionLength = 12.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MeshDimensions", meta = (AllowPrivateAccess = "true"))
	float LegLengthZ = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Roller", meta = (AllowPrivateAccess = "true"))
	float RollerWidthScale = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Roller", meta = (AllowPrivateAccess = "true"))
	float RollerMass = 3.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Roller", meta = (AllowPrivateAccess = "true"))
	float RollerSpacing = 3.0f;

 	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Leg", meta = (AllowPrivateAccess = "true"))
	bool AddLegs = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Leg", meta = (EditCondition = "AddLegs"), meta = (AllowPrivateAccess = "true")) //only editable if AddLegs is true
	float LegSpacing = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Leg", meta = (EditCondition = "AddLegs"), meta = (AllowPrivateAccess = "true"))
	float FloorHeightAdjustment = 6.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	bool ViewMesh = true;

};
