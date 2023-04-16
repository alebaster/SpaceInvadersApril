#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "../Pawns/SIAMovablePawn.h"

#include "SIAEnemyPawn.generated.h"

UCLASS()
class SPACEINVADERSAPRIL_API ASIAEnemyPawn : public ASIAMovablePawn
{
	GENERATED_BODY()

public:
	ASIAEnemyPawn();

    int row = -1;
    int col = -1;

    //virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;

    virtual void TakeDamage(FPointDamageEvent* DamageEvent) override;

public:	
	virtual void Tick(float DeltaTime) override;

};
