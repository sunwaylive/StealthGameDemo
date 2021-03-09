// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"

#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "FPSGameState.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AFPSGameMode::AFPSGameMode()
{
    // set default pawn class to our Blueprinted character
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
    DefaultPawnClass = PlayerPawnClassFinder.Class;

    // use our custom HUD class
    HUDClass = AFPSHUD::StaticClass();
    GameStateClass = AFPSGameState::StaticClass();
}

void AFPSGameMode::CompleteMission(APawn* InstigatorPawn, bool bMissionSucc)
{
    TArray<AActor*> ReturnedActors;
    UGameplayStatics::GetAllActorsOfClass(this, SpectatingViewpointClass, ReturnedActors);
    if (ReturnedActors.Num() > 0)
    {
        AActor* NewViewTarget = ReturnedActors[0];

        for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
        {
            //APlayerController* PC = Cast<APlayerController>(InstigatorPawn->GetController());
            APlayerController* PC = It->Get();
            if (PC && NewViewTarget)
            {
                PC->SetViewTargetWithBlend(NewViewTarget, 0.5, EViewTargetBlendFunction::VTBlend_Cubic);
            }
        }
    }
    else
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
                                             FString::Printf(TEXT("can not change view target")));
        }
    }

    AFPSGameState* GS = GetGameState<AFPSGameState>();
    if (GS)
    {
        GS->MulticastOnMissionComplete(InstigatorPawn, bMissionSucc);
    }

    OnMissionCompleted(InstigatorPawn, bMissionSucc);
}
