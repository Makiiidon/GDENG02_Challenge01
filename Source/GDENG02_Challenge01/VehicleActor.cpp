// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleActor.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AVehicleActor::AVehicleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void AVehicleActor::BeginPlay()
{
	Super::BeginPlay();
	UnloadRequest = false;
	MoveRequest = true;
	TravelTime = MinTravelTime;
}

// Called every frame
void AVehicleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Ticks += DeltaTime;

	if (Mesh != NULL) {
		if (!UnloadRequest) {
			FVector CurrentLocation = this->GetActorLocation();
			FVector TargetLocation;

			if (Target == CoalMine) {
				TargetLocation = CoalMineLocation->GetActorLocation();
			}
			else if (Target == IronMine) {
				TargetLocation = IronMineLocation->GetActorLocation();
			}
			else if (Target == Lumberjack) {
				TargetLocation = LumberjackLocation->GetActorLocation();
			}
			else if (Target == Furnace) {
				TargetLocation = FurnaceLocation->GetActorLocation();
			}
			else if (Target == Factory) {
				TargetLocation = FactoryLocation->GetActorLocation();
			}
			else {
				TargetLocation = CurrentLocation;
			}

			// Movement
			if (MoveRequest) {
				FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();
				float Distance = FVector::Dist(CurrentLocation, TargetLocation);
				float Speed = Distance / TravelTime;
				FVector Velocity = Direction * Speed;
				FVector location = CurrentLocation + Velocity * DeltaTime;

				FRotator rotation = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, TargetLocation) + FRotator(0, RotationOffset, 0);
				this->SetActorLocation(location);
				this->SetActorRotation(rotation);

				if (Distance < 100) {
					if (Target == CoalMine) {
						Target = Furnace;
					}
					else if (Target == IronMine) {
						Target = Furnace;
					}
					else if (Target == Lumberjack) {
						Target = Factory;
					}
					else if (Target == Furnace) {
						Target = Factory;
					}
					else if (Target == Factory) {
						int32 RandomValue = FMath::RandRange(1, 2);
						if (RandomValue == 1) {
							Target = CoalMine;
						}
						else {
							Target = IronMine;
						}
					}
					SetLoadRequest(true);
					//UE_LOG(LogTemp, Warning, TEXT("OVERLAPPING IS HAPPENING!!!!"));
				}
			}
		}
		else {
			if (LoadingTime <= 0.0f) {
				LoadingTime = MinLoadingTime; // Soon randomize
				UnloadRequest = false;
				MoveRequest = true;
				UE_LOG(LogTemp, Warning, TEXT("FINISHED LOADING"));

			}
			else {
				LoadingTime -= DeltaTime;
				MoveRequest = false;

			}
			
		}		
	}
}

void AVehicleActor::Move(bool canMove, BuildingType Type)
{
	MoveRequest = canMove;
	Target = Type;
}

void AVehicleActor::Unload(ItemType Item)
{
	if (ItemList.Peek() == &Item) {
		ItemList.Dequeue(Item);
	}
}

void AVehicleActor::Load(ItemType Item, int AmountIn)
{
	if (AmountIn <= MaxCapacity) {
		ItemList.Enqueue(Item);
	}
}

void AVehicleActor::SetLoadRequest(bool value)
{
	UnloadRequest = value;
}

