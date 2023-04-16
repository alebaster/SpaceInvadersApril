#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "SIAEnemyWave.generated.h"

UCLASS()
class SPACEINVADERSAPRIL_API ASIAEnemyWave : public AActor
{
	GENERATED_BODY()
	
public:	
	ASIAEnemyWave();

    void SpawnNewWave();

    void DestroyPawn(int row, int col);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere)
    TSubclassOf<class ASIAMovablePawn> UFOTemplate; // from top to bottom

    UPROPERTY(EditDefaultsOnly, Category = "Sound")
    class USoundWave* UFOSound = 0;

    UPROPERTY(EditAnywhere)
    int UFOCooldown = 10;

    UPROPERTY(EditAnywhere)
    float UFORightStep = 1;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class ASIAEnemyPawn>> EnemyTemplate; // from top to bottom

	UPROPERTY(EditAnywhere)
	int EnemiesInRow = 5;

    UPROPERTY(EditAnywhere)
    int NewWaveShift = 10;

    UPROPERTY(EditAnywhere)
    int UFOShift = 50;

    UPROPERTY(EditAnywhere)
    float MovementRightStep = 0.1f;

    UPROPERTY(EditAnywhere)
    int MovementUpStep = 10;

    UPROPERTY(EditAnywhere)
    float MoveCooldown = 0;

    UPROPERTY(EditAnywhere)
    double FireCooldown = 1;

private:
	void Spawn();

    class ASIAMovablePawn* UFO = 0;

	TArray<TArray<class ASIAEnemyPawn*>> Enemies;

    void CheckAndMove();
    void CheckAndFire();
    void CheckAndSpawnUFO();

    class ASIAEnemyPawn* GetLeftmost();
    class ASIAEnemyPawn* GetRightmost();
    class ASIAEnemyPawn* GetLowest();

    class ASIAEnemyPawn* GetShooter();

    float lowest_z = 0;

    int WaveCount = 0;

    char movement_dir = 0; // 0 - right, 1 - left
    double move_timestamp = 0;
    double fire_timestamp = 0;
    double ufo_timestamp = 0;

    int killed_pawns = 0;

    //float music_pitch = 1.0f;
};
