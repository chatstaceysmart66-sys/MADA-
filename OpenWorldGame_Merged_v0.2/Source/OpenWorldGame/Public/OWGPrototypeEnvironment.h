#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OWGPrototypeEnvironment.generated.h"

class UStaticMeshComponent;
class USceneComponent;

UCLASS()
class OPENWORLDGAME_API AOWGPrototypeEnvironment : public AActor
{
    GENERATED_BODY()

public:
    AOWGPrototypeEnvironment();

private:
    UPROPERTY(VisibleAnywhere, Category="Prototype")
    TObjectPtr<USceneComponent> SceneRoot;

    UPROPERTY(VisibleAnywhere, Category="Prototype")
    TObjectPtr<UStaticMeshComponent> Ground;

    UPROPERTY(VisibleAnywhere, Category="Prototype")
    TObjectPtr<UStaticMeshComponent> MainRoad;

    UPROPERTY(VisibleAnywhere, Category="Prototype")
    TObjectPtr<UStaticMeshComponent> BuildingA;

    UPROPERTY(VisibleAnywhere, Category="Prototype")
    TObjectPtr<UStaticMeshComponent> BuildingB;

    UPROPERTY(VisibleAnywhere, Category="Prototype")
    TObjectPtr<UStaticMeshComponent> BuildingC;
};
