#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SIATerrainActor.generated.h"

UCLASS()
class SPACEINVADERSAPRIL_API ASIATerrainActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ASIATerrainActor();

    float GetRadius();

protected:
	virtual void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere)
    class UStaticMeshComponent* Mesh = 0;

public:	
	virtual void Tick(float DeltaTime) override;

};
