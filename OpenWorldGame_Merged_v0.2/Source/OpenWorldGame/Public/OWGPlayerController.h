#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "OWGPlayerController.generated.h"

class UOWGMainMenuWidget;

UCLASS()
class OPENWORLDGAME_API AOWGPlayerController : public APlayerController
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

private:
    void SaveFromKeyboard();
    void LoadFromKeyboard();
    void TogglePauseFromKeyboard();

    UPROPERTY()
    TObjectPtr<UOWGMainMenuWidget> MainWidget;
};
