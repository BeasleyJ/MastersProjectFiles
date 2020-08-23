// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ImpulseObject.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Controller.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "ImpulseDevice.generated.h"

UCLASS()
class IMPULSE_API AImpulseDevice : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AImpulseDevice();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
	class UStaticMeshComponent* DeviceMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Position")
	class USphereComponent* DeviceEnd;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
	class UStaticMeshComponent* LeftLaserMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
	class UStaticMeshComponent* RightLaserMesh;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void StartLeftLaser();

	void StartRightLaser();

	void StopLeftLaser();

	void StopRightLaser();

	UFUNCTION(BlueprintImplementableEvent, Category = "Laser") //rules defined in Blueprints
	void OnHitLeftLaser();

	UFUNCTION(BlueprintImplementableEvent, Category = "Laser") //rules defined in Blueprints
	void OnHitRightLaser();

	void SetStoredLinearVelocity(const FVector Velocity);

	void SetStoredAngularVelocity(const FVector Velocity);

private:

	void FireLaser();

	void TraceLine();

	void SwitchOffLeftLaser();

	void SwitchOffRightLaser();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh Dimensions", meta = (AllowPrivateAccess = "true"))
	float BeamMeshLength = 100.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References", meta = (AllowPrivateAccess = "true"))
	AImpulseObject* ObjectRef;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	float TraceLength = 5000.0f; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	float BeamTick = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	float BeamSwitchOffDelay = 0.5f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	int DeviceMode = 1;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "ImpulseMode", meta = (AllowPrivateAccess = "true"))
	FVector StoredLinearVelocity;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "ImpulseMode", meta = (AllowPrivateAccess = "true"))
	FVector StoredAngularVelocity;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "ImpulseMode", meta = (AllowPrivateAccess = "true"))
	float StoredMass;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "ImpulseMode", meta = (AllowPrivateAccess = "true"))
	float MassRatio;

	int LaserSetting = 0; //record on whether device is off (0), left laser is on (1) or right laser is on (2) 

	bool LaserHitObject = false;

	FVector BeamEndPoint;

	FTimerHandle BeamTickHandle;

	FTimerHandle BeamDelayHandle;

};
