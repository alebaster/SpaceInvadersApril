#include "SIAPlayerPawn.h"

#include "Engine/StaticMesh.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Engine/LocalPlayer.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/Box.h"
#include "DrawDebugHelpers.h"
#include "Components/LineBatchComponent.h"

#include "SIAPlayerController.h"
#include "../Actors/SIATerrainActor.h"

#include "../Utils/SIAHelpers.h"

ASIAPlayerPawn::ASIAPlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;

    //Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerMesh"));
    //RootComponent = Mesh;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(Mesh);

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
}

void ASIAPlayerPawn::BeginPlay()
{
	Super::BeginPlay();

    //UGameplayStatics::PlaySound2D()

    // init input subsystem
    APlayerController* pc = Cast<APlayerController>(GetController());
    if (pc)
    {
        
        if (UEnhancedInputLocalPlayerSubsystem* input_system = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer()))
        {
            if (InputMapping)
            {
                //input_system->AddMappingContext(InputMapping->LoadSynchronous(), Priority);
                input_system->AddMappingContext(InputMapping,0);
            }
        }
    }

    InitPos = GetActorLocation();
    InitRot = GetActorRotation();
}

void ASIAPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    //_last_fire_timestamp = GetWorld()->GetRealTimeSeconds();
}

void ASIAPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    
    UEnhancedInputComponent* input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    if (!input)
    {
        USIAHelpers::err("cannot cast EnhancedInputComponent");
        return;
    }
        
    if (MoveForwardInput)
        input->BindAction(MoveForwardInput, ETriggerEvent::Triggered, this, &ASIAPlayerPawn::MoveUpCommand);
    else
        USIAHelpers::err("MoveForwardInput in player pawn is null");

    if (MoveRightInput)
        input->BindAction(MoveRightInput, ETriggerEvent::Triggered, this, &ASIAPlayerPawn::MoveRightCommand);
    else
        USIAHelpers::err("MoveRightInput in player pawn is null");

    if (FireInput)
        input->BindAction(FireInput, ETriggerEvent::Triggered, this, &ASIAPlayerPawn::FireCommand);
    else
        USIAHelpers::err("Fire in player pawn is null");
}

void ASIAPlayerPawn::MoveUpCommand(const FInputActionInstance& Instance)
{
    // we cant go up in space invaders
    return;

    float v = Instance.GetValue().Get<float>();
    //USIAHelpers::info(FString::SanitizeFloat(v));
    MoveUp(v);
    return;
}

void ASIAPlayerPawn::MoveRightCommand(const FInputActionInstance& Instance)
{
    float v = Instance.GetValue().Get<float>();
    float angle = GetCurrentAngle();
    //USIAHelpers::err(FString::SanitizeFloat(angle));

    if (angle > 350 && v < 0)
        return;

    if (angle < 10 && v > 0)
        return;

    MoveRight(v);

    return;
}

void ASIAPlayerPawn::FireCommand(const FInputActionInstance& Instance)
{
    double t = GetWorld()->GetRealTimeSeconds();
    if ((t - _last_fire_timestamp) > FireCooldown)
    {
        _last_fire_timestamp = t;
        Fire();
    }
}

void ASIAPlayerPawn::ResetPawn()
{
    SetActorLocation(InitPos);
    SetActorRotation(InitRot);
}

void ASIAPlayerPawn::TakeDamage(FPointDamageEvent* DamageEvent)
{
    Super::TakeDamage(DamageEvent);

    ASIAPlayerController* pc = Cast<ASIAPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    if (pc)
        pc->PlayerKilled();
}