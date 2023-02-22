// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


UENUM()
enum BuildingType {
	None	UMETA(DisplayName = "None"),
	CoalMine	UMETA(DisplayName = "Coal Mine"),
	IronMine	UMETA(DisplayName = "Iron Mine"),
	Lumberjack  UMETA(DisplayName = "Lumberjack's hut"),
	Furnace		UMETA(DisplayName = "Furnace"),
	Factory		UMETA(DisplayName = "Factory")
};

/**
 * 
 */
class GDENG02_CHALLENGE01_API BuildingEnum
{
public:
	BuildingEnum();
	~BuildingEnum();

};
