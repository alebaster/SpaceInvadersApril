#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"



#include "SIAHelpers.generated.h"

UCLASS()
class SPACEINVADERSAPRIL_API USIAHelpers : public UObject
{
	GENERATED_BODY()
	
public:
    static void err(const FString& s)
    {
        UE_LOG(LogTemp, Error, TEXT("%s"), *s)
    }

    static void softerr(const FString& s)
    {
        UE_LOG(LogTemp, Warning, TEXT("%s"), *s)
    }

    static void info(const FString& s)
    {
        UE_LOG(LogTemp, Display, TEXT("%s"), *s)
    }
};
