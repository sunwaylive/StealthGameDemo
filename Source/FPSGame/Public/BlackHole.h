// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlackHole.generated.h"

class USphereComponent;
UCLASS()
class FPSGAME_API ABlackHole : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ABlackHole();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, Category="Components")
    UStaticMeshComponent* MeshComp;

    UPROPERTY(VisibleAnywhere, Category="Components")
    USphereComponent* SphereComp;

    //是否启用黑洞功能
    bool bUseBlackHole = false;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    /** called when projectile hits something */
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
               const FHitResult& Hit);
};
