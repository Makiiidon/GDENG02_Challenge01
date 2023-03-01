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
		//UE_LOG(LogTemp, Warning, TEXT("Number of Items in Inventory is %d"), ItemList.Num());

		if (!UnloadRequest) {
			// Movement
			if (MoveRequest) {
				if (Target == None) {
					if (!RequestQueue.IsEmpty()) { // Controls the Queue
						Target = RequestQueue[0];
						//UE_LOG(LogTemp, Warning, TEXT("%s has been added to the queue!"), *UEnum::GetValueAsString(RequestQueue[0]));
						
						RequestQueue.RemoveAt(0);
						ComputeTravel();
						Speed = Distance / TravelTime;

					}
					else {
						UpdateQueue();
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
					if (Target == CoalMine) {
						Target = None;
						// Add To the Vehicle
						CoalMineReference->Unload(1);
						Load(Coal, 1);
					}
					else if (Target == IronMine) {
						Target = None;
						IronMineReference->Unload(1);
						Load(Iron, 1);
					}
					else if (Target == Lumberjack) {
						Target = None;
						LumberjackReference->Unload(1);
						Load(Lumber, 1);
					}
					else if (Target == Furnace) {
						Target = None;
						if (FurnaceReference->HasOutput()) {
							FurnaceReference->Unload(1);
							Load(Steel, 1);

						}else if (ItemList.Contains(Coal)) {// Remove From Vehicle and add to building
							FurnaceReference->Load(Unload(Coal), Coal);

						}
						else if (ItemList.Contains(Iron)) {
							FurnaceReference->Load(Unload(Iron), Iron);

						}
						


					}
					else if (Target == Factory) {
						// Remove From Vehicle and add to building
						Target = None;
						if (ItemList.Contains(Lumber)) {
							FactoryReference->Load(Unload(Lumber), Lumber);

						}
						else if (ItemList.Contains(Steel)) {					
							FactoryReference->Load(Unload(Steel), Steel);

						}
						else {
							DropItems();
						}
						
					}


					UnloadRequest = true;
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

void AVehicleActor::UpdateQueue()
{
	if ((FactoryReference->IsBuildingEmpty() && (ItemList.Contains(Lumber)) ||
		(FactoryReference->IsBuildingEmpty() && ItemList.Contains(Steel)))) {
		RequestQueue.Add(Factory);
		//UE_LOG(LogTemp, Warning, TEXT("2Factory Has been Added to queue"));

	}
	else if ((FurnaceReference->IsOutputFull() && !FactoryReference->DoesContainItem(Steel)) ||
		(ItemList.Contains(Coal) || ItemList.Contains(Iron))) {

		RequestQueue.Add(Furnace);
		//UE_LOG(LogTemp, Warning, TEXT("2Furnace Has been Added to queue"));
	}
	else if (LumberjackReference->IsOutputFull() && !FactoryReference->DoesContainItem(Lumber)) {
		RequestQueue.Add(Lumberjack);
		//UE_LOG(LogTemp, Warning, TEXT("2Lumber Has been Added to queue"));

	}
	else if (CoalMineReference->IsOutputFull() && !FurnaceReference->DoesContainItem(Coal)) {
		RequestQueue.Add(CoalMine);
		//UE_LOG(LogTemp, Warning, TEXT("2Coal Has been Added to queue"));
	}
	else if (IronMineReference->IsOutputFull() && !FurnaceReference->DoesContainItem(Iron)) {
		RequestQueue.Add(IronMine);
		//UE_LOG(LogTemp, Warning, TEXT("2Iron Has been Added to queue"));

	}
	ComputeTravel();
	Speed = Distance / TravelTime;
}

void AVehicleActor::Move(bool canMove, BuildingType Type)
{
	MoveRequest = canMove;
	Target = Type;
}

int AVehicleActor::Unload(ItemType Item)
{
	if (ItemList.Contains(Item)) {
		return ItemList.Remove(Item);
	}
	else {
		return 0;
	}
}

void AVehicleActor::Load(ItemType Item, int AmountIn)
{
	if (AmountIn + ItemList.Num() <= MaxCapacity) {
			ItemList.Add(Item);
	}
}

void AVehicleActor::DropItems()
{
	ItemList.Empty();
}


bool AVehicleActor::GetLoadRequest()
{
	return UnloadRequest;
}

bool AVehicleActor::DoesHaveInQueue(BuildingType building)
{
	return RequestQueue.Contains(building);
}

bool AVehicleActor::DoesHaveItemInList(ItemType item)
{
	return ItemList.Contains(item);
}

bool AVehicleActor::IsItemListEmpty()
{
	return ItemList.IsEmpty();
}

bool AVehicleActor::IsFullInCapacity()
{
	return MaxCapacity == ItemList.Num();
}


void AVehicleActor::AddToQueue(BuildingType ActualRequest)
{
	//UE_LOG(LogTemp, Warning, TEXT("Added to queue"));
	RequestQueue.Push(ActualRequest);
}


void AVehicleActor::SetItemList(TArray<ItemType> list)
{
	ItemList = list;
}

BuildingType AVehicleActor::GetTarget()
{
	return Target;
}


void AVehicleActor::SetTarget(BuildingType Location)
{
	Target = Location;
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

