#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "OWGVehiclePawn.generated.h"

class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class AOWGCharacter;

UCLASS()
class OPENWORLDGAME_API AOWGVehiclePawn : public APawn
{
    GENERATED_BODY()

public:
    AOWGVehiclePawn();
    virtual void Tick(float DeltaSeconds) override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    bool EnterVehicle(AOWGCharacter* Character);
    bool TryExitVehicle();
    AOWGCharacter* GetDriverCharacter() const { return DriverCharacter; }
    float GetSpeedKmh() const;

    void SetThrottleInput(float Value);
    void SetSteeringInput(float Value);
    void SetBrakeInput(float Value);

protected:
    virtual void BeginPlay() override;

private:
    void ThrottleAxis(float Value);
    void SteeringAxis(float Value);
    void BrakeAxis(float Value);
    void Interact();
    void TurnCamera(float Value);
    void LookCamera(float Value);

    UPROPERTY(VisibleAnywhere, Category="Vehicle")
    TObjectPtr<UStaticMeshComponent> VehicleBody;

    UPROPERTY(VisibleAnywhere, Category="Vehicle|Camera")
    TObjectPtr<USpringArmComponent> CameraBoom;

    UPROPERTY(VisibleAnywhere, Category="Vehicle|Camera")
    TObjectPtr<UCameraComponent> FollowCamera;

    UPROPERTY(VisibleAnywhere, Category="Vehicle|Visual")
    TArray<TObjectPtr<UStaticMeshComponent>> WheelVisuals;

    UPROPERTY()
    TObjectPtr<AOWGCharacter> DriverCharacter;

    UPROPERTY(EditDefaultsOnly, Category="Vehicle|Handling", meta=(ClampMin="10000.0"))
    float EngineForce = 850000.0f;

    UPROPERTY(EditDefaultsOnly, Category="Vehicle|Handling", meta=(ClampMin="10000.0"))
    float BrakeForce = 1150000.0f;

    UPROPERTY(EditDefaultsOnly, Category="Vehicle|Handling", meta=(ClampMin="100000.0"))
    float SteeringTorque = 150000000.0f;

    float ThrottleInput = 0.0f;
    float SteeringInput = 0.0f;
    float BrakeInput = 0.0f;
};
