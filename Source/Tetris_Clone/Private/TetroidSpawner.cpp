// Fill out your copyright notice in the Description page of Project Settings.

#include "TetroidSpawner.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "TetroidActor.h"

// Sets default values
ATetroidSpawner::ATetroidSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));

	SpawnVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnVolume"));

	SpawnVolume->AttachToComponent(RootComponent,FAttachmentTransformRules::SnapToTargetIncludingScale);
}

// Called when the game starts or when spawned
void ATetroidSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ATetroidSpawner::SpawnTetroid, 1.0f, true);
}

// Called every frame
void ATetroidSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


FVector ATetroidSpawner::GetSpawnPoint()
{	
	const FVector SpawnOrigin = SpawnVolume->Bounds.Origin;
	const FVector SpawnExtent = SpawnVolume->Bounds.BoxExtent;
	
	RandBound = UKismetMathLibrary::RandomPointInBoundingBox(SpawnOrigin, SpawnExtent);

	return RandBound;
}


void ATetroidSpawner::SpawnTetroid()

{
	float timer_rate = GetWorld()->GetTimerManager().GetTimerRate(SpawnTimerHandle);

	FString DMessage = FString::Printf(TEXT("Timer Rate %f"), timer_rate);


	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, DMessage, true);

	if (!isFalling)
	{
		if (!ensure(MytetroidActor != NULL)) return;

		FRotator TetroidRotation;
		TetroidRotation.Roll = 0.0f;
		TetroidRotation.Pitch = FMath::RandRange(0, 4) * 90.0f;
		TetroidRotation.Yaw = 0.0f;

		tetroid = GetWorld()->SpawnActor<ATetroidActor>(MytetroidActor, GetSpawnPoint(), TetroidRotation);

		isFalling =  true;
		isOnGround = false;
	}
	else
	{
		TArray<UStaticMeshComponent*> CubeChildren;

		tetroid->GetComponents <UStaticMeshComponent>(CubeChildren);

		for (UStaticMeshComponent* cubes : CubeChildren)
		{

			FVector TraceStart = tetroid->GetActorLocation();

			FVector TraceEnd = tetroid->GetActorLocation() + (tetroid->GetActorUpVector() * 1000.0f);

			GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd,ECollisionChannel::ECC_WorldStatic);
			
			DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 5, 1, 3.0f);
			

		}
		if (isFalling && !isOnGround)
		{
			tetroid->AddActorLocalOffset(FVector(0.0f, 0.0f, -50.0f));

			if (Hit.bBlockingHit && IsValid(Hit.GetActor()))
			{
				isFalling = false;
				isOnGround = true;

			}
		}
	}
}



