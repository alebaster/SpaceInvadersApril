#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"



#include "SIAPlayerController.generated.h"

UCLASS()
class SPACEINVADERSAPRIL_API ASIAPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    ASIAPlayerController();

    void StartNewGame();

    void AddMusicPitch(float v);

    void EnemyKilled();
    void UFOKilled();
    void PlayerKilled();

    void GameOver();

    void YesClicked();
    void NoClicked();

    UPROPERTY(EditAnywhere)
    int EnemyKillScore = 10;

    UPROPERTY(EditAnywhere)
    int UFOKillScore = 100;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UUserWidget> HudWidgetTemplate = 0;
    UPROPERTY()
    class USIAHUD* HUDWidget = 0;

    UPROPERTY(EditAnywhere)
    class UAudioComponent* MusicAudioComponent = 0;

    UPROPERTY(EditDefaultsOnly, Category = "Sound")
    class USoundWave* MusicWav = 0;

    UPROPERTY(EditDefaultsOnly, Category = "Sound")
    class USoundWave* GameOverWav = 0;

protected:
    virtual void BeginPlay() override;

private:
    void ShowMouseCursor(bool show);

    float MusicPitch = 1.0f;

    int CurrentScore = 0;
    int HighScore = 0;

    int CurrentLives = 3;
};
