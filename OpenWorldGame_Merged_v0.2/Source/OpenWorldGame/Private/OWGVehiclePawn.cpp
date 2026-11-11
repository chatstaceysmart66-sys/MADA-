#include "OWGVehiclePawn.h"

#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/CollisionProfile.h"
#include "CollisionQueryParams.h"
#include "CollisionShape.h"
#include "Engine/World.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Core/OWUnits.h"
#include "Core/OWVehicleEntry.h"
#include "OWGCharacter.h"

#include <array>

AOWGVehiclePawn::AOWGVehiclePawn()
{
    PrimaryActorTick.bCanEverTick = true;
    AutoPossessAI = EAutoPossessAI::Disabled;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMesh(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));

    VehicleBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VehicleBody"));
    SetRootComponent(VehicleBody);

    if (CubeMesh.Succeeded())
    {
        VehicleBody->SetStaticMesh(CubeMesh.Object);
    }

    VehicleBody->SetRelativeScale3D(FVector(2.25f, 1.12f, 0.48f));
    VehicleBody->SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
    VehicleBody->SetSimulatePhysics(true);
    VehicleBody->SetEnableGravity(true);
    VehicleBody->SetMassOverrideInKg(NAME_None, 1250.0f, true);
    VehicleBody->SetLinearDamping(0.12f);
    VehicleBody->SetAngularDamping(2.8f);
    VehicleBody->SetNotifyRigidBodyCollision(true);

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("VehicleCameraBoom"));
    CameraBoom->SetupAttachment(VehicleBody);
    CameraBoom->TargetArmLength = 650.0f;
    CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 170.0f);
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VehicleCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    const FVector WheelLocations[] =
    {
        FVector(62.0f, -58.0f, -22.0f), FVector(62.0f, 58.0f, -22.0f),
        FVector(-62.0f, -58.0f, -22.0f), FVector(-62.0f, 58.0f, -22.0f)
    };

    for (int32 Index = 0; Index < 4; ++Index)
    {
        const FName WheelName(*FString::Printf(TEXT("WheelVisual_%d"), Index));
        UStaticMeshComponent* Wheel = CreateDefaultSubobject<UStaticMeshComponent>(WheelName);
        Wheel->SetupAttachment(VehicleBody);
        if (CylinderMesh.Succeeded())
        {
            Wheel->SetStaticMesh(CylinderMesh.Object);
        }
        Wheel->SetRelativeLocation(WheelLocations[Index]);
        Wheel->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
        Wheel->SetRelativeScale3D(FVector(0.34f, 0.34f, 0.34f));
        Wheel->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        Wheel->SetGenerateOverlapEvents(false);
        WheelVisuals.Add(Wheel);
    }
}

void AOWGVehiclePawn::BeginPlay()
{
    Super::BeginPlay();
    if (VehicleBody && !VehicleBody->IsSimulatingPhysics())
    {
        VehicleBody->SetSimulatePhysics(true);
    }
}

void AOWGVehiclePawn::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    if (!VehicleBody || !VehicleBody->IsSimulatingPhysics()) return;

    const FVector Velocity = VehicleBody->GetPhysicsLinearVelocity();
    const FVector Forward = VehicleBody->GetForwardVector();

    if (!FMath::IsNearlyZero(ThrottleInput))
    {
        VehicleBody->AddForce(Forward * ThrottleInput * EngineForce, NAME_None, false);
    }

    if (BrakeInput > 0.01f && Velocity.SizeSquared() > 25.0f)
    {
        VehicleBody->AddForce(-Velocity.GetSafeNormal() * BrakeInput * BrakeForce, NAME_None, false);
    }

    const float SpeedScale = FMath::Clamp(Velocity.Size() / 850.0f, 0.12f, 1.0f);
    if (!FMath::IsNearlyZero(SteeringInput) && Velocity.SizeSquared() > 100.0f)
    {
        VehicleBody->AddTorqueInRadians(FVector(0.0f, 0.0f, SteeringInput * SteeringTorque * SpeedScale), NAME_None, false);
    }
}

void AOWGVehiclePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    check(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("VehicleThrottle"), this, &AOWGVehiclePawn::ThrottleAxis);
    PlayerInputComponent->BindAxis(TEXT("VehicleSteer"), this, &AOWGVehiclePawn::SteeringAxis);
    PlayerInputComponent->BindAxis(TEXT("VehicleBrake"), this, &AOWGVehiclePawn::BrakeAxis);
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AOWGVehiclePawn::TurnCamera);
    PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AOWGVehiclePawn::LookCamera);
    PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &AOWGVehiclePawn::Interact);
}

void AOWGVehiclePawn::ThrottleAxis(float Value) { SetThrottleInput(Value); }
void AOWGVehiclePawn::SteeringAxis(float Value) { SetSteeringInput(Value); }
void AOWGVehiclePawn::BrakeAxis(float Value) { SetBrakeInput(Value); }
void AOWGVehiclePawn::Interact() { TryExitVehicle(); }
void AOWGVehiclePawn::TurnCamera(float Value) { AddControllerYawInput(Value); }
void AOWGVehiclePawn::LookCamera(float Value) { AddControllerPitchInput(Value); }

void AOWGVehiclePawn::SetThrottleInput(float Value)
{
    ThrottleInput = FMath::Clamp(Value, -1.0f, 1.0f);
}

void AOWGVehiclePawn::SetSteeringInput(float Value)
{
    SteeringInput = FMath::Clamp(Value, -1.0f, 1.0f);
}

void AOWGVehiclePawn::SetBrakeInput(float Value)
{
    BrakeInput = FMath::Clamp(Value, 0.0f, 1.0f);
}

float AOWGVehiclePawn::GetSpeedKmh() const
{
    if (!VehicleBody) return 0.0f;
    return static_cast<float>(OW::CmPerSecToKmh(VehicleBody->GetPhysicsLinearVelocity().Size()));
}

bool AOWGVehiclePawn::EnterVehicle(AOWGCharacter* Character)
{
    if (!IsValid(Character) || !VehicleBody || !GetWorld()) return false;

    OW::EntryRules EntryRules;
    EntryRules.MaxDoorDistanceCm = 350.0;
    EntryRules.MaxVehicleSpeedKmh = 5.0;
    const double DoorDistanceCm = FVector::Dist(Character->GetActorLocation(), GetActorLocation());
    if (!OW::CanEnterVehicle(DoorDistanceCm, GetSpeedKmh(), DriverCharacter == nullptr, EntryRules))
    {
        return false;
    }

    APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
    if (!PlayerController) return false;

    DriverCharacter = Character;
    Character->SetActorHiddenInGame(true);
    Character->SetActorEnableCollision(false);
    Character->GetCharacterMovement()->DisableMovement();
    PlayerController->Possess(this);
    if (PlayerController->GetPawn() != this)
    {
        Character->SetActorHiddenInGame(false);
        Character->SetActorEnableCollision(true);
        Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
        DriverCharacter = nullptr;
        return false;
    }
    return true;
}

bool AOWGVehiclePawn::TryExitVehicle()
{
    if (!IsValid(DriverCharacter) || !GetController() || !GetWorld()) return false;

    APlayerController* PlayerController = Cast<APlayerController>(GetController());
    if (!PlayerController) return false;

    AOWGCharacter* Character = DriverCharacter;
    const float Radius = Character->GetCapsuleComponent()->GetScaledCapsuleRadius();
    const float HalfHeight = Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
    const FVector Side = GetActorRightVector();
    const FVector Forward = GetActorForwardVector();
    const FVector Base = GetActorLocation() + FVector(0.0f, 0.0f, HalfHeight + 10.0f);
    const std::array<FVector, 4> CandidateLocations = {{
        Base - Side * 260.0f,
        Base + Side * 260.0f,
        Base - Forward * 280.0f,
        Base + Forward * 280.0f
    }};

    FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(OWGVehicleExit), false, this);
    QueryParams.AddIgnoredActor(Character);

    const auto SafeLocation = OW::FindFirstFreeExitSpot(
        CandidateLocations,
        [this, &QueryParams, Radius, HalfHeight](const FVector& Candidate)
        {
            return GetWorld()->OverlapBlockingTestByChannel(
                Candidate,
                FQuat::Identity,
                ECC_Pawn,
                FCollisionShape::MakeCapsule(Radius, HalfHeight),
                QueryParams);
        });

    if (!SafeLocation.has_value()) return false;

    Character->SetActorLocation(*SafeLocation, false, nullptr, ETeleportType::TeleportPhysics);
    Character->SetActorHiddenInGame(false);
    Character->SetActorEnableCollision(true);
    Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
    PlayerController->Possess(Character);
    if (PlayerController->GetPawn() != Character)
    {
        // Keep the driver attached to the vehicle if possession fails; do not strand the player.
        Character->SetActorHiddenInGame(true);
        Character->SetActorEnableCollision(false);
        Character->GetCharacterMovement()->DisableMovement();
        PlayerController->Possess(this);
        return false;
    }

    DriverCharacter = nullptr;
    return true;
}
