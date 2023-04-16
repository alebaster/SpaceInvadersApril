#include "SIAHUD.h"

#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Components/Image.h"
#include "Runtime/UMG/Public/Components/Overlay.h"
#include "Runtime/UMG/Public/Components/ProgressBar.h"

#include "../Player/SIAPlayerController.h"

#include "../Utils/SIAHelpers.h"

bool USIAHUD::Initialize()
{
    Super::Initialize();

    if (!ScoreText)
        USIAHelpers::err("ScoreText not binded");

    if (!HighScoreText)
        USIAHelpers::err("HighScoreText not binded");

    if (!LivesText)
        USIAHelpers::err("LivesText not binded");

    if (YesButton)
        YesButton->OnClicked.AddDynamic(this, &USIAHUD::ClickYesButton);

    if (NoButton)
        NoButton->OnClicked.AddDynamic(this, &USIAHUD::ClickNoButton);

    return true;
}

void USIAHUD::Show()
{
    AddToViewport(0);

}

void USIAHUD::Hide()
{
    //RemoveFromViewport();
    RemoveFromParent();
}

void USIAHUD::SetScore(int v)
{
    if (ScoreText)
        ScoreText->SetText(FText::FromString(FString::FromInt(v)));
}

void USIAHUD::SetHighScore(int v)
{
    if (HighScoreText)
        HighScoreText->SetText(FText::FromString(FString::FromInt(v)));
}

void USIAHUD::SetLives(int v)
{
    if (LivesText)
        LivesText->SetText(FText::FromString(FString::FromInt(v)));
}

void USIAHUD::ShowGameOver(bool b)
{
    if (GameOverBox)
        GameOverBox->SetVisibility(b? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void USIAHUD::ClickYesButton()
{
    ASIAPlayerController* pc = Cast<ASIAPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    if (pc)
        pc->YesClicked();
}

void USIAHUD::ClickNoButton()
{
    ASIAPlayerController* pc = Cast<ASIAPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    if (pc)
        pc->NoClicked();
}