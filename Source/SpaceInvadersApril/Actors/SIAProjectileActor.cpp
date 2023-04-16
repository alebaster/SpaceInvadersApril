#include "SIAProjectileActor.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

#include <Kismet/GameplayStatics.h>
#include "Kismet/KismetMathLibrary.h"
#include "Engine/DamageEvents.h"
#include <GameFramework/DamageType.h>

#include "../Pawns/SIAMovablePawn.h"

ASIAProjectileActor::ASIAProjectileActor()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	//CollisionComponent->SetRelativeLocation(FVector(0, 0, 0));
    RootComponent = CollisionComponent;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(CollisionComponent);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->bAutoActivate = false;
	AudioComponent->bAutoDestroy = false;
	AudioComponent->SetupAttachment(RootComponent);
	
	//ExplosionPCS = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ExplosionPCS"));
	//ExplosionPCS->bAutoActivate = false;
	//ExplosionPCS->bAutoDestroy = false;
	//ExplosionPCS->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent"));
	ProjectileMovement->UpdatedComponent = CollisionComponent;
	//ProjectileMovement->OnProjectileStop.AddDynamic(this, &AsbrProjectile::OnImpact);
	ProjectileMovement->OnProjectileStop.AddDynamic(this, &ASIAProjectileActor::OnImpact);
	ProjectileMovement->InitialSpeed = 500.f;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->MaxSpeed = 1000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->Bounciness = 0.0f;
}

void ASIAProjectileActor::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(5.0f);
	
	CollisionComponent->IgnoreActorWhenMoving(GetOwner(),true);
}

void ASIAProjectileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASIAProjectileActor::Launch(const FVector& Direction)
{
	if (ProjectileMovement)
		ProjectileMovement->Velocity = Direction * ProjectileMovement->InitialSpeed;

	AudioComponent->SetSound(LaunchSound);
	AudioComponent->Play();
}

void ASIAProjectileActor::OnImpact(const FHitResult& HitResult)
{
	FTransform tr = UKismetMathLibrary::ConvertTransformToRelative(FTransform(HitResult.Location), HitResult.GetActor()->GetActorTransform());
	//UE_LOG(LogTemp, Error, TEXT("oi %s"),*tr.GetLocation().ToString());

    APawn* victim = Cast<APawn>(HitResult.GetActor());
    if (victim)
    {
        FPointDamageEvent PointDamageEvent(1.0f, HitResult, HitResult.ImpactNormal, {});
        victim->TakeDamage(1.0f, PointDamageEvent, GetOwner()->GetInstigatorController(), this);

        if (ExplosionFXTemplate)
        {
            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionFXTemplate, HitResult.ImpactPoint, victim->GetActorRotation(), FVector(0.2f), true);
            //(const UObject* WorldContextObject, UParticleSystem* EmitterTemplate, FVector SpawnLocation, FRotator SpawnRotation, FVector SpawnScale, bool bAutoDestroy, EPSCPoolMethod PoolingMethod, bool bAutoActivateSystem)
        }

    }
    Destroy();
}