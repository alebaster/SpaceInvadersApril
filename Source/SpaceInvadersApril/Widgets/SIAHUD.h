#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"



#include "SIAHUD.generated.h"

UCLASS()
class SPACEINVADERSAPRIL_API USIAHUD : public UUserWidget
{
	GENERATED_BODY()

public:
    void Show();
    void Hide();

    void SetScore(int v);
    void SetHighScore(int v);
    void SetLives(int v);

    void ShowGameOver(bool b);
	
protected:
    virtual bool Initialize() override;

private:
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ScoreText = 0;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* HighScoreText = 0;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* LivesText = 0;

    UPROPERTY(meta = (BindWidget))
    class UVerticalBox* GameOverBox = 0;

    UPROPERTY(meta = (BindWidget))
    class UButton* YesButton = 0;

    UPROPERTY(meta = (BindWidget))
    class UButton* NoButton = 0;

    UFUNCTION()
    void ClickYesButton();

    UFUNCTION()
    void ClickNoButton();
};
