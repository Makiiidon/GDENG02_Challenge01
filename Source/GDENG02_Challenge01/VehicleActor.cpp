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
	LoadingTime = MinLoadingTime;

	ComputeTravel();
	Speed = Distance / TravelTime;
	//UE_LOG(LogTemp, Warning, TEXT("Speed: %f"), Speed);
}

// Called every frame
void AVehicleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Mesh != NULL) {
		if (!UnloadRequest) {
			// Movement
			if (MoveRequest) {
				if (Target == None) {
					if (!RequestQueue.IsEmpty()) { // Controls the Queue
						Target = *RequestQueue.Peek();
						RequestQueue.Pop();
						ComputeTravel();
						Speed = Distance / TravelTime;
						UE_LOG(LogTemp, Warning, TEXT("%s has been added to the queue!"), *UEnum::GetValueAsString(Target));

					}
					else {
						UpdateQueue();
						UE_LOG(LogTemp, Warning, TEXT("Updated the Queue"));

					}
				}
				

				ComputeTravel();

				FVector Velocity = Direction * Speed;
				FVector location = CurrentLocation + Velocity * DeltaTime;

				FRotator rotation = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, TargetLocation) + FRotator(0, RotationOffset, 0);
				this->SetActorRotation(rotation);
				this->SetActorLocation(location);

				if (Distance < 200) { // For Reaching the target location
					// Unload then go to next in Queue
					// ====================================================================================================== Change into Depending on the Queue
					if (Target == CoalMine) {
						Target = None;
						// Add To the Vehicle
						CoalMineReference->Unload(MaxCapacity-1);
						Load(Coal, MaxCapacity-1);
					}
					else if (Target == IronMine) {
						Target = None;
						IronMineReference->Unload(MaxCapacity-1);
						Load(Iron, MaxCapacity-1);
					}
					else if (Target == Lumberjack) {
						Target = None;
						LumberjackReference->Unload(MaxCapacity-1);
						Load(Lumber, MaxCapacity-1);
					}
					else if (Target == Furnace) {
						Target = None;
						// Remove From Vehicle and add to building
						if (ItemList.Contains(Coal)) {
							int ctr = 0;
							for (int i = 0; i < ItemList.Num(); i++) {
								if (ItemList[i] == Coal) {
									Unload(Coal);
									ctr++;
								}
							}
							FurnaceReference->Load(ctr, Coal);

						}
						else if (ItemList.Contains(Iron)) {
							int ctr = 0;
							for (int i = 0; i < ItemList.Num(); i++) {
								if (ItemList[i] == Iron) {
									Unload(Iron);
									ctr++;
								}
							}
							FurnaceReference->Load(ctr, Iron);
						}


					}
					else if (Target == Factory) {
						// Remove From Vehicle and add to building
						Target = None;
						if (ItemList.Contains(Lumber)) {
							int ctr = 0;
							for (int i = 0; i < ItemList.Num(); i++) {
								if (ItemList[i] == Lumber) {
									Unload(Lumber);
									ctr++;
								}
							}
							FactoryReference->Load(ctr, Lumber);

						}
						else if (ItemList.Contains(Steel)) {
							int ctr = 0;
							for (int i = 0; i < ItemList.Num(); i++) {
								if (ItemList[i] == Steel) {
									Unload(Steel);
									ctr++;
								}
							}
							FactoryReference->Load(ctr, Steel);
						}
						
					}
					// ======================================================================================================

					SetLoadRequest(true);
				}
			}
		}
		else {
			if (LoadingTime <= 0.0f) {
				LoadingTime = MinLoadingTime; // Soon randomize
				UnloadRequest = false;
				MoveRequest = true;

				//UE_LOG(LogTemp, Warning, TEXT("FINISHED LOADING"));
			}
			else {
				LoadingTime -= DeltaTime;
				MoveRequest = false;

			}
			ComputeTravel();

			Distance = FVector::Dist(CurrentLocation, TargetLocation);
			Speed = Distance / TravelTime;
			//UE_LOG(LogTemp, Warning, TEXT("Speed: %f"), Speed);

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
	if (ItemList.Contains(Item)) {
		ItemList.Remove(Item);
		ItemCount--;
	}
}

void AVehicleActor::Load(ItemType Item, int AmountIn)
{
	if (AmountIn + ItemList.Num() <= MaxCapacity) {
		ItemList.Add(Item);
		ItemCount++;
	}
}

void AVehicleActor::UpdateQueue()
{
	if ((FactoryReference->IsBuildingEmpty() && (ItemList.Contains(Lumber)) || 
		(FactoryReference->IsBuildingEmpty() && ItemList.Contains(Steel)))) {
		RequestQueue.Enqueue(Factory);
		UE_LOG(LogTemp, Warning, TEXT("2Factory Has been Added to queue"));

	}
	else if ((FurnaceReference->IsOutputFull() && !FactoryReference->DoesContainItem(Steel)) || 
		(ItemList.Contains(Coal) || ItemList.Contains(Iron))) {

		RequestQueue.Enqueue(Furnace);
		UE_LOG(LogTemp, Warning, TEXT("2Furnace Has been Added to queue"));
	}
	else if (LumberjackReference->IsOutputFull() && !FactoryReference->DoesContainItem(Lumber)) {
		RequestQueue.Enqueue(Lumberjack);
		UE_LOG(LogTemp, Warning, TEXT("2Lumber Has been Added to queue"));

	}
	else if (CoalMineReference->IsOutputFull() && !FurnaceReference->DoesContainItem(Coal)) {
		RequestQueue.Enqueue(CoalMine);
		UE_LOG(LogTemp, Warning, TEXT("2Coal Has been Added to queue"));
	}
	else if (IronMineReference->IsOutputFull() && !FurnaceReference->DoesContainItem(Iron)) {
		RequestQueue.Enqueue(IronMine);
		UE_LOG(LogTemp, Warning, TEXT("2Iron Has been Added to queue"));

	}
	ComputeTravel();
	Speed = Distance / TravelTime;
}

void AVehicleActor::SetLoadRequest(bool value)
{
	UnloadRequest = value;
}

void AVehicleActor::AddToQueue(int Request)
{
	BuildingType ActualRequest;
	if (Request == 1) {
		ActualRequest = CoalMine;
	}
	else if (Request == 2) {
		ActualRequest = IronMine;
	}
	else if (Request == 3) {
		ActualRequest = Lumberjack;
	}
	else if (Request == 4) {
		ActualRequest = Furnace;
	}
	else if (Request == 5) {
		ActualRequest = Factory;
	}
	else {
		ActualRequest = None;
	}

	UE_LOG(LogTemp, Warning, TEXT("Added to queue"));
	if (!RequestQueue.IsEmpty()) {
		if (*RequestQueue.Peek() != ActualRequest) {
			RequestQueue.Enqueue(ActualRequest);
		}
	}
}

void AVehicleActor::ComputeTravel()
{
	CurrentLocation = this->GetActorLocation();

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

	Direction = (TargetLocation - CurrentLocation).GetSafeNormal();
	Distance = FVector::Dist(CurrentLocation, TargetLocation);
}

