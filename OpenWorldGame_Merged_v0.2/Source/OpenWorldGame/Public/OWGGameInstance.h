#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OWGGameInstance.generated.h"

UCLASS()
class OPENWORLDGAME_API UOWGGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    bool StartNewGame();
    bool ConsumeStartNewGameAfterTravel();

    UFUNCTION(BlueprintCallable, Category="Save")
    bool SaveProgress();

    UFUNCTION(BlueprintCallable, Category="Save")
    bool LoadProgress();

    UFUNCTION(BlueprintCallable, Category="Settings")
    void ApplyFrameRateLimit(float NewLimit);

    UFUNCTION(BlueprintPure, Category="Settings")
    float GetFrameRateLimit() const;

private:
    static const FString SaveSlotName;
    bool bStartNewGameAfterTravel = false;
};
