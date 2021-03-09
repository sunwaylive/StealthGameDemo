// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAIGuard.h"

#include "DrawDebugHelpers.h"
#include "FPSGameMode.h"
#include "UnrealNetwork.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
    PawnSensingComponent->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnSeePawn);
    PawnSensingComponent->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnNoiseHeard);
    GuardState = EAIState::Idle;

    CurPatrolPoint = PatrolStartPoint;
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
    Super::BeginPlay();
    OriginRotator = GetActorRotation();

    if (bPatrol)
    {
        MoveToNextPatrolPoint();
    }
}

void AFPSAIGuard::OnSeePawn(APawn* SeenPawn)
{
    if (SeenPawn == nullptr)
    {
        return;
    }

    AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
    if (GM)
    {
        GM->CompleteMission(SeenPawn, false);
    }

    DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.f, 12, FColor::Red, false, 10.f);

    SetGuardState(EAIState::Alerted);

    AController* AIController = GetController();
    if (AIController)
    {
        AIController->StopMovement();
    }
}

void AFPSAIGuard::OnNoiseHeard(APawn* NoiseInstigatorP, const FVector& Location, float Volume)
{
    DrawDebugSphere(GetWorld(), Location, 32.f, 12, FColor::Green, false, 10.f);

    FVector DstDirection = Location - GetActorLocation();
    DstDirection.Normalize();
    FRotator NewLookAt = FRotationMatrix::MakeFromX(DstDirection).Rotator();
    NewLookAt.Pitch = 0.f;
    NewLookAt.Roll = 0.f;

    SetActorRotation(NewLookAt);

    GetWorldTimerManager().ClearTimer(TimerHandle_ResetRotate);
    GetWorldTimerManager().SetTimer(TimerHandle_ResetRotate, this, &AFPSAIGuard::ResetRotate, 3.f);

    SetGuardState(EAIState::Suspicious);

    AController* AIController = GetController();
    if (AIController)
    {
        AIController->StopMovement();
    }
}

void AFPSAIGuard::OnRep_GuardState()
{
    OnGuardStateChanged(GuardState);
}

void AFPSAIGuard::SetGuardState(EAIState NewState)
{
    if (GuardState == NewState)
    {
        return;
    }

    GuardState = NewState;
    OnRep_GuardState();
}

void AFPSAIGuard::ResetRotate()
{
    if (GuardState == EAIState::Alerted)
    {
        return;
    }

    SetActorRotation(OriginRotator);
    SetGuardState(EAIState::Idle);

    if (bPatrol)
    {
        MoveToNextPatrolPoint();
    }
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    //switch between patrol points
    if (CurPatrolPoint != nullptr)
    {
       FVector DeltaVec = GetActorLocation() - CurPatrolPoint->GetActorLocation();
        float Dist = DeltaVec.Size();
       if (Dist < 50)
       {
           MoveToNextPatrolPoint();
       }
    }
}

void AFPSAIGuard::MoveToNextPatrolPoint()
{
    if (PatrolStartPoint == nullptr || PatrolEndPoint == nullptr)
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
                                             FString::Printf(
                                                 TEXT("null patrol points")));
        }

        return;
    }

    if (CurPatrolPoint == nullptr || CurPatrolPoint == PatrolStartPoint)
    {
        CurPatrolPoint = PatrolEndPoint;
    }
    else
    {
        CurPatrolPoint = PatrolStartPoint;
    }

    UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), CurPatrolPoint); 
}

void AFPSAIGuard::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPSAIGuard, GuardState);
}
