#include "OWGPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "OWGGameInstance.h"
#include "OWGMainMenuWidget.h"

void AOWGPlayerController::BeginPlay()
{
    Super::BeginPlay();

    MainWidget = CreateWidget<UOWGMainMenuWidget>(this, UOWGMainMenuWidget::StaticClass());
    if (MainWidget)
    {
        MainWidget->AddToViewport(100);
        SetPause(true, FCanUnpause::CreateUObject(this, &APlayerController::DefaultCanUnpause));
        SetInputMode(FInputModeUIOnly());
        bShowMouseCursor = true;
    }
}

void AOWGPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    if (!InputComponent) return;

    InputComponent->bExecuteWhenPaused = true;
    InputComponent->BindAction(TEXT("SaveGame"), IE_Pressed, this, &AOWGPlayerController::SaveFromKeyboard);
    InputComponent->BindAction(TEXT("LoadGame"), IE_Pressed, this, &AOWGPlayerController::LoadFromKeyboard);
    InputComponent->BindAction(TEXT("PauseMenu"), IE_Pressed, this, &AOWGPlayerController::TogglePauseFromKeyboard);
}

void AOWGPlayerController::SaveFromKeyboard()
{
    if (UOWGGameInstance* GameInstance = Cast<UOWGGameInstance>(GetGameInstance()))
    {
        const bool bSaved = GameInstance->SaveProgress();
        if (MainWidget)
        {
            MainWidget->SetStatusMessage(FText::FromString(bSaved ? TEXT("تم حفظ التقدم محلياً") : TEXT("تعذر حفظ التقدم")));
        }
    }
}

void AOWGPlayerController::LoadFromKeyboard()
{
    if (UOWGGameInstance* GameInstance = Cast<UOWGGameInstance>(GetGameInstance()))
    {
        const bool bLoaded = GameInstance->LoadProgress();
        if (MainWidget)
        {
            MainWidget->SetStatusMessage(FText::FromString(bLoaded ? TEXT("تم تحميل الحفظ المحلي") : TEXT("لا توجد حفظة صالحة")));
        }
    }
}

void AOWGPlayerController::TogglePauseFromKeyboard()
{
    if (MainWidget)
    {
        MainWidget->TogglePauseMenu();
    }
}
