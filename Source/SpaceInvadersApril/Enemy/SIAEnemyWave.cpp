#include "SIAEnemyWave.h"

#include "../Pawns/SIAEnemyPawn.h"
#include "../Player/SIAPlayerController.h"
#include "../Actors/SIABunkerActor.h"

#include "Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

#include "../Utils/SIAHelpers.h"

ASIAEnemyWave::ASIAEnemyWave()
{
	PrimaryActorTick.bCanEverTick = true;

	//TSubclassOf<ABulletProjectile> projectile_bp = 0;

}

void ASIAEnemyWave::BeginPlay()
{
	Super::BeginPlay();

    // find bunker and calc lowest z
    TArray<AActor*> all_actors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASIABunkerActor::StaticClass(), all_actors);
    if (!all_actors.Num())
    {
        USIAHelpers::err("there is no binker actors in level");
    }
    else
    {
        ASIABunkerActor* pawn = Cast<ASIABunkerActor>(all_actors[0]);
        if (pawn)
        {
            lowest_z = pawn->GetActorLocation().Z + pawn->GetHeight();
        }
    }

	Spawn();
}

void ASIAEnemyWave::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    CheckAndMove();
    CheckAndFire();
    CheckAndSpawnUFO();
}

void ASIAEnemyWave::Spawn()
{
    killed_pawns = 0;

	FVector pos = GetActorLocation();
	FVector2D polar;
	FVector2D flat_pos;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	float shift = 180/EnemiesInRow;

    FMath::CartesianToPolar(FVector2D(pos.X, pos.Y), polar);
	polar.X += 10;
    float original_angle = polar.Y;

    pos.Z -= WaveCount * NewWaveShift;
    if (pos.Z < lowest_z)
        pos.Z = lowest_z;

	ASIAEnemyPawn* pawn = 0;

	Enemies.SetNum(EnemyTemplate.Num());
	for (int row=0;row < Enemies.Num();++row)
		Enemies[row].SetNum(EnemiesInRow);

	for (int row=0;row < EnemyTemplate.Num();++row)
	{
		for (int col = 0; col < EnemiesInRow; ++col)
		{
            if (col > 0)
			    polar.Y -= FMath::DegreesToRadians(shift);

            FMath::PolarToCartesian(polar, flat_pos);
			pos.X = flat_pos.X;
			pos.Y = flat_pos.Y;


            if (!EnemyTemplate[row].GetDefaultObject())
            {
                USIAHelpers::err("enemy template is null");
                return;
            }
			pawn = GetWorld()->SpawnActor<ASIAEnemyPawn>(EnemyTemplate[row].GetDefaultObject()->GetClass(), pos, GetActorRotation(), SpawnParams);
            pawn->row = row;
            pawn->col = col;
			Enemies[row][col] = pawn;
		}
        polar.Y = original_angle;
		if (pawn)
			pos.Z -= pawn->GetHeight()*2;
	}
}

void ASIAEnemyWave::SpawnNewWave()
{
    Enemies.Empty();
    WaveCount++;
    Spawn();
}

void ASIAEnemyWave::DestroyPawn(int row, int col)
{
    ASIAEnemyPawn* pawn = Enemies[row][col];
    Enemies[row][col] = 0;
    pawn->Destroy();
    killed_pawns++;

    if (killed_pawns == EnemyTemplate.Num() * EnemiesInRow)
    {
        SpawnNewWave();
    }

    //MovementRightStep += 0.1f;
    MoveCooldown -= 0.05f;
    if (MoveCooldown < 0)
    {
        MoveCooldown = 0;
        MovementRightStep = 0.8;
    }

    //USIAHelpers::err(FString::SanitizeFloat(MoveCooldown));
    //music_pitch += 0.1f;
    ASIAPlayerController* pc = Cast<ASIAPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    if (pc)
        pc->AddMusicPitch(0.01f);
}

void ASIAEnemyWave::CheckAndMove()
{
    double t = GetWorld()->GetRealTimeSeconds();
    if ((t - move_timestamp) < MoveCooldown)
        return;
    move_timestamp = t;

    float move_right_value = 0;
    int move_up_value = 0;
    if (movement_dir == 0) // right
    {
        ASIAEnemyPawn* rightmost = GetRightmost();
        if (rightmost)
        {
            float angle = rightmost->GetCurrentAngle();
            if (angle < 10)
            {
                move_up_value = -MovementUpStep; // go down
                move_right_value = -MovementRightStep; // go left
                movement_dir = 1;
            }
            else
                move_right_value = MovementRightStep; // go right
        }
    }
    else if (movement_dir == 1) // left
    {
        ASIAEnemyPawn* leftmost = GetLeftmost();
        if (leftmost)
        {
            float angle = leftmost->GetCurrentAngle();
            if (angle > 350)
            {
                move_up_value = -MovementUpStep; // go down
                move_right_value = MovementRightStep; // go right
                movement_dir = 0;
            }
            else
                move_right_value = -MovementRightStep; // go left
        }
    }

    // I have no time
    ASIAEnemyPawn* lowest = GetLowest();
    if (lowest)
    {
        //USIAHelpers::err(FString::SanitizeFloat((lowest->GetActorLocation().Z + move_up_value)));
        //USIAHelpers::err(FString::SanitizeFloat(lowest_z));

        float current_z = lowest->GetActorLocation().Z + move_up_value - lowest->GetHeight();

        if (current_z <= lowest_z)
        {
            ASIAPlayerController* pc = Cast<ASIAPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
            if (pc)
                pc->GameOver();
        }
    }

    ASIAEnemyPawn* pawn = 0;
    for (int row = 0; row < Enemies.Num(); ++row)
    {
        for (int col = 0; col < Enemies[row].Num(); ++col)
        {
            pawn = Enemies[row][col];
            if (pawn)
            {
                if (move_up_value)
                    pawn->MoveUp(move_up_value);
                else
                    pawn->MoveRight(move_right_value);
            }
        }
    }
}

void ASIAEnemyWave::CheckAndFire()
{
    double t = GetWorld()->GetRealTimeSeconds();
    if ((t - fire_timestamp) < FireCooldown)
        return;
    fire_timestamp = t;

    ASIAEnemyPawn* pawn = GetShooter();
    if (pawn)
        pawn->Fire();
}

void ASIAEnemyWave::CheckAndSpawnUFO()
{
    if (UFO)
    {
        float angle = UFO->GetCurrentAngle();
        if (angle < 10)
        {
            UFO->Destroy();
            UFO = 0;
            return;
        }

        UFO->MoveRight(UFORightStep);
        return;
    }

    double t = GetWorld()->GetRealTimeSeconds();
    if ((t - ufo_timestamp) < UFOCooldown)
        return;
    ufo_timestamp = t;

    int spawn = FMath::RandRange(0,1);

    if (!spawn)
        return;

    if (UFO)
        return;

    FVector pos = GetActorLocation();
    FVector2D polar;
    FVector2D flat_pos;

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    FMath::CartesianToPolar(FVector2D(pos.X, pos.Y), polar);
    polar.X += 10;
    float original_angle = polar.Y;

    FMath::PolarToCartesian(polar, flat_pos);
    pos.X = flat_pos.X;
    pos.Y = flat_pos.Y;
    pos.Z += UFOShift;

    if (!UFOTemplate.GetDefaultObject())
    {
        USIAHelpers::err("UFO template is null");
        return;
    }
    UFO = GetWorld()->SpawnActor<ASIAMovablePawn>(UFOTemplate.GetDefaultObject()->GetClass(), pos, GetActorRotation(), SpawnParams);

    if (UFOSound)
        UGameplayStatics::PlaySound2D(GetWorld(), UFOSound);
}

ASIAEnemyPawn* ASIAEnemyWave::GetLeftmost()
{
    int r = EnemyTemplate.Num();
    int c = EnemiesInRow;
    for (int row = 0; row < Enemies.Num(); ++row)
    {
        for (int col = 0; col < Enemies[row].Num(); ++col)
        {
            if (col > c)
                break;

            if (Enemies[row][col])
            {
                r = row;
                c = col;
                break;
            }
        }
    }

    if (r == EnemyTemplate.Num())
        return 0;

    return Enemies[r][c];
}

ASIAEnemyPawn* ASIAEnemyWave::GetRightmost()
{
    int r = -1;
    int c = -1;
    for (int row = Enemies.Num()-1; row >= 0; --row)
    {
        for (int col = Enemies[row].Num()-1; col >= 0; --col)
        {
            if (col < c)
                break;

            if (Enemies[row][col])
            {
                r = row;
                c = col;
                break;
            }
        }
    }

    if (r == -1)
        return 0;

    return Enemies[r][c];
}

ASIAEnemyPawn* ASIAEnemyWave::GetLowest()
{
    int r = -1;
    int c = -1;
    for (int row = Enemies.Num() - 1; row >= 0; --row)
    {
        for (int col = Enemies[row].Num() - 1; col >= 0; --col)
        {
            if (Enemies[row][col])
            {
                r = row;
                c = col;
                break;
            }
        }
        if (r)
            break;
    }

    if (r == -1)
        return 0;

    return Enemies[r][c];
}

ASIAEnemyPawn* ASIAEnemyWave::GetShooter()
{
    // go up and right from bottom
    // only one from column

    ASIAEnemyPawn* pawn=0;
    TArray<ASIAEnemyPawn*> candidates;
    candidates.SetNum(EnemiesInRow);

    for (int row = Enemies.Num()-1; row >= 0; --row)
    {
        for (int col = 0; col < Enemies[row].Num(); ++col)
        {
            pawn = Enemies[row][col];
            if (pawn && !candidates[col])
                candidates[col]=pawn;
        }
    }

    TArray<ASIAEnemyPawn*> nonempty;
    for (auto p : candidates)
        if (p)
            nonempty.Add(p);

    if (nonempty.IsEmpty())
        return 0;

    int index = FMath::RandRange(0, nonempty.Num()-1);

    return nonempty[index];
}