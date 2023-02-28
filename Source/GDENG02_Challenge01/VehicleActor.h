// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingEnum.h"
#include "BuildingActor.h"
#include "VehicleActor.generated.h"

UCLASS()
class GDENG02_CHALLENGE01_API AVehicleActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVehicleActor();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Move(bool canMove, BuildingType Type);
	void Unload(ItemType Item);
	void Load(ItemType Item, int AmountIn);
	void SetLoadRequest(bool value);

private:
	UPROPERTY(EditAnywhere)
		TEnumAsByte<BuildingType> Target;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
		float RotationOffset;

	UPROPERTY(EditAnywhere)
		float MinTravelTime;

	UPROPERTY(EditAnywhere)
		float MaxTravelTime;

	UPROPERTY(EditAnywhere)
		float MinLoadingTime;

	UPROPERTY(EditAnywhere)
		float MaxLoadingTime;

	float LoadingTime;

	UPROPERTY(EditAnywhere)
		int MaxCapacity;

	UPROPERTY(EditAnywhere)
		AActor* CoalMineLocation;

	UPROPERTY(EditAnywhere)
		AActor* IronMineLocation;
	
	UPROPERTY(EditAnywhere)
		AActor* LumberjackLocation;
	
	UPROPERTY(EditAnywhere)
		AActor* FurnaceLocation;

	UPROPERTY(EditAnywhere)
		AActor* FactoryLocation;

	UPROPERTY(EditAnywhere)
		ABuildingActor* CoalMineReference;

	UPROPERTY(EditAnywhere)
		ABuildingActor* IronMineReference;

	UPROPERTY(EditAnywhere)
		ABuildingActor* LumberjackReference;

	UPROPERTY(EditAnywhere)
		ABuildingActor* FurnaceReference;

	UPROPERTY(EditAnywhere)
		ABuildingActor* FactoryReference;

	AActor* Destination;

	float TravelTime;
	float Ticks = 0.0f;

	bool UnloadRequest;
	bool MoveRequest;

	TQueue<ItemType> ItemList;
};
