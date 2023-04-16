#include "SIAMovablePawn.h"

#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>

#include "Engine/DamageEvents.h"
#include <GameFramework/DamageType.h>

#include "../Actors/SIATerrainActor.h"
#include "../Components/SIAWeaponComponent.h"

#include "../Utils/SIAHelpers.h"

ASIAMovablePawn::ASIAMovablePawn()
{
	PrimaryActorTick.bCanEverTick = true;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;

    WeaponComp = CreateDefaultSubobject<USIAWeaponComponent>(TEXT("WeaponComp"));
}

void ASIAMovablePawn::BeginPlay()
{
	Super::BeginPlay();
	
    // find terrain and snap to it
    TArray<AActor*> all_actors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASIATerrainActor::StaticClass(), all_actors);
    if (!all_actors.Num())
    {
        USIAHelpers::err("there is no terrain actors in level");
    }
    else
    {
        Terrain = all_actors[0];
        SnapToTerrain();
    }
}

void ASIAMovablePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASIAMovablePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASIAMovablePawn::SnapToTerrain()
{
    if (!Terrain)
    {
        // todo log
        return;
    }

    float range = 1000.0f;
    FVector StartLocation = GetActorLocation();
    //FVector EndLocation = StartLocation + (-GetActorUpVector() * range);
    FVector EndLocation = Terrain->GetActorLocation();
    EndLocation.Z = StartLocation.Z;
    
    FHitResult RaycastHit;
    
    FCollisionQueryParams CQP;
    CQP.AddIgnoredActor(this);
    //CQP.bTraceComplex = true;
    
    //DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red,false,999999.0f,0,5);

    // todo switch to math solution
    GetWorld()->LineTraceSingleByChannel(RaycastHit, StartLocation, EndLocation, ECollisionChannel::ECC_Visibility, CQP);
    
    AActor* actor = RaycastHit.GetActor();
    if (!actor)
    {
        USIAHelpers::err("cannot raycast to terrain actor");
        return;
    }
    //actor = Terrain;
    
    FVector terrain_flat_center = actor->GetActorLocation();
    terrain_flat_center.Z = GetActorLocation().Z;
    FVector normal = GetActorLocation() - terrain_flat_center;
    
    FVector pos = RaycastHit.ImpactPoint;
    
    FVector bmin, bmax;
    Mesh->GetLocalBounds(bmin, bmax);
    FTransform transform;
    FVector scale = Mesh->GetComponentScale();
    float offset = (bmax.X - bmin.X) * scale.X / 1.5f;
    normal.Normalize();
    FVector normal_offset = offset * normal;
    pos += normal_offset;
    SetActorLocation(pos);

    FVector forward = GetActorForwardVector();
    FRotator rot = UKismetMathLibrary::MakeRotFromZX(normal, forward);

    SetActorRotation(rot);
}

void ASIAMovablePawn::MoveRight(float v)
{
    FVector pos = GetActorLocation();

    FVector2D polar;
    FMath::CartesianToPolar(FVector2D(pos.X, pos.Y), polar);
    polar.Y -= FMath::DegreesToRadians(Speed * v);

    FVector2D flat_pos;
    FMath::PolarToCartesian(polar, flat_pos);
    pos.X = flat_pos.X;
    pos.Y = flat_pos.Y;
    SetActorLocation(pos,true);
    //FHitResult hit;
    //SetActorLocation(pos,true,&hit);
    //if (hit.bBlockingHit)
    //    USIAHelpers::err(hit.GetActor()->GetActorNameOrLabel());


    // rotation
    FVector terrain_flat_center = Terrain->GetActorLocation();
    terrain_flat_center.Z = GetActorLocation().Z;

    FVector normal = GetActorLocation() - terrain_flat_center;
    FVector forward = GetActorForwardVector();
    FRotator rot = UKismetMathLibrary::MakeRotFromZX(normal, forward);

    SetActorRotation(rot);
}

void ASIAMovablePawn::MoveUp(float v)
{
    FVector pos = GetActorLocation();
    pos.Z += Speed * v;
    SetActorLocation(pos,true);
}

void ASIAMovablePawn::Fire()
{
    //USIAHelpers::info("fire");

    FVector from(GetActorLocation());
    FVector bmin, bmax;
    Mesh->GetLocalBounds(bmin, bmax);
    FTransform transform;
    FVector scale = Mesh->GetComponentScale();
    float offset = (bmax.X - bmin.X) * scale.X / 2;
    //FVector normal_offset = offset*RaycastHit.Normal;
    FVector forward_offset = offset * GetActorForwardVector();
    from += forward_offset;

    if (WeaponComp)
    {
        WeaponComp->Fire(from, GetActorForwardVector());
    }
}

float ASIAMovablePawn::GetCurrentAngle()
{
    FVector pos = GetActorLocation();
    FVector2D polar;
    FMath::CartesianToPolar(FVector2D(pos.X, pos.Y), polar);

    return FMath::RadiansToDegrees(polar.Y)+180;
}

float ASIAMovablePawn::GetHeight()
{
    if (!Mesh)
        return -1.0f;

    FVector bmin, bmax;
    Mesh->GetLocalBounds(bmin, bmax);
    FTransform transform;
    FVector scale = Mesh->GetComponentScale();
    float offset = (bmax.Z - bmin.Z) * scale.Z;

    return offset;
}

float ASIAMovablePawn::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    FPointDamageEvent* de = 0;

    if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
        de = (FPointDamageEvent*)&DamageEvent; // why unreal cast dont work here?
    else
    {
        // todo log
        return 0.0f;
    }

    if (!de)
    {
        // todo log
        return 0.0f;
    }

    TakeDamage(de);

    return 1.0f;
}

void ASIAMovablePawn::TakeDamage(FPointDamageEvent* DamageEvent)
{
    if (OnHitSound)
        UGameplayStatics::PlaySound2D(GetWorld(), OnHitSound);
}