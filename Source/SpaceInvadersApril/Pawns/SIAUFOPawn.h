#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "SIAMovablePawn.h"

#include "SIAUFOPawn.generated.h"

UCLASS()
class SPACEINVADERSAPRIL_API ASIAUFOPawn : public ASIAMovablePawn
{
	GENERATED_BODY()

public:
	ASIAUFOPawn();

protected:
	virtual void BeginPlay() override;

    virtual void TakeDamage(FPointDamageEvent* DamageEvent) override;

public:	
	virtual void Tick(float DeltaTime) override;

};
