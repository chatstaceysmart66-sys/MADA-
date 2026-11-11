#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OWGGameMode.generated.h"

UCLASS()
class OPENWORLDGAME_API AOWGGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AOWGGameMode();
    virtual void BeginPlay() override;
};
