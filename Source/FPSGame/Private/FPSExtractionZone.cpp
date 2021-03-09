// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSExtractionZone.h"


#include "FPSCharacter.h"
#include "FPSGameMode.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFPSExtractionZone::AFPSExtractionZone()
{
    OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
    OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    OverlapComp->SetCollisionResponseToAllChannels(ECR_Ignore);
    OverlapComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    OverlapComp->SetBoxExtent(FVector(200.0f));
    OverlapComp->SetHiddenInGame(false);
    RootComponent = OverlapComp;

    OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSExtractionZone::HandleOverlap);

    //material comp
    DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
    DecalComp->DecalSize = FVector(200.f, 200.f, 200.f);
    DecalComp->SetupAttachment(OverlapComp);
}

void AFPSExtractionZone::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                       bool bFromSweep, const FHitResult& SweepResult)
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
                                         FString::Printf(
                                             TEXT("extraction zone overlapped %s"), *OtherActor->GetName()));
    }

    AFPSCharacter* MyPawn = Cast<AFPSCharacter>(OtherActor);
    if (MyPawn == nullptr)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
                                         FString::Printf(TEXT("pawn null, err happened: %s"), *OtherActor->GetName()));
        return;
    }

    if (MyPawn->bIsCarryingObjective)
    {
        AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
        //Game Mode永远只会在server上跑， 所有client都是nullptr
        if (GM)
        {
            GM->CompleteMission(MyPawn, true);
        }
    }
    else
    {
        UGameplayStatics::PlaySound2D(this, ObjectiveMissingSound);
    }
}
