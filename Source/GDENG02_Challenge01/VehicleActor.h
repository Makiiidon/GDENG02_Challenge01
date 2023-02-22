// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingEnum.h"
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

private:
	UPROPERTY(EditAnywhere)
		TEnumAsByte<BuildingType> ItemTypeHeld;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Mesh;

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
		AActor* CoalMineReference;

	UPROPERTY(EditAnywhere)
		AActor* IronMineReference;
	
	UPROPERTY(EditAnywhere)
		AActor* LumberjackReference;
	
	UPROPERTY(EditAnywhere)
		AActor* FurnaceReference;

	UPROPERTY(EditAnywhere)
		AActor* FactoryReference;

	AActor* Destination;

	float TravelTime;
	float Ticks = 0.0f;

	int NumberOfItems;

	bool UnloadRequest;
};
