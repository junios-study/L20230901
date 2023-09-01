// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubSystems.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocationAndRotation(
		FVector(0, 0, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()),
		FRotator(0, -90.0f, 0)
	);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	UEnhancedInputLocalPlayerSubsystem* LocalPlayerSystem
		= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());

	if (LocalPlayerSystem && DefaultMappingContext)
	{
		LocalPlayerSystem->AddMappingContext(DefaultMappingContext, 0);
	}
	
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(IA_MoveForwardAndRight, ETriggerEvent::Triggered, this, &AMyCharacter::MoveForwardAndRight);
		EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AMyCharacter::Look);
	}
}

void AMyCharacter::MoveForwardAndRight(const FInputActionValue& Value)
{
	FVector2D MoveVector = Value.Get<FVector2D>();

	FRotator Rotation = GetControlRotation();
	FRotator YawRotation = FRotator(0, Rotation.Yaw, 0);

	FVector ForwadVector =  UKismetMathLibrary::GetForwardVector(YawRotation);
	FVector RightVector = UKismetMathLibrary::GetRightVector(YawRotation);

	AddMovementInput(ForwadVector, MoveVector.X);
	AddMovementInput(RightVector, MoveVector.Y);
}

void AMyCharacter::Look(const FInputActionValue& Value)
{
	FVector2D CameraVector = Value.Get<FVector2D>();

	AddControllerPitchInput(CameraVector.Y);
	AddControllerYawInput(CameraVector.X);

	UE_LOG(LogTemp, Warning, TEXT("Look  %f"), CameraVector.Y);
}

