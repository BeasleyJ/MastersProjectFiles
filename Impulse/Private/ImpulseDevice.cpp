// Fill out your copyright notice in the Description page of Project Settings.


#include "ImpulseDevice.h"


// Sets default values
AImpulseDevice::AImpulseDevice()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	DeviceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DeviceMesh"));
	DeviceMesh->SetupAttachment(RootComponent);

	DeviceEnd = CreateDefaultSubobject<USphereComponent>(TEXT("DeviceEnd"));
	DeviceEnd->SetupAttachment(DeviceMesh);

	LeftLaserMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftLaserMesh"));
	LeftLaserMesh->SetupAttachment(DeviceEnd);

	RightLaserMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightLaserMesh"));
	RightLaserMesh->SetupAttachment(DeviceEnd);

}

// Called when the game starts or when spawned
void AImpulseDevice::BeginPlay()
{
	Super::BeginPlay();
	
}

void AImpulseDevice::StartLeftLaser()
{
	if (LaserSetting == 0)
	{
		LaserSetting = 1; //setting for left laser on

		GetWorldTimerManager().SetTimer(BeamTickHandle, this, &AImpulseDevice::FireLaser, BeamTick, true);
	}
}

void AImpulseDevice::StartRightLaser()
{
	if (LaserSetting == 0)
	{
		LaserSetting = 2;  //setting for right laser on

		GetWorldTimerManager().SetTimer(BeamTickHandle, this, &AImpulseDevice::FireLaser, BeamTick, true); //function Fire Laser repeats until timer handle cleared
	}
}

void AImpulseDevice::FireLaser()
{
	if (LaserHitObject == false)
	{
		TraceLine();
	}

	else if (LaserSetting == 1) //if laser has hit an object and left laser is on
	{
		StopLeftLaser();
	}

	else //if laser has hit an object and right laser is on
	{
		StopRightLaser();
	}
}

//fires line traces to determine end point, scales laser beam to reach it, then apply device rules if object has been hit 
void AImpulseDevice::TraceLine()
{
	FVector Location;
	FRotator Rotation;

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);

	PC->GetPlayerViewPoint(Location, Rotation);

	FVector Start = Location;
	FVector End = Start + (Rotation.Vector() * TraceLength);
	FHitResult Hit;

	FCollisionQueryParams TraceParams;

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams); //fires line from start to end point unless a blocking actor prevents this

	if (bHit) //on hit actor
	{
		Start = DeviceEnd->GetComponentLocation();

		End = Hit.ImpactPoint;

		FVector TraceExtension = End - Start;

		End += TraceExtension; //extends line as it would sometimes not reach the actor hit by the first line

		bool bSecondHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams); //fires another line from end of device to the location of the first hit

		//DrawDebugLine(GetWorld(), Start, End, FColor::Orange, false, BeamTick);

		if (bSecondHit) //if second line trace hits an actor
		{
			BeamEndPoint = Hit.ImpactPoint;
			//DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(5, 5, 5), FColor::Orange, false, BeamTick);

			FVector BeamDirection = BeamEndPoint - Start;
			float TraceDist = BeamDirection.Size() / BeamMeshLength;

			if (LaserSetting == 1)
			{
				LeftLaserMesh->SetWorldScale3D(FVector(0.05, 0.05, TraceDist)); //sizes the mesh to reach the impact point of the line trace

				FRotator BeamRotation = FRotationMatrix::MakeFromZ(BeamDirection).Rotator(); 

				LeftLaserMesh->SetWorldRotation(BeamRotation); //rotates the beam to face the same direction as the line

				if (AImpulseObject* ImpulseObject = Cast<AImpulseObject>(Hit.GetActor()))
				{
					ObjectRef = ImpulseObject; //sets currently targeted object to apply the laser effect to

					LaserHitObject = true;
				}

				else {}
			}

			else //if right laser is turned on
			{
				RightLaserMesh->SetWorldScale3D(FVector(0.05, 0.05, TraceDist));  //sizes the mesh to reach the impact point of the line trace

				FRotator BeamRotation = FRotationMatrix::MakeFromZ(BeamDirection).Rotator();

				RightLaserMesh->SetWorldRotation(BeamRotation); //rotates the beam to face the same direction as the line

				if (AImpulseObject* ImpulseObject = Cast<AImpulseObject>(Hit.GetActor()))
				{
					ObjectRef = ImpulseObject; //sets currently targeted object to apply the laser effect to

					LaserHitObject = true;
				}

				else {}
			}

		}
	}
}

void AImpulseDevice::StopLeftLaser()
{
	if (LaserSetting == 0) {} //if laser is already off, do nothing

	else if (LaserHitObject)
	{
		GetWorldTimerManager().ClearTimer(BeamTickHandle); //clears repeating function

		LaserSetting = 0; //switches laser off

		OnHitLeftLaser(); //function is defined in Blueprint 

		LaserHitObject = false;

		GetWorldTimerManager().SetTimer(BeamDelayHandle, this, &AImpulseDevice::SwitchOffLeftLaser, BeamSwitchOffDelay, false); //executes function once to delay to switching off of the beam by a user-defined value
	}

	else if (LaserSetting == 1) //if user released the fire left laser button
	{
		GetWorldTimerManager().ClearTimer(BeamTickHandle); 

		LaserSetting = 0;

		LeftLaserMesh->SetWorldScale3D(FVector(0.05, 0.05, 0)); //deletes mesh
	}

	else {}
}

void AImpulseDevice::StopRightLaser()
{
	if (LaserSetting == 0) {}  //if laser is already off, do nothing

	else if (LaserHitObject)
	{
		GetWorldTimerManager().ClearTimer(BeamTickHandle); //clears repeating function

		LaserSetting = 0; //switches laser off

		OnHitRightLaser();  //function is defined in Blueprint 

		LaserHitObject = false;

		GetWorldTimerManager().SetTimer(BeamDelayHandle, this, &AImpulseDevice::SwitchOffRightLaser, BeamSwitchOffDelay, false);  //executes function once to delay to switching off of the beam by a user-defined value

	}

	else if (LaserSetting == 2) //if user released the fire right laser button
	{
		GetWorldTimerManager().ClearTimer(BeamTickHandle);

		LaserSetting = 0;

		RightLaserMesh->SetWorldScale3D(FVector(0.05, 0.05, 0)); //beam mesh is not visible
	}

	else {}
}

void AImpulseDevice::SetStoredLinearVelocity(const FVector Velocity)
{
	StoredLinearVelocity = Velocity;
}

void AImpulseDevice::SetStoredAngularVelocity(const FVector Velocity)
{
	StoredAngularVelocity = Velocity;
}

void AImpulseDevice::SwitchOffLeftLaser()
{
	LeftLaserMesh->SetWorldScale3D(FVector(0.05, 0.05, 0)); //beam mesh is not visible

	GetWorldTimerManager().ClearTimer(BeamDelayHandle);
}

void AImpulseDevice::SwitchOffRightLaser()
{
	RightLaserMesh->SetWorldScale3D(FVector(0.05, 0.05, 0)); //beam mesh is not visible

	GetWorldTimerManager().ClearTimer(BeamDelayHandle);
}

// Called every frame
void AImpulseDevice::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

