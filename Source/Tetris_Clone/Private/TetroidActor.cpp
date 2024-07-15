// Fill out your copyright notice in the Description page of Project Settings.


#include "TetroidActor.h"

constexpr auto Constant_50 = 70.0f;
constexpr auto Constant_100 = 120.0f;

// Sets default values
ATetroidActor::ATetroidActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));

	Cubes.SetNum(4);

	for (int i = 0; i < 4; i++)
	{
		FName ComponentName = FName(FString::Printf(TEXT("Cube - %d"), i));

		Cubes[i] = CreateDefaultSubobject<UStaticMeshComponent>(ComponentName);
		Cubes[i]->SetupAttachment(RootComponent);
	}

}

// Called when the game starts or when spawned
void ATetroidActor::BeginPlay()
{
	Super::BeginPlay();
	
	ShapeNumber = FMath::RandRange(0, 4);
	MakeShape(ShapeNumber);
}

// Called every frame
void ATetroidActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void ATetroidActor::MakeShape(int16 number)
{

	UE_LOG(LogTemp, Display, TEXT("Shape Number is %d"), number);
	switch (number)
	{
	case 0:
	{// Vertical Shape
		offset = FVector(0, 0, 0);
		for (UStaticMeshComponent* cubes : Cubes)
		{
			cubes->AddLocalOffset(offset, true);
			offset.Z += Constant_50;
		}
		UE_LOG(LogTemp, Display, TEXT("case 0 called"));
	}
	break;

	case 1:// box Shape
	{
		Cubes[0]->AddLocalOffset(FVector(0, 0, 0));
		Cubes[1]->AddLocalOffset(FVector(Constant_50, 0, 0));
		Cubes[2]->AddLocalOffset(FVector(Constant_50, 0, Constant_50));
		Cubes[3]->AddLocalOffset(FVector(0, 0, Constant_50));

		UE_LOG(LogTemp, Display, TEXT("case 1 called"));
	}

	break;

	case 2: // L Shape
	{
		offset = FVector(0, 0, 0);
		int16 i = 0;
		for (UStaticMeshComponent* cubes : Cubes)
		{
			if (i == 3)
			{
				cubes->AddLocalOffset(FVector(Constant_100, 0, Constant_50));
			}
			else
			{
				cubes->AddLocalOffset(offset);
				offset.X += Constant_50;
			}
			i++;
		}
		UE_LOG(LogTemp, Display, TEXT("case 2 called"));
	}
	break;
	case 3: // S Shape
	{

		Cubes[0]->AddLocalOffset(FVector(0, 0, 0));
		Cubes[1]->AddLocalOffset(FVector(0, 0, -Constant_50));
		Cubes[2]->AddLocalOffset(FVector(-Constant_50, 0, -Constant_50));
		Cubes[3]->AddLocalOffset(FVector(-Constant_50, 0, -Constant_100));

		UE_LOG(LogTemp, Display, TEXT("case 3 called"));
	}

	break;

	case 4: // T Shape
	{
		offset = FVector(0, 0, 0);
		int16 i = 0;
		for (UStaticMeshComponent* cubes : Cubes)
		{
			if (i == 3)
			{
				cubes->AddLocalOffset(FVector(Constant_50, 0, Constant_50));
			}
			else
			{
				cubes->AddLocalOffset(offset);
				offset.X += Constant_50;
			}
			i++;
		}
		UE_LOG(LogTemp, Display, TEXT("case 4 called"));
	}
	break;

	default:
		break;
	}
}

void ATetroidActor::SelectRandomColor()
{

}

USceneComponent* ATetroidActor::GetRootComponent()
{
	return RootComponent;
}