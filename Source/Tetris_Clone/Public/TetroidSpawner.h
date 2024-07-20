// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TetroidSpawner.generated.h"

UCLASS()
class TETRIS_CLONE_API ATetroidSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATetroidSpawner();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	
	UPROPERTY(EditAnywhere, Category = "Spawner")
		TObjectPtr<class UBoxComponent> SpawnVolume;

	UPROPERTY(EditAnywhere, Category = "Class_To_Spawn")
		TSubclassOf<class ATetroidActor> MytetroidActor;

		UPROPERTY(EditAnywhere, Category = "Class_To_Spawn")
		TSubclassOf<class ABPStaticTetroidActor> MyStaticTetroidActor;


	UPROPERTY(EditAnywhere, Category = "TraceLineOffset")
		FVector StartOffset;

	UPROPERTY(EditAnywhere, Category = "TraceLineOffset")
		FVector EndOffset;

	UPROPERTY(EditAnywhere, Category = "MoveOffset")
		float MoveOffset = -50.0f;

	UPROPERTY(EditAnywhere, Category = "Gridsize")
		float GridSize = 50.0f;
	
	UFUNCTION()
		void SpawnTetroid();

	UFUNCTION()
		void MoveTetroid(class ATetroidActor* Tetroid);

	UFUNCTION()
	FVector GetSpawnPoint();

	UFUNCTION()
		void HandleTetroid();

	UFUNCTION()
	FVector GetSnappedVector(FVector TVector);

	UFUNCTION()
	void MoveTetroidToGrid(ATetroidActor* tetroid, FVector MoveTo);

	FVector SpawnLocation;

	class ATetroidActor* tetroid;

	FTimerHandle SpawnTimerHandle;

	bool isFalling = false;

	bool isOnGround = false;

	TArray<FHitResult> Hit;

	TArray<FVector> TraceStart;

	TArray<FVector> TraceEnd;

	

	FCollisionQueryParams CollisionQuery;
};
