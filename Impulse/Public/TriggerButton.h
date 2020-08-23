// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GravityField.h"
#include "TriggerButton.generated.h"

UCLASS()
class IMPULSE_API ATriggerButton : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATriggerButton();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
	class UStaticMeshComponent* StandMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
	class UStaticMeshComponent* ButtonMesh;

	TArray<AGravityField*> GetGravityFields();

private:

	//stores gravity fields to alter them when button pressed
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GravityField", meta = (AllowPrivateAccess = "true"))
	TArray<AGravityField*> Fields;






};
