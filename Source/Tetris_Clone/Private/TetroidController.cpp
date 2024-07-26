// Fill out your copyright notice in the Description page of Project Settings.


#include "TetroidController.h"
#include "TetroidSpawner.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"

ATetroidController::ATetroidController()
{

}
        //EnhancedInputComponent->BindAction(MoveLeftAction, ETriggerEvent::Triggered, this, &ATetroidController::MoveTetroidLeft);
        //EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &ATetroidController::MoveTetroidRight);
    

void ATetroidController::OnPossess(APawn* pawn)
{
    Super::OnPossess(pawn);

    EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);

	checkf(EnhancedInputComponent,
	       TEXT("Unable to get reference to the EnhancedInputComponent."));

    UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
        ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
    checkf(InputSubsystem,
        TEXT("Unable to get reference to the EnhancedInputLocalPlayerSubsystem."));

    checkf(InputMappingContext, TEXT("InputMappingContent was not specified."));
    InputSubsystem->ClearAllMappings();
    InputSubsystem->AddMappingContext(InputMappingContext, 0);

    if (MoveLeftAction)
    {
        EnhancedInputComponent->BindAction(MoveLeftAction, ETriggerEvent::Triggered, this, &ATetroidController::MoveTetroidLeft);
    }

     if (MoveRightAction)
    {
        EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &ATetroidController::MoveTetroidRight);
    }

     GEngine->AddOnScreenDebugMessage(0, 7.0f, FColor::Green, TEXT("PossesionComplete"));

     if (RotateTetroidAction)
     {
         EnhancedInputComponent->BindAction(RotateTetroidAction, ETriggerEvent::Triggered, this, &ATetroidController::RotateTetroid);
     }

     if (SpeedUpAction)
     {
         EnhancedInputComponent->BindAction(SpeedUpAction, ETriggerEvent::Triggered, this, &ATetroidController::SpeedUpTetroid);
         EnhancedInputComponent->BindAction(SpeedUpAction, ETriggerEvent::Completed, this, &ATetroidController::SpeedDownTetroid);
     }
}

void ATetroidController::OnUnPossess()
{
    EnhancedInputComponent->ClearActionBindings();
    Super::OnUnPossess();

}

void ATetroidController::BeginPlay()
{
	Super::BeginPlay();
}

void ATetroidController::MoveTetroidLeft()
{   
    
    if(TetroidSpawner) TetroidSpawner->CheckAndMove(LeftOffset);

    //AddActorLocalOffset(UKismetMathLibrary::Vector_SnappedToGrid(LeftOffset, 50.0f));
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(0, 5.0f, FColor::Red, TEXT("Pressed"));
    }
}

void ATetroidController::MoveTetroidRight()
{
   if(TetroidSpawner) TetroidSpawner->CheckAndMove(RightOffset);
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(0, 5.0f, FColor::Red, TEXT("Pressed"));
    }
}

void ATetroidController::RotateTetroid()
{
    TetroidSpawner->RotateTetroid();
}

void ATetroidController::SpeedUpTetroid()
{
    TetroidSpawner->setrate(0.3);

}

void ATetroidController::SpeedDownTetroid()
{
    TetroidSpawner->setrate(1.0f);
}