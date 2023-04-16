#include "SIAEnemyPawn.h"

#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include <GameFramework/DamageType.h>

#include "../Player/SIAPlayerController.h"
#include "../Enemy/SIAEnemyWave.h"

ASIAEnemyPawn::ASIAEnemyPawn()
{
	PrimaryActorTick.bCanEverTick = true;

    //Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    //RootComponent = Mesh;
}

void ASIAEnemyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASIAEnemyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASIAEnemyPawn::TakeDamage(FPointDamageEvent* DamageEvent)
{
    Super::TakeDamage(DamageEvent);

    ASIAEnemyWave* parent = Cast<ASIAEnemyWave>(GetOwner());
    if (parent)
    {
        parent->DestroyPawn(row, col);

        ASIAPlayerController* pc = Cast<ASIAPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0));
        if (pc)
            pc->EnemyKilled();
    }
}

