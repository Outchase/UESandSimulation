// Fill out your copyright notice in the Description page of Project Settings.


#include "StealthCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"


// Sets default values
AStealthCharacter::AStealthCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);

	SpringArmComp->TargetArmLength = 250.f;
	SpringArmComp->SetRelativeLocation(FVector(0.f, 20.f, 70.f));
	SpringArmComp->SocketOffset = FVector(0.f, 40.f, 0.f);
	SpringArmComp->bUsePawnControlRotation = true;


	SetupStimulusSource();
}

// Called when the game starts or when spawned
void AStealthCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController())) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(StealthMappingContext, 0);
		}
	}

}

void AStealthCharacter::Move(const FInputActionValue& Value) {
	const FVector2D MoveValue = Value.Get<FVector2D>();

	//const FVector Forward = GetActorForwardVector();
	//const FVector Strafe = GetActorRightVector();

	//Directional movement
	//Get Controllers Rotation
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	//coressponding to controller rotation who is pointing in
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (GetController()) {

		AddMovementInput(ForwardDirection, MoveValue.Y);
		AddMovementInput(RightDirection, MoveValue.X);

		//UE_LOG(LogTemp, Warning, TEXT("Move X: %f  Y: %f "), MoveValue.X, MoveValue.YB);

	}
}

void AStealthCharacter::Look(const FInputActionValue& Value) {
	const FVector2D LookAxisValue = Value.Get<FVector2D>();

	if (GetController()) {
		AddControllerYawInput(LookAxisValue.X); //left and right
		AddControllerPitchInput(LookAxisValue.Y); // up and down
	}
}

void AStealthCharacter::LookRatio(const FInputActionValue& Value) {
	const FVector2D LookAxisValue = Value.Get<FVector2D>();

	if (GetController()) {
		AddControllerYawInput(LookAxisValue.X * (RotationRate * Sensibilty) * GetWorld()->GetDeltaSeconds()); //left and right
		AddControllerPitchInput(LookAxisValue.Y * (RotationRate * Sensibilty) * GetWorld()->GetDeltaSeconds()); // up and down

	}
}

// Called every frame
void AStealthCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AStealthCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComp = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComp->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AStealthCharacter::Move);
		EnhancedInputComp->BindAction(LookAction, ETriggerEvent::Triggered, this, &AStealthCharacter::Look);
		EnhancedInputComp->BindAction(LookRatioAction, ETriggerEvent::Triggered, this, &AStealthCharacter::LookRatio);
	}
}

void AStealthCharacter::SetupStimulusSource(){
	StimulusSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimulusComp"));

	if (StimulusSource) {
		
		StimulusSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
		StimulusSource->RegisterForSense(TSubclassOf<UAISense_Hearing>());
		StimulusSource->RegisterWithPerceptionSystem();
	}
}