// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "VSK_MonsterData.generated.h"

class UVSK_Action;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UVSK_MonsterData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn Info")
		TSubclassOf<AActor> MonsterClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn Info")
		TArray<TSubclassOf<UVSK_Action>> Actions;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
		UTexture2D* Icon;

	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("Monsters", GetFName());
	}

};
