// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ImpulseObject.h"
#include "Components/BoxComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "ObjectiveButton.generated.h"

UCLASS()
class IMPULSE_API AObjectiveButton : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObjectiveButton();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* Holder;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* Button;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Physics")
	UPhysicsConstraintComponent* ButtonConstraint;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Collision")
	class UBoxComponent* ButtonCollision;

	bool GetIsPressed();

private:


	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
	bool IsPressed = false;

	int OverlappingObjectsCount = 0;

	




};
