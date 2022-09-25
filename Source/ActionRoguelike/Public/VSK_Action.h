// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "VSK_Action.generated.h"

class UWord;

/**
 * 
 */
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API UVSK_Action : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
		void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
		void StopAction(AActor* Instigator);

	UPROPERTY(EditDefaultsOnly, Category = "Action")
		FName ActionName;

	UWorld* GetWorld()const override;
	
};
