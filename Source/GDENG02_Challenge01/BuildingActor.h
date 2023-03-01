// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingEnum.h"
#include "BuildingActor.generated.h"



UCLASS()
class GDENG02_CHALLENGE01_API ABuildingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildingActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int Unload(int amount);

	void Load(int amount, ItemType type);

	bool IsOutputFull();
	bool HasOutput();
	bool IsBuildingEmpty();
	bool DoesContainItem(ItemType itemType);
	BuildingType GetBuildingType();

	ItemType GetItem();

private:
	UPROPERTY(EditAnywhere)
		TEnumAsByte<BuildingType> Type;

	UPROPERTY(EditAnywhere)
		TEnumAsByte<ItemType> Item;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent * Mesh;

	UPROPERTY(EditAnywhere)
		float INTERVAL = 3.0f;

	UPROPERTY(EditAnywhere)
		int InputCapacity;

	UPROPERTY(EditAnywhere)
		int OutputCapacity;

	TArray<ItemType> Inventory;
	bool CanUseInventory = false;

	float Timer;
	int Input;
	int Output;
	bool IsFull;
	bool IsEmpty = true;
};
