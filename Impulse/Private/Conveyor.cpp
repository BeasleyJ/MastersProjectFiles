// Fill out your copyright notice in the Description page of Project Settings.


#include "Conveyor.h"


// Sets default values
AConveyor::AConveyor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	ConveyorPath = CreateDefaultSubobject<USplineComponent>(TEXT("ConveyorPath"));
	ConveyorPath->SetupAttachment(RootComponent);

}

USplineMeshComponent* AConveyor::SpawnRoller(USplineMeshComponent* SplineComponent, int LoopIndex)
{
	SplineComponent->SetStaticMesh(RollerMesh);
	SplineComponent->SetMobility(EComponentMobility::Movable);

	//roller is set as a physics object and attached to the spline
	SplineComponent->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
	SplineComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SplineComponent->SetSimulatePhysics(true);
	SplineComponent->SetMassOverrideInKg(NAME_None, RollerMass, true);
	SplineComponent->SetAngularDamping(0.05f);
	SplineComponent->SetForwardAxis(ESplineMeshAxis::Type::X, true);
	SplineComponent->SetRelativeScale3D(FVector(1, RollerWidthScale, 1));

	float StartDistance = (LoopIndex * RollerSectionLength) + (LoopIndex * RollerSpacing); //beginning of the mesh
	FVector StartPos = ConveyorPath->GetLocationAtDistanceAlongSpline(StartDistance, ESplineCoordinateSpace::Local);
	FVector StartTangent = ConveyorPath->GetTangentAtDistanceAlongSpline(StartDistance, ESplineCoordinateSpace::Local);

	float EndDistance = ((LoopIndex + 1) * RollerSectionLength) + (LoopIndex * RollerSpacing); //end of the mesh
	FVector EndPos = ConveyorPath->GetLocationAtDistanceAlongSpline(EndDistance, ESplineCoordinateSpace::Local);
	FVector EndTangent = ConveyorPath->GetTangentAtDistanceAlongSpline(EndDistance, ESplineCoordinateSpace::Local);

	//clamp tangents to avoid exponential tangent scaling
	FVector ClampedStartTangent = UKismetMathLibrary::ClampVectorSize(StartTangent, 0, RollerSectionLength);
	FVector ClampedEndTangent = UKismetMathLibrary::ClampVectorSize(EndTangent, 0, RollerSectionLength);

	SplineComponent->SetStartAndEnd(StartPos, ClampedStartTangent, EndPos, ClampedEndTangent, true); //sets the spline mesh component in space

	return SplineComponent;
}

void AConveyor::SpawnSideSectionWithConstraint(USplineMeshComponent* SplineComponent, UPhysicsConstraintComponent* Constraint, int LoopIndex, USplineMeshComponent* RollerSplineMesh, bool IsLeftSide)
{
	float OffsetMultiplier = 1;

	float SideSectionSpacing = 0.0f;

	UStaticMesh* Mesh;
	
	if (IsLeftSide) //if spawning a mesh is on the left side of the conveyor, transform position in that direction
	{
		OffsetMultiplier = -1;

		Mesh = LeftSideMesh;
	}
	else
	{
		Mesh = RightSideMesh;
	}

	//does not need to simulate physics but collision is turned on
	SplineComponent->SetStaticMesh(Mesh);
	SplineComponent->SetMobility(EComponentMobility::Movable);
	SplineComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SplineComponent->SetForwardAxis(ESplineMeshAxis::Type::X, true);
	SplineComponent->AttachToComponent(ConveyorPath, FAttachmentTransformRules::KeepRelativeTransform);

	float SideOffsetY = ((RollerWidthScale - 1.0f) * RollerHalfWidth) * OffsetMultiplier; //account for any change in roller width, in direction calculated earlier
	SplineComponent->SetRelativeLocation(FVector(0.0f, SideOffsetY, 0.0f));

	float StartDistance = (LoopIndex * SideSectionLength) + (LoopIndex * SideSectionSpacing); //beginning of the mesh
	FVector StartPos = ConveyorPath->GetLocationAtDistanceAlongSpline(StartDistance, ESplineCoordinateSpace::Local);
	FVector StartTangent = ConveyorPath->GetTangentAtDistanceAlongSpline(StartDistance, ESplineCoordinateSpace::Local);

	float EndDistance = ((LoopIndex + 1) * SideSectionLength) + (LoopIndex * SideSectionSpacing); //end of the mesh
	FVector EndPos = ConveyorPath->GetLocationAtDistanceAlongSpline(EndDistance, ESplineCoordinateSpace::Local);
	FVector EndTangent = ConveyorPath->GetTangentAtDistanceAlongSpline(EndDistance, ESplineCoordinateSpace::Local);

	FVector ClampedStartTangent = UKismetMathLibrary::ClampVectorSize(StartTangent, 0, SideSectionLength);
	FVector ClampedEndTangent = UKismetMathLibrary::ClampVectorSize(EndTangent, 0, SideSectionLength);

	SplineComponent->SetStartAndEnd(StartPos, ClampedStartTangent, EndPos, ClampedEndTangent, true); //sets the spline mesh component in space

	FVector RollerStartPos = RollerSplineMesh->GetStartPosition();
	FVector RollerEndPos = RollerSplineMesh->GetEndPosition();
	FVector RollerCentrePos = (RollerStartPos + RollerEndPos) / 2.0f;

	float ConstraintOffsetY = ((RollerWidthScale * RollerHalfWidth) + 0.5) * OffsetMultiplier; //positions the constraint between the roller and side mesh

	FVector ConstraintPos = RollerCentrePos + FVector(0.0f, ConstraintOffsetY, 0.0f);

	Constraint->AttachToComponent(ConveyorPath, FAttachmentTransformRules::KeepRelativeTransform);
	Constraint->SetRelativeLocation(ConstraintPos);
	Constraint->SetConstrainedComponents(SplineComponent, NAME_None, RollerSplineMesh, NAME_None); //attaches the constraint to the necessary meshes
	Constraint->SetDisableCollision(true); //disables collision between the two meshes

	//lock rotation in two axis, while allowing free rotation in the necessary one
	Constraint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 45.0f);
	Constraint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Free, 45.0f);
	Constraint->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 45.0f);

}

void AConveyor::SpawnLeg(USplineMeshComponent* SplineComponent, int LoopIndex, bool IsLeftLeg)
{
	float OffsetMultiplier = 1;

	UStaticMesh* Mesh;

	if (IsLeftLeg)  //if spawning a leg is on the left of the conveyor, transform position in that direction
	{
		OffsetMultiplier = -1;

		Mesh = LeftLegMesh;
	}
	else
	{
		Mesh = RightLegMesh;
	}

	//does not need to simulate physics but collision is turned on
	SplineComponent->SetStaticMesh(Mesh);
	SplineComponent->SetMobility(EComponentMobility::Movable);
	SplineComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SplineComponent->SetForwardAxis(ESplineMeshAxis::Type::X, true);
	SplineComponent->AttachToComponent(ConveyorPath, FAttachmentTransformRules::KeepRelativeTransform);

	float SideOffsetY = ((RollerWidthScale - 1.0f) * RollerHalfWidth) * OffsetMultiplier;  //account for any change in roller width, in direction calculated earlier
	SplineComponent->SetRelativeLocation(FVector(0.0f, SideOffsetY, 0.0f));

	//tangents are not required as legs must be perpendicular to the floor, rather than the spline
	float StartDistance = (LoopIndex * LegSectionLength) + (LoopIndex * LegSpacing);  //beginning of the mesh
	FVector StartPos = ConveyorPath->GetLocationAtDistanceAlongSpline(StartDistance, ESplineCoordinateSpace::Local);
	SplineComponent->SetStartPosition(StartPos);

	//scales leg according to the height of the floor to reduce need for editing
	FVector StartPosWorld = ConveyorPath->GetLocationAtDistanceAlongSpline(StartDistance, ESplineCoordinateSpace::World);
	float StartScaleY = (StartPosWorld.Z - FloorHeightAdjustment) / LegLengthZ;
	SplineComponent->SetStartScale(FVector2D(1.0f, StartScaleY));

	float EndDistance = ((LoopIndex + 1) * LegSectionLength) + (LoopIndex * LegSpacing); //end of the mesh
	FVector EndPos = ConveyorPath->GetLocationAtDistanceAlongSpline(EndDistance, ESplineCoordinateSpace::Local);
	SplineComponent->SetEndPosition(EndPos);

	FVector EndPosWorld = ConveyorPath->GetLocationAtDistanceAlongSpline(EndDistance, ESplineCoordinateSpace::World);
	float EndScaleY = (EndPosWorld.Z - FloorHeightAdjustment) / LegLengthZ;
	SplineComponent->SetEndScale(FVector2D(1.0f, EndScaleY));
}

