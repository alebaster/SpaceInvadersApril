#include "SIABunkerActor.h"

#include <ProceduralMeshComponent/Public/ProceduralMeshComponent.h>
//#include "/Engine/Plugins/Experimental/GeometryProcessing/Source/DynamicMesh/Public/DynamicMesh3.h"
#include "KismetProceduralMeshLibrary.h"
#include "UDynamicMesh.h"
//#include "Engine/Plugins/Experimental/GeometryScripting/Source/GeometryScriptingCore/Public/GeometryScript/MeshAssetFunctions.h"
#include "GeometryScriptingCore/Public/GeometryScript/MeshAssetFunctions.h"
#include "GeometryScriptingCore/Public/GeometryScript/MeshBooleanFunctions.h"
#include "GeometryScriptingCore/Public/GeometryScript/MeshPrimitiveFunctions.h"
#include "Components/DynamicMeshComponent.h"
#include "DynamicMeshActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

#include "Engine/DamageEvents.h"
#include <GameFramework/DamageType.h>

#include "../Player/SIAPlayerController.h"

#include "../Utils/SIAHelpers.h"


ASIABunkerActor::ASIABunkerActor()
{
	PrimaryActorTick.bCanEverTick = true;

    //Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OriginalMesh"));
    //RootComponent = Mesh;
    
    //ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>("ProceduralMesh");
    //ProceduralMesh->SetupAttachment(OriginalMesh);

    DynamicMesh = CreateDefaultSubobject<UDynamicMesh>("DynamicMesh");
    //DynamicMesh->SetupAttachment(OriginalMesh);

    DynamicMeshComponent = CreateDefaultSubobject<UDynamicMeshComponent>("DynamicMeshComponent");
    DynamicMeshComponent->SetupAttachment(RootComponent);
    // !bp enable complex collision
    // !bp complex collision as simple


    HoleDynamicMeshActor = CreateDefaultSubobject<ADynamicMeshActor>("HoleDynamicMeshActor");
}

void ASIABunkerActor::BeginPlay()
{
	Super::BeginPlay();

    Mesh->GetStaticMesh()->bAllowCPUAccess = true;

    FGeometryScriptCopyMeshFromAssetOptions opt;
    FGeometryScriptMeshReadLOD lod;
    EGeometryScriptOutcomePins pins;
    UGeometryScriptLibrary_StaticMeshFunctions::CopyMeshFromStaticMesh(Mesh->GetStaticMesh(), DynamicMeshComponent->GetDynamicMesh(), opt, lod, pins);
    Mesh->SetVisibility(false);

    // for the true mesh substruction, we need copy projectile in TakeDamage (or from blueprint)
    // but its too small and with this mesh we dont need to think about XY deep for full mesh destruction
    holeDynamicMesh =  HoleDynamicMeshActor->AllocateComputeMesh();
    FGeometryScriptPrimitiveOptions primitiveOptions;
    FTransform hole_transform;
    //hole_transform.SetLocation(FVector(15,0,0));
    float radius = 10;
    float height = 200;
    int32 radialSteps = 12;
    int32 heightSteps = 5;
    bool bCapped = true;
    EGeometryScriptPrimitiveOriginMode origin = EGeometryScriptPrimitiveOriginMode::Center;
    UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendCylinder(holeDynamicMesh, primitiveOptions,
                              hole_transform, radius, height, radialSteps, heightSteps, bCapped, origin);
}

void ASIABunkerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASIABunkerActor::TakeDamage(FPointDamageEvent* DamageEvent)
{
    Super::TakeDamage(DamageEvent);

    FVector pos = UKismetMathLibrary::InverseTransformLocation(DynamicMeshComponent->GetComponentTransform(), DamageEvent->HitInfo.ImpactPoint);

    FTransform targetTransform;
    FTransform toolTransform;
    toolTransform.SetLocation(pos);
    EGeometryScriptBooleanOperation operation = EGeometryScriptBooleanOperation::Subtract;
    FGeometryScriptMeshBooleanOptions options;
    UGeometryScriptLibrary_MeshBooleanFunctions::ApplyMeshBoolean(DynamicMeshComponent->GetDynamicMesh(), targetTransform, holeDynamicMesh, toolTransform, operation, options);

    //HoleDynamicMeshActor->ReleaseAllComputeMeshes();
}