// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ImpulseObject.h"
#include "Components/SphereComponent.h"
#include "GravityField.generated.h"

UCLASS()
class IMPULSE_API AGravityField : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGravityField();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
	class UStaticMeshComponent* OriginMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
	class UStaticMeshComponent* TargetMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
	class UStaticMeshComponent* FieldMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Position")
	class USphereComponent* OriginBeamLocation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Position")
	class USphereComponent* TargetBeamLocation;

	UFUNCTION(BlueprintCallable, Category = "FieldControls")
	void ToggleField();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "FieldControls")
	void ReverseField();

	UFUNCTION(BlueprintCallable, Category = "FieldControls")
	void SwitchFieldOn();

	UFUNCTION(BlueprintCallable, Category = "FieldControls")
	void SwitchFieldOff();

private:

	UFUNCTION(BlueprintCallable, Category = "BeamSetup", meta = (AllowPrivateAccess = "true"))
	void InitialiseBeam();

	UFUNCTION()
	void OnFieldBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnFieldEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void ApplyGravityField();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FieldControls", meta = (AllowPrivateAccess = "true"))
	bool FieldOn = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FieldControls", meta = (AllowPrivateAccess = "true"))
	float FieldStrength = 15.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "BeamSetup", meta = (AllowPrivateAccess = "true"))
	float BeamLength = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "MeshDimensions", meta = (AllowPrivateAccess = "true"))
	float BeamSectionLength = 120.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Material", meta = (AllowPrivateAccess = "true"))
	UMaterialInstanceDynamic* BeamMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material", meta = (AllowPrivateAccess = "true"))
	float PanSpeedMatchModifier = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material", meta = (AllowPrivateAccess = "true"))
	float PanSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material", meta = (AllowPrivateAccess = "true"))
	float MaterialRepeatFrequency = 10.0f;
	 
	FVector FieldStart;

	FVector FieldEnd;

	FVector FieldImpulse;

	int OverlappingObjects = 0;
	
	

};
