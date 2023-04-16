#include "SIAWeaponComponent.h"

#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"

#include "../Actors/SIAProjectileActor.h"

#include "../Utils/SIAHelpers.h"

USIAWeaponComponent::USIAWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	//ConstructorHelpers::FClassFinder<ASIAProjectileActor> UMainHUDClass(TEXT("Blueprint'/Game/MySIAProjectile_bp'"));
	//if (UMainHUDClass.Succeeded())
	//	Projectile_bp = UMainHUDClass.Class;
}


void USIAWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

}


void USIAWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USIAWeaponComponent::Fire(FVector from, FVector to)
{
	UWorld* const World = GetWorld();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	//SpawnParams.Instigator = this;
    if (!Projectile_bp)
    {
        USIAHelpers::err("projectile bp is null");
        return;
    }

    ASIAProjectileActor* const projectile = World->SpawnActor<ASIAProjectileActor>(Projectile_bp.GetDefaultObject()->GetClass(), from, SpawnParams.Owner->GetActorRotation(), SpawnParams);
	if (projectile)
	{
		projectile->SetOwner(GetOwner());
		projectile->Launch(to);
	}
	
    if (FireSound)
        UGameplayStatics::PlaySound2D(GetWorld(), FireSound);
}