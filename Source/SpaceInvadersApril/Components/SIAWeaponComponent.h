#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"



#include "SIAWeaponComponent.generated.h"

//UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
UCLASS()
class SPACEINVADERSAPRIL_API USIAWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USIAWeaponComponent();

	void Fire(FVector from, FVector to);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ASIAProjectileActor> Projectile_bp = 0;
	//UPROPERTY()
	//class ASIAProjectileActor* Projectile = 0;

    UPROPERTY(EditDefaultsOnly, Category = "Sound")
    class USoundWave* FireSound = 0;
};
