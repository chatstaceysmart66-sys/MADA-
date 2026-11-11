#include "OWGCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "OWGVehiclePawn.h"

AOWGCharacter::AOWGCharacter()
{
    PrimaryActorTick.bCanEverTick = false;

    GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
    GetCharacterMovement()->JumpZVelocity = 430.0f;
    GetCharacterMovement()->AirControl = 0.25f;
    GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
    GetCharacterMovement()->BrakingDecelerationWalking = 1700.0f;
    GetCharacterMovement()->GroundFriction = 7.5f;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 360.0f;
    CameraBoom->SocketOffset = FVector(0.0f, 55.0f, 65.0f);
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;
}

void AOWGCharacter::BeginPlay()
{
    Super::BeginPlay();
    GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
}

void AOWGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    check(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AOWGCharacter::MoveForward);
    PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AOWGCharacter::MoveRight);
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AOWGCharacter::TurnCamera);
    PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AOWGCharacter::LookCamera);
    PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AOWGCharacter::StartJump);
    PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &AOWGCharacter::StopJump);
    PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &AOWGCharacter::StartSprint);
    PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &AOWGCharacter::StopSprint);
    PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &AOWGCharacter::Interact);
}

void AOWGCharacter::MoveForward(float Value)
{
    if (!Controller || FMath::IsNearlyZero(Value)) return;
    const FRotator YawRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
    AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X), Value);
}

void AOWGCharacter::MoveRight(float Value)
{
    if (!Controller || FMath::IsNearlyZero(Value)) return;
    const FRotator YawRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
    AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y), Value);
}

void AOWGCharacter::TurnCamera(float Value)
{
    AddControllerYawInput(Value);
}

void AOWGCharacter::LookCamera(float Value)
{
    AddControllerPitchInput(Value);
}

void AOWGCharacter::SetSprintHeld(bool bHeld)
{
    if (UCharacterMovementComponent* Movement = GetCharacterMovement())
    {
        Movement->MaxWalkSpeed = bHeld ? SprintWalkSpeed : BaseWalkSpeed;
    }
}

void AOWGCharacter::StartSprint()
{
    SetSprintHeld(true);
}

void AOWGCharacter::StopSprint()
{
    SetSprintHeld(false);
}

void AOWGCharacter::Interact()
{
    TryEnterNearbyVehicle();
}

void AOWGCharacter::TryEnterNearbyVehicle()
{
    if (!GetWorld() || !Controller) return;

    TArray<AActor*> Vehicles;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AOWGVehiclePawn::StaticClass(), Vehicles);

    AOWGVehiclePawn* ClosestVehicle = nullptr;
    float ClosestDistanceSquared = FMath::Square(350.0f);

    for (AActor* Actor : Vehicles)
    {
        AOWGVehiclePawn* Vehicle = Cast<AOWGVehiclePawn>(Actor);
        if (!Vehicle || Vehicle->GetDriverCharacter()) continue;

        const float DistanceSquared = FVector::DistSquared(GetActorLocation(), Vehicle->GetActorLocation());
        if (DistanceSquared <= ClosestDistanceSquared)
        {
            ClosestDistanceSquared = DistanceSquared;
            ClosestVehicle = Vehicle;
        }
    }

    if (ClosestVehicle)
    {
        ClosestVehicle->EnterVehicle(this);
    }
}
