#include "SIAPlayerController.h"

#include "Components/AudioComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"
#include "Kismet/KismetSystemLibrary.h"

#include "SIAPlayerPawn.h"
#include "../Widgets/SIAHUD.h"

#include "../Utils/SIAHelpers.h"

ASIAPlayerController::ASIAPlayerController()
{
    MusicAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("MusicAudioComponent"));
}

void ASIAPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (HudWidgetTemplate)
        HUDWidget = CreateWidget<USIAHUD>(this, HudWidgetTemplate);
    else
        USIAHelpers::err("hud_widget_bp is null");

    if (HUDWidget)
        HUDWidget->Show();

    StartNewGame();

    if (MusicWav)
    {
        MusicAudioComponent->SetSound(MusicWav);
        MusicAudioComponent->Play();
    }
}

void ASIAPlayerController::AddMusicPitch(float v)
{
    MusicAudioComponent->SetPitchMultiplier(MusicAudioComponent->PitchMultiplier+v);
}

void ASIAPlayerController::StartNewGame()
{
    if (!HUDWidget)
        return;

    FString path = FPaths::ProjectContentDir() + "runtime/HighScore.txt";
    FString result;
    FFileHelper::LoadFileToString(result, *path);
    HighScore = FCString::Atoi(*result);

    HUDWidget->ShowGameOver(false);

    HUDWidget->SetScore(0);
    HUDWidget->SetHighScore(HighScore);
    HUDWidget->SetLives(3);
}

void ASIAPlayerController::EnemyKilled()
{
    CurrentScore += EnemyKillScore;

    if (HUDWidget)
    {
        HUDWidget->SetScore(CurrentScore);
        if (CurrentScore > HighScore)
        {
            HighScore = CurrentScore;
            HUDWidget->SetHighScore(HighScore);
        }
    }
}

void ASIAPlayerController::UFOKilled()
{
    CurrentScore += UFOKillScore;

    if (HUDWidget)
    {
        HUDWidget->SetScore(CurrentScore);

        if (CurrentScore > HighScore)
        {
            HighScore = CurrentScore;
            HUDWidget->SetHighScore(HighScore);
        }
    }
}

void ASIAPlayerController::PlayerKilled()
{
    CurrentLives -= 1;

    if (HUDWidget)
        HUDWidget->SetLives(CurrentLives);

    if (CurrentLives == 0)
    {
        GameOver();
        //RestartLevel();
    }
    else
    {
        ASIAPlayerPawn* pawn = Cast<ASIAPlayerPawn>(GetPawn());
        if (pawn)
            pawn->ResetPawn();
    }
}

void ASIAPlayerController::GameOver()
{
    FString path = FPaths::ProjectContentDir() + "runtime/HighScore.txt";
    FString result(FString::FromInt(HighScore));
    FFileHelper::SaveStringToFile(result, *path);

    if (GameOverWav)
        UGameplayStatics::PlaySound2D(GetWorld(), GameOverWav);

    if (HUDWidget)
    {
        HUDWidget->ShowGameOver(true);
        ShowMouseCursor(true);
    }
    Pause();
}

void ASIAPlayerController::ShowMouseCursor(bool show)
{
    bShowMouseCursor = show;
    bEnableClickEvents = show;
    bEnableMouseOverEvents = show;
}

void ASIAPlayerController::YesClicked()
{
    RestartLevel();
}


void ASIAPlayerController::NoClicked()
{
    UKismetSystemLibrary::QuitGame(GetWorld(),this, EQuitPreference::Quit,true);
}