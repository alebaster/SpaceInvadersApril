#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"



#include "SIAMovablePawn.generated.h"

UCLASS()
class SPACEINVADERSAPRIL_API ASIAMovablePawn : public APawn
{
	GENERATED_BODY()

public:
	ASIAMovablePawn();

    void MoveRight(float v);
    void MoveUp(float v);
    void Fire();

    float GetCurrentAngle();
    float GetHeight();


protected:
	virtual void BeginPlay() override;
    virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

    virtual void TakeDamage(FPointDamageEvent* DamageEvent);

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
    UPROPERTY(EditAnywhere)
    class UStaticMeshComponent* Mesh = 0;

    UPROPERTY()
    class AActor* Terrain = 0;

    UPROPERTY(EditAnywhere)
    class USIAWeaponComponent* WeaponComp = 0;

    UPROPERTY(EditDefaultsOnly, Category = "Sound")
    class USoundWave* OnHitSound = 0;

    UPROPERTY(EditAnywhere)
    float Speed = 1.0f;

    void SnapToTerrain();
    
};
