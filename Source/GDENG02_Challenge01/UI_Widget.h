// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Widget.generated.h"

/**
 * 
 */
UCLASS()
class GDENG02_CHALLENGE01_API UUI_Widget : public UUserWidget
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintImplementableEvent) void SetCoalValue(int amount);
	UFUNCTION(BlueprintImplementableEvent) void SetIronValue(int amount);
	UFUNCTION(BlueprintImplementableEvent) void SetLumberValue(int amount);
	UFUNCTION(BlueprintImplementableEvent) void SetSteelValue(int amount);
	UFUNCTION(BlueprintImplementableEvent) void SetMachineValue(int amount);
};
