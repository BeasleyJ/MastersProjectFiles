// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerLauncher.h"

void ASpawnerLauncher::FireObject(AImpulseObject* SpawnedObject)
{
	//fire object on launcher tangent with a custom size
	if (UseCustomVelocity)
	{
		float LaunchAngle = OriginMesh->GetRelativeRotation().Roll;

		float VelocityX = CustomVelocityMagnitude * UKismetMathLibrary::DegCos(LaunchAngle); //horizontal component
		float VelocityZ = CustomVelocityMagnitude * UKismetMathLibrary::DegSin(LaunchAngle); //vertical component

		FVector InitialVelocity = FVector(VelocityX, 0.0f, VelocityZ);
		FVector AdjustedInitialVelocity = GetActorTransform().GetRotation().RotateVector(InitialVelocity); //alter velocity to face actor rotation

		SpawnedObject->ObjectMesh->SetPhysicsLinearVelocity(AdjustedInitialVelocity, false, NAME_None);

	}

	else
	{
		FireAtTarget(SpawnedObject);
	}
}

void ASpawnerLauncher::FireAtTarget(AImpulseObject* ImpulseObject)
{
	FVector Origin = OriginSpawn->GetRelativeLocation();
	FVector Target = TargetDespawn->GetRelativeLocation();

	//use horizontal range, launch angle and height difference to calculate initial velocity
	float XRange = UKismetMathLibrary::Abs(Origin.X - Target.X);
	float LaunchAngle = OriginMesh->GetRelativeRotation().Roll;
	float ZOffset = (Origin.Z - Target.Z);

	float VelocityMag = UKismetMathLibrary::Sqrt((UKismetMathLibrary::Square(XRange) * 0.5 * 981) / (XRange * UKismetMathLibrary::DegTan(LaunchAngle) + ZOffset)) / UKismetMathLibrary::DegCos(LaunchAngle); //re-arranged projectile physics equation, gravity is adjusted to Unreal units

	float VelocityX = VelocityMag * UKismetMathLibrary::DegCos(LaunchAngle);
	float VelocityZ = VelocityMag * UKismetMathLibrary::DegSin(LaunchAngle);

	FVector InitialVelocity = FVector(VelocityX, 0.0f, VelocityZ);
	FVector AdjustedInitialVelocity = GetActorTransform().GetRotation().RotateVector(InitialVelocity);

	ImpulseObject->ObjectMesh->SetPhysicsLinearVelocity(AdjustedInitialVelocity, false, NAME_None);
}
