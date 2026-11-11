#include "OWGGameMode.h"

#include "Engine/World.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "OWGCharacter.h"
#include "OWGPlayerController.h"
#include "OWGPrototypeEnvironment.h"
#include "OWGVehiclePawn.h"

AOWGGameMode::AOWGGameMode()
{
    DefaultPawnClass = AOWGCharacter::StaticClass();
    PlayerControllerClass = AOWGPlayerController::StaticClass();
    bStartPlayersAsSpectators = false;
}

void AOWGGameMode::BeginPlay()
{
    Super::BeginPlay();

    if (!GetWorld()) return;

    if (UGameplayStatics::GetActorOfClass(GetWorld(), AOWGPrototypeEnvironment::StaticClass()) == nullptr)
    {
        GetWorld()->SpawnActor<AOWGPrototypeEnvironment>(AOWGPrototypeEnvironment::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
    }

    if (UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass()) == nullptr)
    {
        GetWorld()->SpawnActor<APlayerStart>(APlayerStart::StaticClass(), FVector(-250.0f, 0.0f, 60.0f), FRotator::ZeroRotator);
    }

    if (UGameplayStatics::GetActorOfClass(GetWorld(), AOWGVehiclePawn::StaticClass()) == nullptr)
    {
        GetWorld()->SpawnActor<AOWGVehiclePawn>(AOWGVehiclePawn::StaticClass(), FVector(300.0f, 0.0f, 5.0f), FRotator::ZeroRotator);
    }
}
