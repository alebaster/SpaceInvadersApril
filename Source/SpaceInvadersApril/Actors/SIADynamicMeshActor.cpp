#include "SIADynamicMeshActor.h"

#include "GeometryScriptingCore/Public/GeometryScript/MeshAssetFunctions.h"

ASIADynamicMeshActor::ASIADynamicMeshActor()
{
    PrimaryActorTick.bCanEverTick = true;

    OriginalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OriginalMesh"));
    OriginalMesh->SetupAttachment(DynamicMeshComponent);
    //DynamicMeshComponent->GetSourceMesh();
    //GetSourceMesh();
}

void ASIADynamicMeshActor::BeginPlay()
{
    Super::BeginPlay();

    //FGeometryScriptCopyMeshFromAssetOptions opt;
    //FGeometryScriptMeshReadLOD lod;
    //EGeometryScriptOutcomePins pins;
    //UGeometryScriptLibrary_StaticMeshFunctions::CopyMeshFromStaticMesh(OriginalMesh->GetStaticMesh(), GetSourceMesh(), opt, lod, pins);
    
}
