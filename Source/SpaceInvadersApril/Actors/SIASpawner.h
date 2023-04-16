#pragma once

// unused

// cannot spawn ASIABunkerActor due runtime error in Dynamic Mesh creation for some unknown reason
// have to add it manually 

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"



#include "SIASpawner.generated.h"



UCLASS()
class SPACEINVADERSAPRIL_API ASIASpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	ASIASpawner();

    UPROPERTY(EditAnywhere)
    TArray<TSubclassOf<class ASIAMovablePawn>> Template;

    UPROPERTY(EditAnywhere)
    int Columns = 5;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
    void Spawn();

};
