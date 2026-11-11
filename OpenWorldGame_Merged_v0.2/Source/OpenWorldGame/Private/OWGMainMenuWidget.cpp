#include "OWGMainMenuWidget.h"

#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Blueprint/WidgetTree.h"
#include "Engine/Engine.h"
#include "GameFramework/GameUserSettings.h"
#include "GameFramework/PlayerController.h"
#include "Core/OWCredits.h"
#include "OWGCharacter.h"
#include "OWGGameInstance.h"
#include "OWGVehiclePawn.h"

void UOWGMainMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    RootCanvas = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("RootCanvas"));
    WidgetTree->RootWidget = RootCanvas;

    StartupBackdrop = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("StartupBackdrop"));
    UCanvasPanelSlot* BackdropSlot = RootCanvas->AddChildToCanvas(StartupBackdrop);
    BackdropSlot->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
    BackdropSlot->SetOffsets(FMargin(0.0f));
    StartupBackdrop->SetBrushColor(FLinearColor(0.025f, 0.055f, 0.065f, 0.94f));

    StartupPanel = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("StartupPanel"));
    UCanvasPanelSlot* StartupSlot = RootCanvas->AddChildToCanvas(StartupPanel);
    StartupSlot->SetAnchors(FAnchors(0.5f, 0.5f));
    StartupSlot->SetAlignment(FVector2D(0.5f, 0.5f));
    StartupSlot->SetAutoSize(true);

    UTextBlock* StartupTitle = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("StartupTitle"));
    StartupTitle->SetText(FText::FromString(UTF8_TO_TCHAR(OW::GameNameEnglishUtf8)));
    StartupTitle->SetJustification(ETextJustify::Center);
    StartupTitle->SetColorAndOpacity(FSlateColor(FLinearColor(0.88f, 0.70f, 0.38f, 1.0f)));
    StartupPanel->AddChildToVerticalBox(StartupTitle);

    UTextBlock* StartupArabicTitle = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("StartupArabicTitle"));
    StartupArabicTitle->SetText(FText::FromString(UTF8_TO_TCHAR(OW::GameNameArabicUtf8)));
    StartupArabicTitle->SetJustification(ETextJustify::Center);
    StartupPanel->AddChildToVerticalBox(StartupArabicTitle);

    UTextBlock* StartupTagline = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("StartupTagline"));
    StartupTagline->SetText(FText::FromString(UTF8_TO_TCHAR(OW::GameTaglineUtf8)));
    StartupTagline->SetJustification(ETextJustify::Center);
    StartupPanel->AddChildToVerticalBox(StartupTagline);

    UTextBlock* StartupCredit = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("StartupDeveloperCredit"));
    StartupCredit->SetText(FText::FromString(UTF8_TO_TCHAR(OW::DeveloperCreditUtf8)));
    StartupCredit->SetJustification(ETextJustify::Center);
    StartupPanel->AddChildToVerticalBox(StartupCredit);

    UButton* StartupContinueButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("StartupContinueButton"));
    UTextBlock* StartupContinueText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
    StartupContinueText->SetText(FText::FromString(TEXT("متابعة")));
    StartupContinueText->SetJustification(ETextJustify::Center);
    StartupContinueButton->AddChild(StartupContinueText);
    StartupPanel->AddChildToVerticalBox(StartupContinueButton);
    StartupContinueButton->OnClicked.AddDynamic(this, &UOWGMainMenuWidget::HandleStartupContinueClicked);

    MainMenuPanel = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("MainMenuPanel"));
    UCanvasPanelSlot* MenuSlot = RootCanvas->AddChildToCanvas(MainMenuPanel);
    MenuSlot->SetAnchors(FAnchors(0.5f, 0.5f));
    MenuSlot->SetAlignment(FVector2D(0.5f, 0.5f));
    MenuSlot->SetPosition(FVector2D(0.0f, 0.0f));
    MenuSlot->SetAutoSize(true);

    UTextBlock* Title = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("GameTitle"));
    Title->SetText(FText::FromString(UTF8_TO_TCHAR(OW::GameNameEnglishUtf8)));
    Title->SetJustification(ETextJustify::Center);
    MainMenuPanel->AddChildToVerticalBox(Title);

    UTextBlock* ArabicTitle = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("ArabicGameTitle"));
    ArabicTitle->SetText(FText::FromString(UTF8_TO_TCHAR(OW::GameNameArabicUtf8)));
    ArabicTitle->SetJustification(ETextJustify::Center);
    MainMenuPanel->AddChildToVerticalBox(ArabicTitle);

    UTextBlock* Tagline = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("GameTagline"));
    Tagline->SetText(FText::FromString(UTF8_TO_TCHAR(OW::GameTaglineUtf8)));
    Tagline->SetJustification(ETextJustify::Center);
    Tagline->SetColorAndOpacity(FSlateColor(FLinearColor(0.72f, 0.82f, 0.81f, 1.0f)));
    MainMenuPanel->AddChildToVerticalBox(Tagline);

    UTextBlock* Credit = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("DeveloperCredit"));
    Credit->SetText(FText::FromString(UTF8_TO_TCHAR(OW::DeveloperCreditUtf8)));
    Credit->SetJustification(ETextJustify::Center);
    MainMenuPanel->AddChildToVerticalBox(Credit);

    UButton* StartButton = AddMenuButton(FText::FromString(TEXT("بدء التجربة")), TEXT("StartButton"));
    StartButton->OnClicked.AddDynamic(this, &UOWGMainMenuWidget::HandleStartClicked);

    UButton* ContinueButton = AddMenuButton(FText::FromString(TEXT("متابعة الحفظ")), TEXT("ContinueButton"));
    ContinueButton->OnClicked.AddDynamic(this, &UOWGMainMenuWidget::HandleContinueClicked);

    UButton* FrameCapButton = AddMenuButton(FText::FromString(TEXT("حد الإطارات")), TEXT("FrameCapButton"));
    FrameCapButton->OnClicked.AddDynamic(this, &UOWGMainMenuWidget::HandleFrameCapClicked);

    FrameCapLabel = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("FrameCapLabel"));
    FrameCapLabel->SetJustification(ETextJustify::Center);
    MainMenuPanel->AddChildToVerticalBox(FrameCapLabel);
    SyncFrameCapSelection();

    StatusText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("StatusText"));
    StatusText->SetText(FText::FromString(TEXT("نموذج أولي — الأصول الفنية النهائية غير مضافة")));
    UCanvasPanelSlot* StatusSlot = RootCanvas->AddChildToCanvas(StatusText);
    StatusSlot->SetAnchors(FAnchors(0.0f, 1.0f));
    StatusSlot->SetAlignment(FVector2D(0.0f, 1.0f));
    StatusSlot->SetPosition(FVector2D(18.0f, -18.0f));
    StatusSlot->SetAutoSize(true);

    FPSLabel = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("MeasuredFPS"));
    FPSLabel->SetText(FText::FromString(TEXT("FPS المقاس: --")));
    UCanvasPanelSlot* FPSSlot = RootCanvas->AddChildToCanvas(FPSLabel);
    FPSSlot->SetAnchors(FAnchors(0.0f, 0.0f));
    FPSSlot->SetPosition(FVector2D(18.0f, 18.0f));
    FPSSlot->SetAutoSize(true);

    SpeedLabel = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("VehicleSpeed"));
    SpeedLabel->SetText(FText::FromString(TEXT("")));
    UCanvasPanelSlot* SpeedSlot = RootCanvas->AddChildToCanvas(SpeedLabel);
    SpeedSlot->SetAnchors(FAnchors(1.0f, 0.0f));
    SpeedSlot->SetAlignment(FVector2D(1.0f, 0.0f));
    SpeedSlot->SetPosition(FVector2D(-18.0f, 18.0f));
    SpeedSlot->SetAutoSize(true);

    SaveButton = AddTouchButton(FText::FromString(TEXT("حفظ")), FVector2D(-18.0f, 80.0f), FVector2D(82.0f, 52.0f), TEXT("SaveButton"), FAnchors(1.0f, 0.0f), FVector2D(1.0f, 0.0f));
    SaveButton->OnClicked.AddDynamic(this, &UOWGMainMenuWidget::HandleSaveClicked);

    FootInteractButton = AddTouchButton(FText::FromString(TEXT("تفاعل")), FVector2D(-18.0f, -18.0f), FVector2D(92.0f, 64.0f), TEXT("FootInteract"));
    FootInteractButton->OnClicked.AddDynamic(this, &UOWGMainMenuWidget::HandleFootInteractClicked);

    JumpButton = AddTouchButton(FText::FromString(TEXT("قفز")), FVector2D(-120.0f, -18.0f), FVector2D(82.0f, 64.0f), TEXT("Jump"));
    JumpButton->OnPressed.AddDynamic(this, &UOWGMainMenuWidget::HandleJumpPressed);
    JumpButton->OnReleased.AddDynamic(this, &UOWGMainMenuWidget::HandleJumpReleased);

    SprintButton = AddTouchButton(FText::FromString(TEXT("ركض")), FVector2D(-212.0f, -18.0f), FVector2D(82.0f, 64.0f), TEXT("Sprint"));
    SprintButton->OnPressed.AddDynamic(this, &UOWGMainMenuWidget::HandleSprintPressed);
    SprintButton->OnReleased.AddDynamic(this, &UOWGMainMenuWidget::HandleSprintReleased);

    ThrottleButton = AddTouchButton(FText::FromString(TEXT("تسارع")), FVector2D(-18.0f, -18.0f), FVector2D(92.0f, 64.0f), TEXT("Throttle"));
    ThrottleButton->OnPressed.AddDynamic(this, &UOWGMainMenuWidget::HandleThrottlePressed);
    ThrottleButton->OnReleased.AddDynamic(this, &UOWGMainMenuWidget::HandleThrottleReleased);

    BrakeButton = AddTouchButton(FText::FromString(TEXT("فرامل")), FVector2D(-120.0f, -18.0f), FVector2D(82.0f, 64.0f), TEXT("Brake"));
    BrakeButton->OnPressed.AddDynamic(this, &UOWGMainMenuWidget::HandleBrakePressed);
    BrakeButton->OnReleased.AddDynamic(this, &UOWGMainMenuWidget::HandleBrakeReleased);

    SteerLeftButton = AddTouchButton(FText::FromString(TEXT("يسار")), FVector2D(-120.0f, -92.0f), FVector2D(82.0f, 64.0f), TEXT("SteerLeft"));
    SteerLeftButton->OnPressed.AddDynamic(this, &UOWGMainMenuWidget::HandleSteerLeftPressed);
    SteerLeftButton->OnReleased.AddDynamic(this, &UOWGMainMenuWidget::HandleSteerLeftReleased);

    SteerRightButton = AddTouchButton(FText::FromString(TEXT("يمين")), FVector2D(-18.0f, -92.0f), FVector2D(92.0f, 64.0f), TEXT("SteerRight"));
    SteerRightButton->OnPressed.AddDynamic(this, &UOWGMainMenuWidget::HandleSteerRightPressed);
    SteerRightButton->OnReleased.AddDynamic(this, &UOWGMainMenuWidget::HandleSteerRightReleased);

    ExitVehicleButton = AddTouchButton(FText::FromString(TEXT("نزول")), FVector2D(-212.0f, -18.0f), FVector2D(82.0f, 64.0f), TEXT("ExitVehicle"));
    ExitVehicleButton->OnClicked.AddDynamic(this, &UOWGMainMenuWidget::HandleVehicleExitClicked);

    bGameStarted = false;
    StartupPanel->SetVisibility(ESlateVisibility::Visible);
    MainMenuPanel->SetVisibility(ESlateVisibility::Collapsed);
    FootInteractButton->SetVisibility(ESlateVisibility::Collapsed);
    JumpButton->SetVisibility(ESlateVisibility::Collapsed);
    SprintButton->SetVisibility(ESlateVisibility::Collapsed);
    ThrottleButton->SetVisibility(ESlateVisibility::Collapsed);
    BrakeButton->SetVisibility(ESlateVisibility::Collapsed);
    SteerLeftButton->SetVisibility(ESlateVisibility::Collapsed);
    SteerRightButton->SetVisibility(ESlateVisibility::Collapsed);
    ExitVehicleButton->SetVisibility(ESlateVisibility::Collapsed);
    SaveButton->SetVisibility(ESlateVisibility::Collapsed);
}

UButton* UOWGMainMenuWidget::AddMenuButton(const FText& Label, const FName& Name)
{
    UButton* Button = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), Name);
    UTextBlock* Text = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
    Text->SetText(Label);
    Text->SetJustification(ETextJustify::Center);
    Button->AddChild(Text);
    if (UVerticalBoxSlot* Slot = MainMenuPanel->AddChildToVerticalBox(Button))
    {
        Slot->SetPadding(FMargin(8.0f, 7.0f));
        Slot->SetHorizontalAlignment(HAlign_Fill);
    }
    return Button;
}

UButton* UOWGMainMenuWidget::AddTouchButton(const FText& Label, const FVector2D& Position, const FVector2D& Size, const FName& Name, const FAnchors& Anchors, const FVector2D& Alignment)
{
    UButton* Button = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), Name);
    UTextBlock* Text = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
    Text->SetText(Label);
    Text->SetJustification(ETextJustify::Center);
    Button->AddChild(Text);

    UCanvasPanelSlot* Slot = RootCanvas->AddChildToCanvas(Button);
    Slot->SetAnchors(Anchors);
    Slot->SetAlignment(Alignment);
    Slot->SetPosition(Position);
    Slot->SetSize(Size);
    return Button;
}

void UOWGMainMenuWidget::UpdateFrameCapLabel()
{
    static const float FrameCaps[] = { 0.0f, 30.0f, 40.0f, 60.0f };
    const float Cap = FrameCaps[FMath::Clamp(FrameCapIndex, 0, 3)];
    const FString Label = Cap <= 0.0f
        ? TEXT("حد الإطارات المطلوب: تلقائي")
        : FString::Printf(TEXT("حد الإطارات المطلوب: %.0f FPS"), Cap);
    if (FrameCapLabel)
    {
        FrameCapLabel->SetText(FText::FromString(Label));
    }
}

void UOWGMainMenuWidget::SyncFrameCapSelection()
{
    FrameCapIndex = 0;
    if (const UOWGGameInstance* GameInstance = Cast<UOWGGameInstance>(GetGameInstance()))
    {
        const float CurrentLimit = GameInstance->GetFrameRateLimit();
        static const float FrameCaps[] = { 0.0f, 30.0f, 40.0f, 60.0f };
        for (int32 Index = 0; Index < UE_ARRAY_COUNT(FrameCaps); ++Index)
        {
            if (FMath::IsNearlyEqual(CurrentLimit, FrameCaps[Index]))
            {
                FrameCapIndex = Index;
                break;
            }
        }
    }
    UpdateFrameCapLabel();
}

void UOWGMainMenuWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (InDeltaTime > KINDA_SMALL_NUMBER)
    {
        FPSStats.AddFrame(static_cast<double>(InDeltaTime));
        FPSLabelUpdateTime += InDeltaTime;
        if (FPSLabelUpdateTime >= 0.5f && FPSLabel)
        {
            FPSLabelUpdateTime = 0.0f;
            const double MeasuredFPS = FPSStats.MeasuredFps();
            if (MeasuredFPS > 0.0)
            {
                FPSLabel->SetText(FText::FromString(
                    FString::Printf(TEXT("FPS المقاس: %.0f"), static_cast<float>(MeasuredFPS))));
            }
            else
            {
                FPSLabel->SetText(FText::FromString(TEXT("FPS المقاس: جارٍ القياس")));
            }
        }
    }

    if (!bGameStarted || !GetOwningPlayer()) return;

    const bool bDriving = Cast<AOWGVehiclePawn>(GetOwningPlayer()->GetPawn()) != nullptr;
    const ESlateVisibility FootVisibility = bDriving ? ESlateVisibility::Collapsed : ESlateVisibility::Visible;
    const ESlateVisibility VehicleVisibility = bDriving ? ESlateVisibility::Visible : ESlateVisibility::Collapsed;

    FootInteractButton->SetVisibility(FootVisibility);
    JumpButton->SetVisibility(FootVisibility);
    SprintButton->SetVisibility(FootVisibility);
    ThrottleButton->SetVisibility(VehicleVisibility);
    BrakeButton->SetVisibility(VehicleVisibility);
    SteerLeftButton->SetVisibility(VehicleVisibility);
    SteerRightButton->SetVisibility(VehicleVisibility);
    ExitVehicleButton->SetVisibility(VehicleVisibility);
    SaveButton->SetVisibility(ESlateVisibility::Visible);

    if (bDriving)
    {
        const AOWGVehiclePawn* Vehicle = Cast<AOWGVehiclePawn>(GetOwningPlayer()->GetPawn());
        SpeedLabel->SetText(FText::FromString(FString::Printf(TEXT("السرعة: %.0f km/h"), Vehicle ? Vehicle->GetSpeedKmh() : 0.0f)));
    }
    else
    {
        SpeedLabel->SetText(FText::FromString(TEXT("")));
    }
}

void UOWGMainMenuWidget::SetStatusMessage(const FText& Message)
{
    if (StatusText) StatusText->SetText(Message);
}

void UOWGMainMenuWidget::HandleStartupContinueClicked()
{
    StartupPanel->SetVisibility(ESlateVisibility::Collapsed);
    StartupBackdrop->SetVisibility(ESlateVisibility::Collapsed);
    MainMenuPanel->SetVisibility(ESlateVisibility::Visible);
}

void UOWGMainMenuWidget::HandleStartClicked()
{
    bGameStarted = true;
    MainMenuPanel->SetVisibility(ESlateVisibility::Collapsed);
    if (APlayerController* PC = GetOwningPlayer())
    {
        PC->SetPause(false, FCanUnpause::CreateUObject(PC, &APlayerController::DefaultCanUnpause));
        FInputModeGameAndUI InputMode;
        PC->SetInputMode(InputMode);
        PC->bShowMouseCursor = false;
    }
    SetStatusMessage(FText::FromString(TEXT("استكشف المنطقة التجريبية — E للدخول/الخروج، F5 للحفظ")));
}

void UOWGMainMenuWidget::HandleContinueClicked()
{
    bool bLoaded = false;
    if (UOWGGameInstance* GameInstance = Cast<UOWGGameInstance>(GetGameInstance()))
    {
        bLoaded = GameInstance->LoadProgress();
    }
    if (bLoaded) SyncFrameCapSelection();
    SetStatusMessage(FText::FromString(bLoaded ? TEXT("تم تحميل الحفظ المحلي") : TEXT("لم يتم العثور على حفظة — بدأنا تجربة جديدة")));
    HandleStartClicked();
}

void UOWGMainMenuWidget::HandleFrameCapClicked()
{
    FrameCapIndex = (FrameCapIndex + 1) % 4;
    static const float FrameCaps[] = { 0.0f, 30.0f, 40.0f, 60.0f };
    const float NewLimit = FrameCaps[FrameCapIndex];

    if (UOWGGameInstance* GameInstance = Cast<UOWGGameInstance>(GetGameInstance()))
    {
        GameInstance->ApplyFrameRateLimit(NewLimit);
    }

    UpdateFrameCapLabel();
    SetStatusMessage(FText::FromString(TEXT("تم تطبيق حد الإطارات المطلوب. الرقم المقاس مستقل وقد يكون أقل.")));
}

void UOWGMainMenuWidget::HandleSaveClicked()
{
    if (UOWGGameInstance* GameInstance = Cast<UOWGGameInstance>(GetGameInstance()))
    {
        const bool bSaved = GameInstance->SaveProgress();
        SetStatusMessage(FText::FromString(bSaved ? TEXT("تم حفظ التقدم محلياً") : TEXT("فشل الحفظ المحلي")));
    }
}

void UOWGMainMenuWidget::HandleFootInteractClicked()
{
    if (AOWGCharacter* Character = Cast<AOWGCharacter>(GetOwningPlayerPawn()))
    {
        Character->TryEnterNearbyVehicle();
    }
}

void UOWGMainMenuWidget::HandleVehicleExitClicked()
{
    if (AOWGVehiclePawn* Vehicle = Cast<AOWGVehiclePawn>(GetOwningPlayerPawn()))
    {
        if (!Vehicle->TryExitVehicle())
        {
            SetStatusMessage(FText::FromString(TEXT("لا توجد مساحة آمنة للنزول بجانب السيارة")));
        }
    }
}

void UOWGMainMenuWidget::HandleJumpPressed()
{
    if (AOWGCharacter* Character = Cast<AOWGCharacter>(GetOwningPlayerPawn())) Character->Jump();
}
void UOWGMainMenuWidget::HandleJumpReleased()
{
    if (AOWGCharacter* Character = Cast<AOWGCharacter>(GetOwningPlayerPawn())) Character->StopJumping();
}
void UOWGMainMenuWidget::HandleSprintPressed()
{
    if (AOWGCharacter* Character = Cast<AOWGCharacter>(GetOwningPlayerPawn())) Character->SetSprintHeld(true);
}
void UOWGMainMenuWidget::HandleSprintReleased()
{
    if (AOWGCharacter* Character = Cast<AOWGCharacter>(GetOwningPlayerPawn())) Character->SetSprintHeld(false);
}
void UOWGMainMenuWidget::HandleThrottlePressed()
{
    if (AOWGVehiclePawn* Vehicle = Cast<AOWGVehiclePawn>(GetOwningPlayerPawn())) Vehicle->SetThrottleInput(1.0f);
}
void UOWGMainMenuWidget::HandleThrottleReleased()
{
    if (AOWGVehiclePawn* Vehicle = Cast<AOWGVehiclePawn>(GetOwningPlayerPawn())) Vehicle->SetThrottleInput(0.0f);
}
void UOWGMainMenuWidget::HandleBrakePressed()
{
    if (AOWGVehiclePawn* Vehicle = Cast<AOWGVehiclePawn>(GetOwningPlayerPawn())) Vehicle->SetBrakeInput(1.0f);
}
void UOWGMainMenuWidget::HandleBrakeReleased()
{
    if (AOWGVehiclePawn* Vehicle = Cast<AOWGVehiclePawn>(GetOwningPlayerPawn())) Vehicle->SetBrakeInput(0.0f);
}
void UOWGMainMenuWidget::HandleSteerLeftPressed()
{
    if (AOWGVehiclePawn* Vehicle = Cast<AOWGVehiclePawn>(GetOwningPlayerPawn())) Vehicle->SetSteeringInput(-1.0f);
}
void UOWGMainMenuWidget::HandleSteerLeftReleased()
{
    if (AOWGVehiclePawn* Vehicle = Cast<AOWGVehiclePawn>(GetOwningPlayerPawn())) Vehicle->SetSteeringInput(0.0f);
}
void UOWGMainMenuWidget::HandleSteerRightPressed()
{
    if (AOWGVehiclePawn* Vehicle = Cast<AOWGVehiclePawn>(GetOwningPlayerPawn())) Vehicle->SetSteeringInput(1.0f);
}
void UOWGMainMenuWidget::HandleSteerRightReleased()
{
    if (AOWGVehiclePawn* Vehicle = Cast<AOWGVehiclePawn>(GetOwningPlayerPawn())) Vehicle->SetSteeringInput(0.0f);
}
