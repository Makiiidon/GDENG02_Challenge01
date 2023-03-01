// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"

// Sets default values
AGameManager::AGameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGameManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CoalReference		!= nullptr &&
		IronReference		!= nullptr &&
		LumberjackReference != nullptr &&
		FurnaceReference	!= nullptr &&
		FactoryReference	!= nullptr) {


		

		for (int i = 0; i < VehicleReferences.Num(); i++) {
			if (VehicleReferences[i]->IsFullInCapacity()) {
			
				RequestQueue.Empty();

				// Checks if The Vehicle has a material
				bool FactoryCondition = false;
				bool FurnaceCondition = false;

				FactoryCondition = (VehicleReferences[i]->DoesHaveItemInList(Steel) && !FactoryReference->DoesContainItem(Steel)) ||
					(VehicleReferences[i]->DoesHaveItemInList(Lumber) && !FactoryReference->DoesContainItem(Lumber));

				FurnaceCondition = (VehicleReferences[i]->DoesHaveItemInList(Coal) && !FurnaceReference->DoesContainItem(Coal)) ||
					(VehicleReferences[i]->DoesHaveItemInList(Iron) && !FurnaceReference->DoesContainItem(Iron));


				if (FactoryCondition) {
					QUEUE temp;
					temp.type = Factory;
					temp.vechicle = VehicleReferences[i];
					RequestQueue.Push(temp);

				}
				else if (FurnaceCondition) {
					QUEUE temp;
					temp.type = Furnace;
					temp.vechicle = VehicleReferences[i];
					RequestQueue.Push(temp);

				}
			}
			else {
				// Checks if any resource has an output
				IsCoalFull = CoalReference->HasOutput()			&& !VehicleReferences[i]->DoesHaveItemInList(Coal);
				IsIronFull = IronReference->HasOutput()			&& !VehicleReferences[i]->DoesHaveItemInList(Iron);
				IsLumberFull = LumberjackReference->HasOutput() && !VehicleReferences[i]->DoesHaveItemInList(Lumber);
				IsSteelFull = FurnaceReference->HasOutput()		&& !VehicleReferences[i]->DoesHaveItemInList(Steel);

				if (IsSteelFull) {
					QUEUE temp;
					temp.type = Furnace;
					temp.vechicle = VehicleReferences[i];
					RequestQueue.Push(temp); IsSteelFull = false;
				}
				else if (IsCoalFull) {
					QUEUE temp;
					temp.type = CoalMine;
					temp.vechicle = VehicleReferences[i];
					RequestQueue.Push(temp); IsCoalFull = false;

				}
				else if (IsIronFull) {
					QUEUE temp;
					temp.type = IronMine;
					temp.vechicle = VehicleReferences[i];
					RequestQueue.Push(temp); IsIronFull = false;

				}
				else if (IsLumberFull) {
					QUEUE temp;
					temp.type = Lumberjack;
					temp.vechicle = VehicleReferences[i];
					RequestQueue.Push(temp); IsLumberFull = false;

				}
				
			}

			
			

			
		}

		for (int i = 0; i < RequestQueue.Num(); i ++) {
			QUEUE temp;

			if (VehicleReferences[0] == RequestQueue[i].vechicle) {
				if (!VehicleReferences[0]->DoesHaveInQueue(RequestQueue[0].type)) {
					if (!VehicleReferences[1]->DoesHaveInQueue(VehicleReferences[0]->GetTarget())) {
						UE_LOG(LogTemp, Warning, TEXT("RequestQueue[0] = %s"), *UEnum::GetValueAsString(RequestQueue[0].type));
						VehicleReferences[0]->AddToQueue(RequestQueue[0].type);
					}
				}
			}
			if (VehicleReferences[1] == RequestQueue[i].vechicle) {
				if (!VehicleReferences[1]->DoesHaveInQueue(RequestQueue[0].type)) {
					if (!VehicleReferences[0]->DoesHaveInQueue(VehicleReferences[1]->GetTarget())) {
						UE_LOG(LogTemp, Warning, TEXT("RequestQueue[1] = %s"), *UEnum::GetValueAsString(RequestQueue[0].type));
							VehicleReferences[1]->AddToQueue(RequestQueue[0].type);
					}
					
					
				}
			}
			RequestQueue.RemoveAt(0);

		}
		//RequestQueue.Empty();
		
	}
}

