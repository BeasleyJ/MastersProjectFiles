// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ImpulseObject.h"
#include "Components/BoxComponent.h"
#include "Despawner.generated.h"

UCLASS()
class IMPULSE_API ADespawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADespawner();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Despawn")
	class UBoxComponent* DespawnVolume;

private:

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void DespawnObjects();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Despawn", meta = (AllowPrivateAccess = "true"))
	float DespawnDelay = 1.0f;

	int OverlappingObjects = 0;

	FTimerHandle DelayHandle;

};
