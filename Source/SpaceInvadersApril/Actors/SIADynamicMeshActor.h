#pragma once

// delete

#include "CoreMinimal.h"
#include "DynamicMeshActor.h"



#include "SIADynamicMeshActor.generated.h"

UCLASS()
class SPACEINVADERSAPRIL_API ASIADynamicMeshActor : public ADynamicMeshActor
{
	GENERATED_BODY()
	
public:
    ASIADynamicMeshActor();

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere)
    class UStaticMeshComponent* OriginalMesh = 0;
};
