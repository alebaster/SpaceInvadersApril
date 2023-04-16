#include "SIASpawner.h"

#include "../Pawns/SIAMovablePawn.h"

#include "Kismet/KismetMathLibrary.h"

#include "../Utils/SIAHelpers.h"

ASIASpawner::ASIASpawner()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ASIASpawner::BeginPlay()
{
	Super::BeginPlay();

    //Spawn();
}

void ASIASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASIASpawner::Spawn()
{
    // todo
    // sad code duplication

    FVector pos = GetActorLocation();
    FVector2D polar;
    FVector2D flat_pos;

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    float shift = 360 / Columns;

    FMath::CartesianToPolar(FVector2D(pos.X, pos.Y), polar);

    ASIAMovablePawn* pawn=0;
    for (int row = 0; row < Template.Num(); ++row)
    {
        for (int col = 0; col < Columns; ++col)
        {
            if (col > 0)
                polar.Y -= FMath::DegreesToRadians(shift);

            FMath::PolarToCartesian(polar, flat_pos);
            pos.X = flat_pos.X;
            pos.Y = flat_pos.Y;


            if (!Template[row].GetDefaultObject())
            {
                USIAHelpers::err("template is null");
                return;
            }
            pawn = GetWorld()->SpawnActor<ASIAMovablePawn>(Template[row].GetDefaultObject()->GetClass(), pos, GetActorRotation(), SpawnParams);
        }
        if (pawn)
            pos.Z -= pawn->GetHeight() * 2;
    }
}