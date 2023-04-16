#include "SIATerrainActor.h"

ASIATerrainActor::ASIATerrainActor()
{
	PrimaryActorTick.bCanEverTick = false;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;
}

void ASIATerrainActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASIATerrainActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ASIATerrainActor::GetRadius()
{
    if (!Mesh)
        return -1.0f;

    FVector bmin, bmax;
    Mesh->GetLocalBounds(bmin, bmax);
    FVector scale = Mesh->GetComponentScale();
    float size = (bmax.Y - bmin.Y) * scale.Y;

    return size;
}