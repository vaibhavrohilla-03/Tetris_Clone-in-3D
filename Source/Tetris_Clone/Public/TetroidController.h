// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TetroidController.generated.h"

/**
 * 
 */
UCLASS()
class TETRIS_CLONE_API ATetroidController : public APlayerController
{
	GENERATED_BODY()
	

public:
	ATetroidController();

	UPROPERTY(BlueprintReadWrite, Category = "Manager")
	class ATetroidSpawner* TetroidSpawner;

private:

	UPROPERTY(EditAnywhere, Category = "Input")
		class UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
		class UInputAction* MoveLeftAction;

	UPROPERTY(EditAnywhere, Category = "Input")
		UInputAction* MoveRightAction;

	//UPROPERTY(EditAnywhere, Category = "Input")
		//UInputAction* SpeedUpAction;

	UPROPERTY(EditAnywhere, Category = "Input")
		UInputAction* RotateTetroidAction;

	UPROPERTY(EditAnywhere, Category = "Input")
		UInputAction* SpeedUpAction;


	UPROPERTY(EditAnyWhere, Category = "OFFSETS")
		FVector LeftOffset;

	UPROPERTY(EditAnyWhere, Category = "OFFSETS")
		FVector RightOffset;

	class UEnhancedInputComponent* EnhancedInputComponent;

	//virtual void SetupInputComponent() override;

	virtual void OnPossess(APawn* aPawn) override; 

	virtual void OnUnPossess() override;
	void MoveTetroidLeft();

	void MoveTetroidRight();

	void RotateTetroid();

	void SpeedUpTetroid();
	void SpeedDownTetroid();



protected:
	
	virtual void BeginPlay() override;




};
