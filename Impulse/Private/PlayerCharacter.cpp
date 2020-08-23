// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
//#include "HeadMountedDisplayFunctionLibrary.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a follow camera
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(RootComponent); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	PlayerCamera->bUsePawnControlRotation = true; // Camera does not rotate relative to arm
}

// Input

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APlayerCharacter::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APlayerCharacter::StopSprinting);

	PlayerInputComponent->BindAction("FireLeftLaser", IE_Pressed, this, &APlayerCharacter::StartLeftLaser);
	PlayerInputComponent->BindAction("FireLeftLaser", IE_Released, this, &APlayerCharacter::StopLeftLaser);

	PlayerInputComponent->BindAction("FireRightLaser", IE_Pressed, this, &APlayerCharacter::StartRightLaser);
	PlayerInputComponent->BindAction("FireRightLaser", IE_Released, this, &APlayerCharacter::StopRightLaser);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::Interact);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APlayerCharacter::LookUpAtRate);
}

//must minimise logic in functions directly tied to player input

void APlayerCharacter::Sprint()
{
	GetCharacterMovement()->MaxWalkSpeed *= SprintSpeedMultiplier;
}

void APlayerCharacter::StopSprinting()
{
	GetCharacterMovement()->MaxWalkSpeed /= SprintSpeedMultiplier;
}

void APlayerCharacter::StartLeftLaser()
{
	if (DeviceRef)
	{
		DeviceRef->StartLeftLaser();
	}
}

void APlayerCharacter::StartRightLaser()
{
	if (DeviceRef)
	{
		DeviceRef->StartRightLaser();
	}
}

void APlayerCharacter::StopLeftLaser()
{
	if (DeviceRef)
	{
		DeviceRef->StopLeftLaser();
	}
}

void APlayerCharacter::StopRightLaser()
{
	if (DeviceRef)
	{
		DeviceRef->StopRightLaser();
	}
}

void APlayerCharacter::AttachDeviceToPlayer_Implementation()
{
	FVector Location = { 46,15,-32 };
	FRotator Rotation = { 0,0,0 };

	DeviceRef = GetWorld()->SpawnActor<AImpulseDevice>(SpawnedDevice, Location, Rotation);

	DeviceRef->AttachToComponent(PlayerCamera, FAttachmentTransformRules::KeepRelativeTransform);

	HasDevice = true;
}
//general line trace to interact with objects
AActor* APlayerCharacter::TraceLine(float Range)
{
	FVector Location;
	FRotator Rotation;

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);

	PC->GetPlayerViewPoint(Location, Rotation);

	FVector Start = Location;
	FVector End = Start + (Rotation.Vector() * Range);
	FHitResult Hit;

	FCollisionQueryParams TraceParams;

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams); //fires line from start to end point unless a blocking actor prevents this

	if (bHit)
	{
		AActor* HitActor = Hit.GetActor();

		return HitActor;
	}

	else
	{
		return nullptr;
	}
	
}

//intial logic to toggle gravity fields with button press, can be extended in Blueprints
void APlayerCharacter::Interact_Implementation()
{
	AActor* HitActor = TraceLine(InteractRange);

	if (ATriggerButton* Button = Cast<ATriggerButton>(HitActor))
	{
		TArray<AGravityField*> Fields = Button->GetGravityFields();

		if (Fields.Num() > 0)
		{
			for (AGravityField* Field : Fields)
			{
				Field->ToggleField();
			}
		}
	}
}

void APlayerCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

AImpulseDevice* APlayerCharacter::GetDevice()
{
	return DeviceRef;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


