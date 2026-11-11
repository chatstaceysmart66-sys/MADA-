#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "OWGCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class OPENWORLDGAME_API AOWGCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AOWGCharacter();

    void SetSprintHeld(bool bHeld);
    void TryEnterNearbyVehicle();

protected:
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    void MoveForward(float Value);
    void MoveRight(float Value);
    void TurnCamera(float Value);
    void LookCamera(float Value);
    void StartSprint();
    void StopSprint();
    void Interact();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
    TObjectPtr<USpringArmComponent> CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
    TObjectPtr<UCameraComponent> FollowCamera;

    UPROPERTY(EditDefaultsOnly, Category="Movement", meta=(ClampMin="100.0"))
    float BaseWalkSpeed = 360.0f;

    UPROPERTY(EditDefaultsOnly, Category="Movement", meta=(ClampMin="100.0"))
    float SprintWalkSpeed = 620.0f;
};
