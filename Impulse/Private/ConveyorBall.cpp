// Fill out your copyright notice in the Description page of Project Settings.


#include "ConveyorBall.h"

/*void AConveyorBall::OnConstruction(const FTransform& Transform)
{
	if (ViewMesh) //Can be turned off to improve performance while editing spline, then turned back on before runtime
	{
		Super::OnConstruction(Transform);

		FVector Scale = GetActorScale3D();
		ConveyorSectionLength *= Scale.X;

		float LastIndexConveyorFloat = ConveyorPath->GetSplineLength() / (ConveyorSectionLength + ConveyorSectionSpacing); //determines number of mesh sections by spline length
		int LastIndexConveyorInt = (int)LastIndexConveyorFloat - 1;

		for (int i = 0; i < LastIndexConveyorInt; i++) //spawn roller and side sections
		{
			SpawnConveyorSection(i);
		}

		if (AddLegs)
		{
			LegSectionLength *= Scale.X;

			float LastIndexLegFloat = ConveyorPath->GetSplineLength() / (LegSectionLength + LegSpacing);
			int LastIndexLegInt = (int)LastIndexLegFloat + 1;

			for (int j = 0; j < LastIndexLegInt; j++) //spawn legs
			{
				SpawnLegs(j);
			}
		}

		else {}
	}
}*/

// Sets default values
AConveyorBall::AConveyorBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	ConveyorPath = CreateDefaultSubobject<USplineComponent>(TEXT("ConveyorPath"));
	ConveyorPath->SetupAttachment(RootComponent);

}

void AConveyorBall::SpawnConveyorSection(USplineMeshComponent* SplineComponent, int LoopIndex)
{
	//process to create a dynamic component in the editor
	/*USplineMeshComponent* SplineComponent = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
	SplineComponent->RegisterComponentWithWorld(GetWorld());
	SplineComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;*/
	

	//roller is set as a physics object and attached to the spline
	SplineComponent->SetStaticMesh(ConveyorMesh);
	SplineComponent->SetMobility(EComponentMobility::Movable);
	SplineComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SplineComponent->SetForwardAxis(ESplineMeshAxis::Type::X, true);
	SplineComponent->AttachToComponent(ConveyorPath, FAttachmentTransformRules::KeepRelativeTransform);


	float StartDistance = (LoopIndex * ConveyorSectionLength) + (LoopIndex * ConveyorSectionSpacing);//beginning of the mesh
	FVector StartPos = ConveyorPath->GetLocationAtDistanceAlongSpline(StartDistance, ESplineCoordinateSpace::Local);
	FVector StartTangent = ConveyorPath->GetTangentAtDistanceAlongSpline(StartDistance, ESplineCoordinateSpace::Local);

	float EndDistance = ((LoopIndex + 1) * ConveyorSectionLength) + (LoopIndex * ConveyorSectionSpacing); //end of the mesh
	FVector EndPos = ConveyorPath->GetLocationAtDistanceAlongSpline(EndDistance, ESplineCoordinateSpace::Local);
	FVector EndTangent = ConveyorPath->GetTangentAtDistanceAlongSpline(EndDistance, ESplineCoordinateSpace::Local);

	//clamp tangents to avoid exponential tangent scaling
	FVector ClampedStartTangent = UKismetMathLibrary::ClampVectorSize(StartTangent, 0, ConveyorSectionLength);
	FVector ClampedEndTangent = UKismetMathLibrary::ClampVectorSize(EndTangent, 0, ConveyorSectionLength);

	SplineComponent->SetStartAndEnd(StartPos, ClampedStartTangent, EndPos, ClampedEndTangent, true); //sets the spline mesh component in space
}

void AConveyorBall::SpawnLegs(USplineMeshComponent* SplineComponent, int LoopIndex)
{
	//process to create a dynamic component in the editor
	/*USplineMeshComponent* SplineComponent = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
	SplineComponent->RegisterComponentWithWorld(GetWorld());
	SplineComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;*/
	

	//does not need to simulate physics but collision is turned on
	SplineComponent->SetStaticMesh(LegsMesh);
	SplineComponent->SetMobility(EComponentMobility::Movable);
	SplineComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SplineComponent->SetForwardAxis(ESplineMeshAxis::Type::X, true);
	SplineComponent->AttachToComponent(ConveyorPath, FAttachmentTransformRules::KeepRelativeTransform);

	//tangents are not required as legs must be perpendicular to the floor, rather than the spline
	float StartDistance = (LoopIndex * LegSectionLength) + (LoopIndex * LegSpacing);  //beginning of the mesh
	FVector StartPos = ConveyorPath->GetLocationAtDistanceAlongSpline(StartDistance, ESplineCoordinateSpace::Local);
	SplineComponent->SetStartPosition(StartPos);

	//scales leg according to the height of the floor to reduce need for editing
	FVector StartPosWorld = ConveyorPath->GetLocationAtDistanceAlongSpline(StartDistance, ESplineCoordinateSpace::World);
	float StartScaleY = (StartPosWorld.Z - FloorHeightAdjustment) / LegLengthZ;
	SplineComponent->SetStartScale(FVector2D(1.0f, StartScaleY));

	float EndDistance = ((LoopIndex + 1) * LegSectionLength) + ((LoopIndex) * LegSpacing); //end of the mesh
	FVector EndPos = ConveyorPath->GetLocationAtDistanceAlongSpline(EndDistance, ESplineCoordinateSpace::Local);
	SplineComponent->SetEndPosition(EndPos);

	FVector EndPosWorld = ConveyorPath->GetLocationAtDistanceAlongSpline(EndDistance, ESplineCoordinateSpace::World);
	float EndScaleY = (EndPosWorld.Z - FloorHeightAdjustment) / LegLengthZ;
	SplineComponent->SetEndScale(FVector2D(1.0f, EndScaleY));
}



