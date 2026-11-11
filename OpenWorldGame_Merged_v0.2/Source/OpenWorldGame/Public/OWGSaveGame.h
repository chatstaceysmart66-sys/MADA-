#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "OWGSaveGame.generated.h"

UCLASS()
class OPENWORLDGAME_API UOWGSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UPROPERTY(VisibleAnywhere, Category="Save")
    FTransform PlayerTransform = FTransform::Identity;

    UPROPERTY(VisibleAnywhere, Category="Save")
    FTransform VehicleTransform = FTransform::Identity;

    UPROPERTY(VisibleAnywhere, Category="Save")
    bool bHasVehicleTransform = false;

    UPROPERTY(VisibleAnywhere, Category="Save")
    bool bWasDriving = false;

    UPROPERTY(VisibleAnywhere, Category="Save")
    float FrameRateLimit = 0.0f;
};
