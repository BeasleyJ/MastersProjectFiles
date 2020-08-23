// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerCharacter.h"
#include "ResetField.generated.h"

UCLASS()
class IMPULSE_API AResetField : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AResetField();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent, Category = "Overlap")
	void ResetDevice(AImpulseDevice* Device);

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
	class UStaticMeshComponent* FieldMesh;

private:

	UFUNCTION()
	void OnFieldBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	

};
