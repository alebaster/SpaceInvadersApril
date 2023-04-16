#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "../Pawns/SIAMovablePawn.h"

#include "SIABunkerActor.generated.h"

UCLASS()
class SPACEINVADERSAPRIL_API ASIABunkerActor : public ASIAMovablePawn
{
	GENERATED_BODY()
	
public:	
	ASIABunkerActor();

    //virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;

    virtual void TakeDamage(FPointDamageEvent* DamageEvent) override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
    //UPROPERTY(EditAnywhere)
    //class UStaticMeshComponent* OriginalMesh = 0;

    UPROPERTY(EditAnywhere)
    class UDynamicMeshComponent* DynamicMeshComponent = 0;

    UPROPERTY(EditAnywhere)
    class UDynamicMesh* DynamicMesh = 0;

    //UPROPERTY()
    //class FDynamicMesh3* SourceMesh = 0;

    //UPROPERTY(EditAnywhere)
    //class UProceduralMeshComponent* ProceduralMesh = 0;

    UPROPERTY(EditAnywhere)
    class ADynamicMeshActor* HoleDynamicMeshActor = 0;

    UPROPERTY()
    UDynamicMesh* holeDynamicMesh = 0;
};
