// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackHole.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABlackHole::ABlackHole()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    MeshComp->SetSimulatePhysics(false);
    MeshComp->SetNotifyRigidBodyCollision(true);
    MeshComp->OnComponentHit.AddDynamic(this, &ABlackHole::OnHit);
    //set as root component
    RootComponent = MeshComp;

    SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
    SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    SphereComp->SetupAttachment(MeshComp);

}

// Called when the game starts or when spawned
void ABlackHole::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void ABlackHole::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bUseBlackHole)
    {
        TArray<AActor*> ActorsArray;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), ActorsArray);
        for (AActor* OneActor : ActorsArray)
        {
            if (OneActor == this)
            {
                UE_LOG(LogTemp, Display, TEXT("self no need to add force\n"));
                return;
            }

            TArray<UPrimitiveComponent*> PrimitiveCompArray;
            OneActor->GetComponents(PrimitiveCompArray);

            for (UPrimitiveComponent* OneCompPtr : PrimitiveCompArray)
            {
                if (OneCompPtr != nullptr && OneCompPtr->IsSimulatingPhysics())
                {
                    OneCompPtr->AddRadialForce(GetActorLocation(), 500000000, -2000, RIF_Constant, true);
                    UE_LOG(LogTemp, Display, TEXT("add force to %s\n"), *(OneActor->GetName()));
                }
            }
        }
    } // end for bUseBlackHole
}

void ABlackHole::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                       FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr && OtherComp->IsSimulatingPhysics())
    {
        OtherActor->Destroy();
        UE_LOG(LogTemp, Warning, TEXT("Destroy one obj"));

        // if (GEngine)
        // {
        //     GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("I Hit: %s"), *OtherActor->GetName()));
        // }
    }
}
