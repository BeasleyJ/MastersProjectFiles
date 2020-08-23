// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerConveyor.h"

//can add an intial velocity, either in direction of starting mesh or a conveyor reference
void ASpawnerConveyor::SetInitialVelocityAndRotation(AImpulseObject* SpawnedObject)
{
	FVector InitialVelocity = FVector(0.0f, 0.0f, 0.0f);
	float LaunchAngle = OriginMesh->GetRelativeRotation().Roll;

	//can add velocity on object spawn
	if (AddVelocity)
	{
		InitialVelocity.X = IntialVelocityMagnitude * UKismetMathLibrary::DegCos(LaunchAngle); //horizontal component
		InitialVelocity.Z = IntialVelocityMagnitude * UKismetMathLibrary::DegSin(LaunchAngle); //vertical component

		if (ConveyorRef) //if conveyor ref is valid conveyor
		{
			FRotator ConveyorRot = UKismetMathLibrary::MakeRotFromX(ConveyorRef->ConveyorPath->GetLeaveTangentAtSplinePoint(0, ESplineCoordinateSpace::Local)); //direction of travel for conveyor
			FRotator ActorRot = GetActorRotation();
			FRotator CombinedRot = UKismetMathLibrary::ComposeRotators(ConveyorRot, ActorRot);

			InitialVelocity = CombinedRot.RotateVector(InitialVelocity); //alters velocity to face direction of conveyor

			SpawnedObject->ObjectMesh->SetWorldRotation(CombinedRot); //rotates mesh to face direction of conveyor
		}

		else
		{
			//rotate and alter velocity to face forward vector of actor
			InitialVelocity = GetActorRotation().RotateVector(InitialVelocity);
			SpawnedObject->ObjectMesh->SetWorldRotation(GetActorRotation());
		}
	}

	//if no velocity added
	else
	{
		InitialVelocity = GetActorRotation().RotateVector(InitialVelocity); //rotate to face away from spawn

		if (ConveyorRef)
		{
			FRotator ConveyorRot = UKismetMathLibrary::MakeRotFromX(ConveyorRef->ConveyorPath->GetLeaveTangentAtSplinePoint(0, ESplineCoordinateSpace::Local));
			FRotator ActorRot = GetActorRotation();
			FRotator CombinedRot = UKismetMathLibrary::ComposeRotators(ConveyorRot, ActorRot);

			SpawnedObject->ObjectMesh->SetWorldRotation(CombinedRot); //rotates mesh to face direction of conveyor
		}

		else
		{
			SpawnedObject->ObjectMesh->SetWorldRotation(GetActorRotation());  //rotate to face forward vector of actor
		}
	}

	SpawnedObject->ObjectMesh->SetPhysicsLinearVelocity(InitialVelocity);
}