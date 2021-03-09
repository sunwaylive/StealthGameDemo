// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameState.h"


#include "FPSCharacter.h"
#include "FPSPlayerController.h"

void AFPSGameState::MulticastOnMissionComplete_Implementation(APawn* MyInstigator, bool bMissionSuccess)
{
    // for (FConstPawnIterator PawnIt = GetWorld()->GetPawnIterator(); PawnIt; PawnIt++)
    // {
    //     APawn* Pawn = PawnIt->Get();
    //     if (Pawn != nullptr&& Pawn->IsLocallyControlled())
    //     {
    //         Pawn->DisableInput(nullptr);
    //     }
    // }

    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
    {
        AFPSPlayerController* PC = Cast<AFPSPlayerController>(It->Get());
        if (PC && PC->IsLocalController())
        {
            PC->OnMissionCompleted(MyInstigator, bMissionSuccess);

            APawn* MyPawn = PC->GetPawn();
            if (MyPawn)
            {
                MyPawn->DisableInput(PC);
            }
        }
    }
}
