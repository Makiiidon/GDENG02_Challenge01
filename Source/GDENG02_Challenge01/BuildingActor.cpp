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
		CanUseInventory = true;
		UE_LOG(LogTemp, Warning, TEXT("Furnace"));
	}
	else if (Type == Factory) {
		Input = 0;
		OutputCapacity = 1000000;
		CanUseInventory = true;
		UE_LOG(LogTemp, Warning, TEXT("Factory"));
	}

	IsFull = false;

}

// Called every frame
void ABuildingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Mesh != NULL) {
		if (Input != 0) {
			if (Type == Furnace) {
				for (int i = 0; i < Inventory.Num(); i++) {
					UE_LOG(LogTemp, Warning, TEXT("For the Furnace... Input: %d, Output: %d "), Inventory[i], Output);
				}
			}
			if (Output < OutputCapacity) {
				if (Timer <= 0.0f) {
					if (CanUseInventory) {
						if (Type == Furnace) {
							int CoalFlag = -1;
							int IronFlag = -1;
							for (int i = 0; i < Inventory.Num(); i++) {
								if (Inventory[i] == Coal) {
									CoalFlag = i;
								}
								else if (Inventory[i] == Iron) {
									IronFlag = i;
								}
							}
							if (CoalFlag != -1 && IronFlag != -1) {
								Inventory.Remove(Coal);
								Inventory.Remove(Iron);
								Input -= 2;
								Output++;
							}

						}
						else if (Type == Factory) {
							int LumberFlag = -1;
							int SteelFlag = -1;
							for (int i = 0; i < Inventory.Num(); i++) {
								if (Inventory[i] == Lumber) {
									LumberFlag = i;
								}
								else if (Inventory[i] == Steel) {
									SteelFlag = i;
								}
							}
							if (LumberFlag != -1 && SteelFlag != -1) {
								Inventory.Remove(Lumber);
								Inventory.Remove(Steel);
								Input -= 2;
								Output++;
							}
						}
					}
					else {
						Input--;
						Output++;
					}

					Timer = INTERVAL;
					//UE_LOG(LogTemp, Warning, TEXT("Input: %d, Output: %d "), Input, Output);
				}
				else {
					Timer -= DeltaTime;
					IsFull = false;
					
				}
			}
			else { // Building is full, therefore call vehicle
				IsFull = true;
				IsOutputFull();
			}
		}
		else {
			//UE_LOG(LogTemp, Warning, TEXT("Building has no Input!"));
		}
		
	}
}

int ABuildingActor::Unload(int amount)
{
	Output -= amount;
	return amount;
}

void ABuildingActor::Load(int amount, ItemType type)
{
	if (CanUseInventory) {
		if (Type == Furnace) {
			if (type == Coal || type == Iron) {
				for (int i = 0; i < amount; i++) {
					Inventory.Add(type);
				}
			}
		}
		else if (Type == Factory) {
			if (type == Lumber || type == Steel) {
				for (int i = 0; i < amount; i++) {
					Inventory.Add(type);
				}
			}
		}
		Input = amount;
	}
}

bool ABuildingActor::IsOutputFull()
{
	return IsFull;
}

bool ABuildingActor::IsBuildingEmpty()
{
	return IsEmpty;
}

bool ABuildingActor::DoesContainItem(ItemType itemType)
{
	if (Inventory.Contains(itemType)) {
		return true;
	}
	else {
		return false;
	}
}

