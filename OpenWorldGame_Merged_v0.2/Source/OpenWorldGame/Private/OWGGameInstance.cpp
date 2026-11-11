#include "OWGGameInstance.h"

#include "Engine/Engine.h"
#include "GameFramework/GameUserSettings.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "OWGCharacter.h"
#include "OWGSaveGame.h"
#include "OWGVehiclePawn.h"

const FString UOWGGameInstance::SaveSlotName = TEXT("OWG_Local_Save_01");

bool UOWGGameInstance::StartNewGame()
{
    const FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this, true);
    if (CurrentLevelName.IsEmpty()) return false;

    if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
    {
        if (!UGameplayStatics::DeleteGameInSlot(SaveSlotName, 0)) return false;
    }

    bStartNewGameAfterTravel = true;
    UGameplayStatics::OpenLevel(this, FName(*CurrentLevelName));
    return true;
}

bool UOWGGameInstance::ConsumeStartNewGameAfterTravel()
{
    const bool bShouldStart = bStartNewGameAfterTravel;
    bStartNewGameAfterTravel = false;
    return bShouldStart;
}

bool UOWGGameInstance::SaveProgress()
{
    UOWGSaveGame* SaveObject = Cast<UOWGSaveGame>(UGameplayStatics::CreateSaveGameObject(UOWGSaveGame::StaticClass()));
    if (!SaveObject) return false;

    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    if (!PlayerController) return false;

    AOWGCharacter* Character = Cast<AOWGCharacter>(PlayerController->GetPawn());
    AOWGVehiclePawn* ControlledVehicle = Cast<AOWGVehiclePawn>(PlayerController->GetPawn());

    if (ControlledVehicle)
    {
        Character = ControlledVehicle->GetDriverCharacter();
        SaveObject->bWasDriving = true;
        SaveObject->VehicleTransform = ControlledVehicle->GetActorTransform();
        SaveObject->bHasVehicleTransform = true;
    }

    if (Character)
    {
        SaveObject->PlayerTransform = Character->GetActorTransform();
    }

    TArray<AActor*> Vehicles;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AOWGVehiclePawn::StaticClass(), Vehicles);
    if (!SaveObject->bHasVehicleTransform && Vehicles.Num() > 0)
    {
        if (const AOWGVehiclePawn* Vehicle = Cast<AOWGVehiclePawn>(Vehicles[0]))
        {
            SaveObject->VehicleTransform = Vehicle->GetActorTransform();
            SaveObject->bHasVehicleTransform = true;
        }
    }

    SaveObject->FrameRateLimit = GetFrameRateLimit();
    const bool bSaved = UGameplayStatics::SaveGameToSlot(SaveObject, SaveSlotName, 0);
    UE_LOG(LogTemp, Log, TEXT("[OWG] Local save %s"), bSaved ? TEXT("written") : TEXT("failed"));
    return bSaved;
}

bool UOWGGameInstance::LoadProgress()
{
    if (!UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0)) return false;

    UOWGSaveGame* SaveObject = Cast<UOWGSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
    if (!SaveObject) return false;

    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    if (!PlayerController) return false;

    AOWGCharacter* Character = Cast<AOWGCharacter>(PlayerController->GetPawn());
    AOWGVehiclePawn* ControlledVehicle = Cast<AOWGVehiclePawn>(PlayerController->GetPawn());
    if (ControlledVehicle)
    {
        Character = ControlledVehicle->GetDriverCharacter();
    }

    TArray<AActor*> Vehicles;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AOWGVehiclePawn::StaticClass(), Vehicles);
    AOWGVehiclePawn* Vehicle = Vehicles.Num() > 0 ? Cast<AOWGVehiclePawn>(Vehicles[0]) : nullptr;

    if (Character)
    {
        Character->SetActorTransform(SaveObject->PlayerTransform, false, nullptr, ETeleportType::TeleportPhysics);
    }

    if (Vehicle && SaveObject->bHasVehicleTransform)
    {
        Vehicle->SetActorTransform(SaveObject->VehicleTransform, false, nullptr, ETeleportType::TeleportPhysics);
    }

    if (SaveObject->bWasDriving && Character && Vehicle && PlayerController->GetPawn() != Vehicle)
    {
        Vehicle->EnterVehicle(Character);
    }

    ApplyFrameRateLimit(SaveObject->FrameRateLimit);
    return true;
}

void UOWGGameInstance::ApplyFrameRateLimit(float NewLimit)
{
    if (!GEngine) return;

    if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
    {
        Settings->SetFrameRateLimit(FMath::Max(0.0f, NewLimit));
        Settings->ApplySettings(false);
        Settings->SaveSettings();
    }
}

float UOWGGameInstance::GetFrameRateLimit() const
{
    if (!GEngine) return 0.0f;
    if (const UGameUserSettings* Settings = GEngine->GetGameUserSettings())
    {
        return Settings->GetFrameRateLimit();
    }
    return 0.0f;
}
