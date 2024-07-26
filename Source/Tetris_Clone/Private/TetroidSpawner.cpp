// Fill out your copyright notice in the Description page of Project Settings.

#include "TetroidSpawner.h"
#include "Misc/OutputDeviceNull.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "BPStaticTetroidActor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TetroidActor.h"

// Sets default values
ATetroidSpawner::ATetroidSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));

	SpawnVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnVolume"));

	SpawnVolume->AttachToComponent(RootComponent,FAttachmentTransformRules::SnapToTargetIncludingScale);

	int16 i = 4;

	TraceStart.SetNum(i);
	TraceEnd.SetNum(i);
	Hit.SetNum(i);


	MyObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));

	ClassFilter = AStaticMeshActor::StaticClass();

	StartLocations.SetNum(i);
	LeftDirections.SetNum(i);
	RightDirections.SetNum(i);
	EndLocationsLeft.SetNum(i);
	EndLocationsRight.SetNum(i);

	HitResultLeft.SetNum(i);
	HitResultRight.SetNum(i);
}

// Called when the game starts or when spawned
void ATetroidSpawner::BeginPlay()
{ 
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ATetroidSpawner::HandleTetroid, rate, false);
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
/*void ATetroidSpawner::CallBlueprintFunction()
{
	FOutputDeviceNull ar;
	const FString cmd = FString::Printf(TEXT("CheckLines"));

	if(CheckLineBlueprint) CheckLineBlueprint->CallFunctionByNameWithArguments(*cmd, ar, nullptr, true); 
	else
	{
		UE_LOG(LogTemp, Display, TEXT("blueprint function wasn't called"));
	}

}*/

void ATetroidSpawner::setrate(float Ratetoset)
{
	rate = Ratetoset;
}
FVector ATetroidSpawner::GetSnappedVector(FVector TVector)
{
	return UKismetMathLibrary::Vector_SnappedToGrid(TVector, GridSize);
}

void ATetroidSpawner::MoveTetroidToGrid(FVector MoveTo)
{	

	if(tetroid) tetroid->AddActorWorldOffset(GetSnappedVector(MoveTo));
}

void ATetroidSpawner::CheckAndMove(FVector MoveTo)
{

	MoveTetroidToGrid(MoveTo);

	if (CheckMove())
	{
		FVector offset = (MoveTo * -1);
		MoveTetroidToGrid(offset);
	}
}

FVector ATetroidSpawner::GetSpawnPoint()
{
	const FVector SpawnOrigin = SpawnVolume->Bounds.Origin;
	const FVector SpawnExtent = SpawnVolume->Bounds.BoxExtent;

	FVector RandBound = UKismetMathLibrary::RandomPointInBoundingBox(SpawnOrigin, SpawnExtent);

	return GetSnappedVector(RandBound);
}

void ATetroidSpawner::SpawnTetroid()
{		
		if (!ensure(MytetroidActor != NULL)) return;

		tetroid = GetWorld()->SpawnActor<ATetroidActor>(MytetroidActor, GetSpawnPoint(), FRotator::ZeroRotator);

		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("spawned"));

		return;
}

void ATetroidSpawner::DoLineTrace()
{
	if (tetroid == nullptr) return;
	TArray<UStaticMeshComponent*> CubeChildren;

	tetroid->GetComponents <UStaticMeshComponent>(CubeChildren);
	int16 i = 0;
	for (UStaticMeshComponent* cubes : CubeChildren)
	{

		TraceStart[i] = cubes->GetComponentLocation() + StartOffset;
		TraceEnd[i] = cubes->GetComponentLocation() + EndOffset;
		CollisionQuery.AddIgnoredActor(tetroid);

		GetWorld()->LineTraceSingleByChannel(Hit[i], TraceStart[i], TraceEnd[i], ECollisionChannel::ECC_WorldStatic, CollisionQuery);

		i++;
	}
}

bool ATetroidSpawner::CheckMove()
{
	int16 i = 0;
	bool bHit[4] = {};

	TArray<UStaticMeshComponent*> ChildTetroids;
	ChildTetroids.SetNum(i);

	tetroid->GetComponents<UStaticMeshComponent>(ChildTetroids);

	TArray<AActor*> OutActor;

	for (UStaticMeshComponent* Cubes : ChildTetroids)
	{
		FVector BoxCenter = Cubes->GetComponentLocation();
		FVector BoxExtent(20.0f, 20.0f, 20.0f);

		bHit[i] = UKismetSystemLibrary::BoxOverlapActors(GetWorld(), BoxCenter, BoxExtent, MyObjectTypes,ClassFilter,{tetroid},OutActor);

		DrawDebugBox(GetWorld(), BoxCenter, BoxExtent, FColor::Blue, false, 2.0f);
		++i;
	}
	bool result = (bHit[0] || bHit[1] || bHit[2] || bHit[3]);

	return result;
	
}
bool ATetroidSpawner::CheckRotate(float checkdistance)
{
		int16 i = 0;
		bool bHitLeft[4] = {};
		bool bHitRight[4] = {};

		TArray<UStaticMeshComponent*> ChildTetroids;
		ChildTetroids.SetNum(i);

		tetroid->GetComponents<UStaticMeshComponent>(ChildTetroids);

		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(tetroid);

		for (UStaticMeshComponent* Cubes : ChildTetroids)
		{
			StartLocations[i] = Cubes->GetComponentLocation();
			LeftDirections[i] = Cubes->GetForwardVector();
			RightDirections[i] = -1 * Cubes->GetForwardVector();
			EndLocationsLeft[i] = StartLocations[i] + (LeftDirections[i] * checkdistance);
			EndLocationsRight[i] = StartLocations[i] + (RightDirections[i] * checkdistance);

			bHitLeft[i] = GetWorld()->LineTraceSingleByChannel(HitResultLeft[i], StartLocations[i], EndLocationsLeft[i], ECC_Visibility, CollisionParams);
			bHitRight[i] = GetWorld()->LineTraceSingleByChannel(HitResultRight[i], StartLocations[i], EndLocationsRight[i], ECC_Visibility, CollisionParams);

			DrawDebugLine(GetWorld(), StartLocations[i], EndLocationsLeft[i], FColor::Blue, false, 1.0f, 0, 5.0f);
			DrawDebugLine(GetWorld(), StartLocations[i], EndLocationsRight[i], FColor::Blue, false, 1.0f, 0, 5.0f);

			++i;
		}

		bool resultleft = (bHitLeft[0] || bHitLeft[1] || bHitLeft[2] || bHitLeft[3]);
		bool resultRight = (bHitRight[0] || bHitRight[1] || bHitRight[2] || bHitRight[3]);

		bool result = (resultleft || resultRight);

		return !result;
}
void  ATetroidSpawner::RotateTetroid()
{	
	Rotation.Pitch += 90;

	if (CheckRotate(50.0f))
	{
		tetroid->AddActorWorldRotation(Rotation.Quaternion());
	}
	else Rotation.Pitch -= 90.0f;

	if (Rotation.Pitch == 360) Rotation.Pitch = 0;

	UE_LOG(LogTemp, Display, TEXT("%s"),*Rotation.ToString());

}

void ATetroidSpawner::MoveTetroid()
{
	
	DoLineTrace();
	MoveTetroidToGrid(FVector(0.0f,0.0f,MoveOffset));
}

// Handles whole lifetime of Tetroid(falling down to colliding with surface/another tetroid)
void ATetroidSpawner::HandleTetroid()
{
	if (!isFalling)
	{
		SpawnTetroid();

		isFalling = true;
		isOnGround = false;
		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ATetroidSpawner::HandleTetroid, rate, false);
		return;
	}
	else if (ensure(tetroid != nullptr))
	{
		MoveTetroid();
		for (FHitResult& hits : Hit)
		{
			if (hits.bBlockingHit)
			{
				UE_LOG(LogTemp, Display, TEXT("Blocked"));
				isFalling = false;
				isOnGround = true;
			}
		}
		if (isOnGround)
		{
			TArray<UStaticMeshComponent*> Cubes;
			tetroid->GetComponents<UStaticMeshComponent>(Cubes);
			
				for (UStaticMeshComponent* cubes : Cubes)
				{	
					FVector Location = cubes->GetComponentLocation();
					ABPStaticTetroidActor* StaticTetroid =  GetWorld()->SpawnActor<ABPStaticTetroidActor>(MyStaticTetroidActor, Location, FRotator::ZeroRotator);
					StaticTetroid->GetComponentByClass<UStaticMeshComponent>()->SetMaterial(0, tetroid->GetDynMaterial());
				}
				CallBlueprintFunction();
				GetWorld()->DestroyActor(tetroid);

				GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ATetroidSpawner::HandleTetroid, rate, false);
		}

		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ATetroidSpawner::HandleTetroid, rate, false);

	}

}