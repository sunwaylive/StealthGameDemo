// Fill out your copyright notice in the Description page of Project Settings.


#include "LaunchPad.h"

#include "FPSCharacter.h"
#include "FPSGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALaunchPad::ALaunchPad()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    PadComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PadComp"));
    PadComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    PadComp->SetCollisionResponseToAllChannels(ECR_Ignore);
    PadComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    PadComp->OnComponentBeginOverlap.AddDynamic(this, &ALaunchPad::OverlapLaunchPad);
    RootComponent = PadComp;
}

// Called when the game starts or when spawned
void ALaunchPad::BeginPlay()
{
    Super::BeginPlay();
}

void ALaunchPad::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);
}

void ALaunchPad::OverlapLaunchPad(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                  const FHitResult& SweepResult)
{
   FRotator LaunchDir = OtherActor->GetActorRotation();
    LaunchDir.Pitch += 70.f;
    FVector LaunchVelocity = LaunchDir.Vector() * 1500.f;
    
    AFPSCharacter* MyCharacter = Cast<AFPSCharacter>(OtherActor);
    if (MyCharacter != nullptr)
    {
        //const FVector MyDir = MyCharacter->GetActorForwardVector();
        MyCharacter->LaunchCharacter(LaunchVelocity, true, true);
    }
    else if (OtherComp && OtherComp->IsSimulatingPhysics())
    {
        OtherComp->AddImpulse(LaunchVelocity, NAME_None, true);

        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ActiveLaunchPadEffect, GetActorLocation());
    }
}

// Called every frame
void ALaunchPad::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
