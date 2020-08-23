// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ImpulseObject.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Spawner.generated.h"

UCLASS()
class IMPULSE_API ASpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawner();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
	class UStaticMeshComponent* OriginMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
	class UStaticMeshComponent* TargetMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
	class USphereComponent* OriginSpawn;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Despawn")
	class UBoxComponent* TargetDespawn;

	UFUNCTION(BlueprintCallable, Category = "State")
	void SetIsActive(bool SpawnerIsActive);



protected:

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	virtual UPARAM(DisplayName = "SpawnedObject") AImpulseObject* SpawnImpulseObject(); //uparam allows custom function output title

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void DespawnObject();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	float SpawnDelay = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Despawn")
	bool DespawnOnTargetReached = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Despawn", meta = (EditCondition = "DespawnOnTargetReached"))
	float DespawnDelay = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool IsActive = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	TSubclassOf<AImpulseObject> ImpulseObjectToSpawn;

	FTimerHandle DelayHandle;

	AImpulseObject* Object;
	


};
