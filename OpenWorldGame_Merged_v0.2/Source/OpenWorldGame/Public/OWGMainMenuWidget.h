#pragma once

#include "CoreMinimal.h"
#include "Core/OWFrameStats.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/Layout/Anchors.h"
#include "OWGMainMenuWidget.generated.h"

class UCanvasPanel;
class UBorder;
class UVerticalBox;
class UButton;
class UTextBlock;

UCLASS()
class OPENWORLDGAME_API UOWGMainMenuWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
    void SetStatusMessage(const FText& Message);
    void TogglePauseMenu();

private:
    UButton* AddMenuButton(const FText& Label, const FName& Name);
    UButton* AddTouchButton(const FText& Label, const FVector2D& Position, const FVector2D& Size, const FName& Name, const FAnchors& Anchors = FAnchors(1.0f, 1.0f), const FVector2D& Alignment = FVector2D(1.0f, 1.0f));
    void SyncFrameCapSelection();
    void RefreshSupportedFrameCaps();
    void UpdateFrameCapLabel();
    void BeginNewGameTransition();

    UFUNCTION()
    void HandleStartupContinueClicked();
    UFUNCTION()
    void HandlePauseClicked();
    UFUNCTION()
    void HandleResumeClicked();
    UFUNCTION()
    void HandleNewGameClicked();
    UFUNCTION()
    void HandleNewGameConfirmed();
    UFUNCTION()
    void HandleNewGameCancelled();
    UFUNCTION()
    void HandleStartClicked();
    UFUNCTION()
    void HandleContinueClicked();
    UFUNCTION()
    void HandleFrameCapClicked();
    UFUNCTION()
    void HandleSaveClicked();
    UFUNCTION()
    void HandleFootInteractClicked();
    UFUNCTION()
    void HandleVehicleExitClicked();
    UFUNCTION()
    void HandleJumpPressed();
    UFUNCTION()
    void HandleJumpReleased();
    UFUNCTION()
    void HandleSprintPressed();
    UFUNCTION()
    void HandleSprintReleased();
    UFUNCTION()
    void HandleThrottlePressed();
    UFUNCTION()
    void HandleThrottleReleased();
    UFUNCTION()
    void HandleBrakePressed();
    UFUNCTION()
    void HandleBrakeReleased();
    UFUNCTION()
    void HandleSteerLeftPressed();
    UFUNCTION()
    void HandleSteerLeftReleased();
    UFUNCTION()
    void HandleSteerRightPressed();
    UFUNCTION()
    void HandleSteerRightReleased();

    UPROPERTY()
    TObjectPtr<UCanvasPanel> RootCanvas;
    UPROPERTY()
    TObjectPtr<UBorder> StartupBackdrop;
    UPROPERTY()
    TObjectPtr<UVerticalBox> StartupPanel;
    UPROPERTY()
    TObjectPtr<UVerticalBox> MainMenuPanel;
    UPROPERTY()
    TObjectPtr<UVerticalBox> PausePanel;
    UPROPERTY()
    TObjectPtr<UVerticalBox> NewGameConfirmationPanel;
    UPROPERTY()
    TObjectPtr<UTextBlock> StatusText;
    UPROPERTY()
    TObjectPtr<UTextBlock> FPSLabel;
    UPROPERTY()
    TObjectPtr<UTextBlock> SpeedLabel;
    UPROPERTY()
    TObjectPtr<UTextBlock> FrameCapLabel;

    UPROPERTY()
    TObjectPtr<UButton> FootInteractButton;
    UPROPERTY()
    TObjectPtr<UButton> JumpButton;
    UPROPERTY()
    TObjectPtr<UButton> SprintButton;
    UPROPERTY()
    TObjectPtr<UButton> ThrottleButton;
    UPROPERTY()
    TObjectPtr<UButton> BrakeButton;
    UPROPERTY()
    TObjectPtr<UButton> SteerLeftButton;
    UPROPERTY()
    TObjectPtr<UButton> SteerRightButton;
    UPROPERTY()
    TObjectPtr<UButton> ExitVehicleButton;
    UPROPERTY()
    TObjectPtr<UButton> SaveButton;
    UPROPERTY()
    TObjectPtr<UButton> PauseButton;

    bool bGameStarted = false;
    bool bPauseMenuVisible = false;
    int32 FrameCapIndex = 0;
    TArray<int32> SupportedFrameCaps;
    OW::FrameStats FPSStats{1.0};
    float FPSLabelUpdateTime = 0.0f;
};
