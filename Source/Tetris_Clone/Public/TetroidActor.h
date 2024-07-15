// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TetroidActor.generated.h"

UCLASS()
class TETRIS_CLONE_API ATetroidActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATetroidActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	USceneComponent* GetRootComponent();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, Category = "Tetroid Cubes")
		TArray<TObjectPtr<UStaticMeshComponent>> Cubes;


	FVector offset = FVector(0, 0, 0);
	void MakeShape(int16 number);

	void SelectRandomColor();

	int32 ShapeNumber;
	
};
