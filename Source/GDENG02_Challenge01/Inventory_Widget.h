// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory_Widget.generated.h"

/**
 * 
 */
UCLASS()
class GDENG02_CHALLENGE01_API UInventory_Widget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent)
		void SetCoalText(int32 value);

	UFUNCTION(BlueprintImplementableEvent)
		void SetIronText(int32 value);

	UFUNCTION(BlueprintImplementableEvent)
		void SetLumberText(int32 value);

	UFUNCTION(BlueprintImplementableEvent)
		void SetSteelText(int32 value);

	UFUNCTION(BlueprintImplementableEvent)
		void SetSewingText(int32 value);
};
