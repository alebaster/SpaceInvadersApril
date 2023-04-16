#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "../Pawns/SIAMovablePawn.h"

#include "InputAction.h"

#include "SIAPlayerPawn.generated.h"

UCLASS()
class SPACEINVADERSAPRIL_API ASIAPlayerPawn : public ASIAMovablePawn
{
	GENERATED_BODY()

public:
	ASIAPlayerPawn();

    void ResetPawn();

protected:
	virtual void BeginPlay() override;

    virtual void TakeDamage(FPointDamageEvent* DamageEvent) override;

private:
    // camera
    UPROPERTY(EditDefaultsOnly, Instanced, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* CameraBoom = 0;

    UPROPERTY(EditDefaultsOnly, Instanced, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* FollowCamera = 0;

    //
    //UPROPERTY(EditAnywhere)
    //class UStaticMeshComponent* Mesh = 0;

    //UPROPERTY(EditAnywhere)
    //class AActor* Terrain = 0;

    // input
    //UPROPERTY(EditAnywhere)
    //float Speed = 1.0f;

    UPROPERTY(EditAnywhere, Category = "Input")
    class UInputMappingContext* InputMapping = 0;

    UPROPERTY(EditAnywhere, Category = "Input")
    class UInputAction* MoveForwardInput = 0;

    UPROPERTY(EditAnywhere, Category = "Input")
    class UInputAction* MoveRightInput = 0;

    UPROPERTY(EditAnywhere, Category = "Input")
    class UInputAction* FireInput = 0;

    UPROPERTY(EditAnywhere, Category = "Input")
    float FireCooldown = 0.5f;

    double _last_fire_timestamp = 0;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
    void MoveUpCommand(const FInputActionInstance& Instance);
    void MoveRightCommand(const FInputActionInstance& Instance);
    void FireCommand(const FInputActionInstance& Instance);

    FVector InitPos;
    FRotator InitRot;
};
