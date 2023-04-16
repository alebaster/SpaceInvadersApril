#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"



#include "SIAProjectileActor.generated.h"

UCLASS()
class SPACEINVADERSAPRIL_API ASIAProjectileActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ASIAProjectileActor();

	void Launch(const FVector& Direction);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Mesh = 0;

	UPROPERTY(VisibleDefaultsOnly)
	class UProjectileMovementComponent* ProjectileMovement = 0;

	UPROPERTY(VisibleDefaultsOnly)
	class USphereComponent* CollisionComponent = 0;

	UPROPERTY(VisibleDefaultsOnly)
	class UAudioComponent* AudioComponent = 0;

	//UPROPERTY(VisibleDefaultsOnly)
	//class UParticleSystemComponent* ExplosionPCS = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	class UParticleSystem* ExplosionFXTemplate = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	class UParticleSystem* TrailFXTemplate = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	class USoundCue* LaunchSound = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	class USoundCue* ExplosionSound = 0;

private:
	UFUNCTION()
	void OnImpact(const FHitResult& HitResult);
};
