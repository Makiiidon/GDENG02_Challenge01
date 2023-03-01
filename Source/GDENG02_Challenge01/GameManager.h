// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingActor.h"
#include "BuildingEnum.h"
#include "VehicleActor.h"
#include "GameManager.generated.h"

struct QUEUE {
	BuildingType type = None;
	AVehicleActor* vechicle = nullptr;
};

UCLASS()
class GDENG02_CHALLENGE01_API AGameManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Widgets")
		TSubclassOf<class UUserWidget> WidgetClass;

	UPROPERTY()
		UInventory_Widget* Widget;

	void UpdateTextValue();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere) TArray<AVehicleActor*> VehicleReferences;

	UPROPERTY(EditAnywhere) ABuildingActor* CoalReference;
	UPROPERTY(EditAnywhere) ABuildingActor* IronReference;
	UPROPERTY(EditAnywhere) ABuildingActor* LumberjackReference;
	UPROPERTY(EditAnywhere) ABuildingActor* FurnaceReference;
	UPROPERTY(EditAnywhere) ABuildingActor* FactoryReference;

	bool IsCoalFull = false;
	bool IsIronFull = false;
	bool IsLumberFull = false;
	bool IsSteelFull = false;

	TArray<QUEUE> RequestQueue;
};
