// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleActor.h"
#include "Math/UnrealMathUtility.h"

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
	TravelTime = MinTravelTime;
}

// Called every frame
void AVehicleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Ticks += DeltaTime;

	if (Mesh != NULL) {
		if (UnloadRequest) {
			if (LoadingTime <= 0.0f) {

				LoadingTime = MinLoadingTime; // Soon randomize
			}
			else {
				LoadingTime -= DeltaTime;
			}
		}

		FVector CurrentLocation = this->GetActorLocation();
		FVector TargetLocation;

		if (ItemTypeHeld == CoalMine) {
			TargetLocation = CoalMineReference->GetActorLocation();
		}
		else if (ItemTypeHeld == IronMine) {
			TargetLocation = IronMineReference->GetActorLocation();
		}
		else if (ItemTypeHeld == Lumberjack) {
			TargetLocation = LumberjackReference->GetActorLocation();
		}
		else if (ItemTypeHeld == Furnace) {
			TargetLocation = FurnaceReference->GetActorLocation();
		}
		else if (ItemTypeHeld == Factory) {
			TargetLocation = FactoryReference->GetActorLocation();
		}
		else {
			TargetLocation = CurrentLocation;
		}

		// Movement
		//float Distance = FVector::Dist(CurrentLocation, TargetLocation);
		FVector location = FMath::Lerp(CurrentLocation, TargetLocation, DeltaTime/TravelTime);
		this->SetActorLocation(location);

	}
}

