// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ImpulseDevice.h"
#include "TriggerButton.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerCharacter.generated.h"

UCLASS(config = Game)
class IMPULSE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Returns FollowCamera subobject 
	FORCEINLINE class UCameraComponent* GetPlayerCamera() const { return PlayerCamera; }

	// head mounted camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* PlayerCamera;

	// Base turn rate, in deg/sec. Other scaling may affect final turn rate
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseTurnRate;

	// Base look up/down rate, in deg/sec. Other scaling may affect final rate
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseLookUpRate;

	UFUNCTION(BlueprintCallable, Category = "Device")
	virtual UPARAM(DisplayName = "Device Ref") AImpulseDevice* GetDevice();


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Device")
	void AttachDeviceToPlayer();

	UFUNCTION(BlueprintNativeEvent, Category = "Interact")
	void Interact();

private:

	// Called for forwards/backward input 
	void MoveForward(float Value);

	// Called for side to side input 
	void MoveRight(float Value);

	// Called via input to turn at a given rate
	// @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	void TurnAtRate(float Rate);

	// Called via input to turn look up/down at a given rate.
	// @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	void LookUpAtRate(float Rate);

	//Called via input to increase max walk speed to maximum
	void Sprint();

	//Called via input to decrease max walk speed to standard
	void StopSprinting();

	//Called via input to start firing left laser
	void StartLeftLaser();

	//Called via input to stop firing left laser
	void StartRightLaser();

	//Called via input to start firing right laser
	void StopLeftLaser();

	//Called via input to stop firing right laser
	void StopRightLaser();


	UFUNCTION(BlueprintCallable, Category = "Interact", meta = (AllowPrivateAccess = "true"))
	virtual UPARAM(DisplayName = "Hit Actor") AActor* TraceLine(float Range);

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Device", meta = (AllowPrivateAccess = "true"))
	bool HasDevice = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Movement: Walking", meta = (AllowPrivateAccess = "true"))
	float SprintSpeedMultiplier = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interact", meta = (AllowPrivateAccess = "true"))
	float InteractRange = 150.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "References", meta = (AllowPrivateAccess = "true"))
	AImpulseDevice* DeviceRef;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn", meta = (AllowPrivateAccess = true))
	TSubclassOf<AImpulseDevice> SpawnedDevice;

};

