// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingActor.h"

// Sets default values
ABuildingActor::ABuildingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;

}

// Called when the game starts or when spawned
void ABuildingActor::BeginPlay()
{
	Super::BeginPlay();
	Timer = INTERVAL;

	if (Type == CoalMine) {
		Input = 1000000;
		Output = 0;
		UE_LOG(LogTemp, Warning, TEXT("Coal Mine"));
	}
	else if (Type == IronMine) {
		Input = 1000000;
		Output = 0;
		UE_LOG(LogTemp, Warning, TEXT("Iron Mine"));
	}
	else if (Type == Lumberjack) {
		Input = 1000000;
		Output = 0;
		UE_LOG(LogTemp, Warning, TEXT("LumberJack"));
	}
	else if (Type == Furnace) {
		Input = 0;
		Output = 0;
		UE_LOG(LogTemp, Warning, TEXT("Furnace"));
	}
	else if (Type == Factory) {
		Input = 0;
		OutputCapacity = 1000000;
		UE_LOG(LogTemp, Warning, TEXT("Factory"));
	}
}

// Called every frame
void ABuildingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Mesh != NULL) {
		if (Input != 0) {
			if (Output < OutputCapacity) {
				if (Timer <= 0.0f) {
					Input--;
					Output++;
					Timer = INTERVAL;
					UE_LOG(LogTemp, Warning, TEXT("Input: %d, Output: %d "), Input, Output);
				}
				else {
					Timer -= DeltaTime;
				}
			}
		}
		else {
			//UE_LOG(LogTemp, Warning, TEXT("Building has no Input!"));
		}
	}
	
}

BuildingType ABuildingActor::Unload(int amount)
{
	Input += amount;
	Output -= amount;
	return Type;
}

