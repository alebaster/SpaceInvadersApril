#include "SIAUFOPawn.h"

#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

#include "../Player/SIAPlayerController.h"

ASIAUFOPawn::ASIAUFOPawn()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ASIAUFOPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASIAUFOPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASIAUFOPawn::TakeDamage(FPointDamageEvent* DamageEvent)
{
    Super::TakeDamage(DamageEvent);

    ASIAPlayerController* pc = Cast<ASIAPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    if (pc)
        pc->UFOKilled();

    Destroy();
}
