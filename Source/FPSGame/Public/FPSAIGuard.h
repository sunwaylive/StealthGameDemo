// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/PawnSensingComponent.h"

#include "FPSAIGuard.generated.h"

UENUM(BlueprintType)
enum class EAIState : uint8
{
    Idle,
    Suspicious,
    Alerted,
};


UCLASS()
class FPSGAME_API AFPSAIGuard : public ACharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    AFPSAIGuard();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnSeePawn(APawn* SeenPawn);

    UFUNCTION()
    void OnNoiseHeard(APawn* NoiseInstigatorP, const FVector& Location, float Volume);

    UPROPERTY(ReplicatedUsing=OnRep_GuardState)
    EAIState GuardState;
    
    UFUNCTION()
    void OnRep_GuardState();

    UPROPERTY(VisibleAnywhere, Category="Components")
    UPawnSensingComponent* PawnSensingComponent;

    FTimerHandle TimerHandle_ResetRotate;
    FRotator OriginRotator;
    
    void SetGuardState(EAIState NewState);

    UFUNCTION(BlueprintImplementableEvent, Category="AI")
    void OnGuardStateChanged(EAIState NewState);

    UFUNCTION()
    void ResetRotate();

    //patrol ability
    UPROPERTY(EditInstanceOnly, Category="AI")
    bool bPatrol;

    UPROPERTY(EditInstanceOnly, Category="AI", meta=(EditconCondition="bPatrol"))
    AActor* PatrolStartPoint;
    UPROPERTY(EditInstanceOnly, Category="AI", meta=(EditconCondition="bPatrol"))
    AActor* PatrolEndPoint;

    AActor* CurPatrolPoint;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
    void MoveToNextPatrolPoint();
};
