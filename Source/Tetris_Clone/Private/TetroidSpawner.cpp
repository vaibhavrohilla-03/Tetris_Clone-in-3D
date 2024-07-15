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

	TraceStart.SetNum(4);
	TraceEnd.SetNum(4);
}

// Called when the game starts or when spawned
void ATetroidSpawner::BeginPlay()
{ 
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ATetroidSpawner::HandleTetroid, 0.5f, true);
}

// Called every frame
void ATetroidSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (int16 i = 0; i < 4; i++)
	{	if( (TraceStart.IsValidIndex(i) && TraceEnd.IsValidIndex(i)) && 
			(TraceStart[i] != FVector::ZeroVector && TraceEnd[i] != FVector::ZeroVector)  )
		{

		DrawDebugLine(GetWorld(), TraceStart[i], TraceEnd[i], FColor::Red);
		
		}
		
	}

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
		if (!ensure(MytetroidActor != NULL)) return;

		FRotator TetroidRotation;
		TetroidRotation.Roll = 0.0f;
		TetroidRotation.Pitch = 0.0f;       // for random rotation ---> FMath::RandRange(0, 4) * 90.0f;
		TetroidRotation.Yaw = 0.0f;

		tetroid = GetWorld()->SpawnActor<ATetroidActor>(MytetroidActor, GetSpawnPoint(), TetroidRotation);

		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("spawned"));

		return;
			/*for (int16 j = 0; j < 4; j++)
			{
				FString Debugmessage = FString::Printf(TEXT("trace start  => %s"), *TraceStart[j].ToString());
				FString Debugmessageone = FString::Printf(TEXT("trace End  => %s"), *TraceEnd[j].ToString());
				FString Debugmessagetwo = FString::Printf(TEXT("hit result %s"), Hit.bBlockingHit ? TEXT("true") : TEXT("false"));
				//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, Debugmessage);
				//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, Debugmessageone);
				//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, Debugmessagetwo);
			}
			

			if (Hit.bBlockingHit && IsValid(Hit.GetActor()))
			{
				isFalling = false;
				isOnGround = true;
				GEngine->AddOnScreenDebugM*/
}

void ATetroidSpawner::MoveTetroid(ATetroidActor* Tetroid)
{
	if (Tetroid == nullptr) return;


	TArray<UStaticMeshComponent*> CubeChildren;

	Tetroid->GetComponents <UStaticMeshComponent>(CubeChildren);
	int16 i = 0;
	for (UStaticMeshComponent* cubes : CubeChildren)
	{

		TraceStart[i] = cubes->GetComponentLocation();

		TraceEnd[i] = cubes->GetComponentLocation() + FVector(0.0f, 0.0f, 50.0f);

		FString DebugMessage = FString::Printf(TEXT("TraceStart ->  %s"), *TraceStart[i].ToString());
		FString DebugMessage_two = FString::Printf(TEXT("TraceEnd ->  %s"), *TraceEnd[i].ToString());
		GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Red, DebugMessage);
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Red, DebugMessage_two);

		CollisionQuery.AddIgnoredActor(Tetroid);

		GetWorld()->LineTraceSingleByChannel(Hit, TraceStart[i], TraceEnd[i], ECollisionChannel::ECC_WorldStatic, CollisionQuery,FCollisionResponseParams());

		i++;
	}

	Tetroid->AddActorLocalOffset(FVector(0.0f, 0.0f, -50.0f));
}

void ATetroidSpawner::HandleTetroid()
{
	if (!isFalling)
	{
		SpawnTetroid();

		isFalling = true;
		isOnGround = false;
		return;
	}
	else if (ensure(tetroid != nullptr))
	{
		MoveTetroid(tetroid);

		if (Hit.bBlockingHit)
		{
			AActor* hitactor = Hit.GetActor();	
			isFalling = false;
			isOnGround = true;
		}
	}
}
