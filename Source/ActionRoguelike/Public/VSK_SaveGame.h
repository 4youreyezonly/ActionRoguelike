// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "VSK_SaveGame.generated.h"

USTRUCT()
struct FActorSaveData
{
	GENERATED_BODY()

public:
	UPROPERTY()
		FString ActorName;
	UPROPERTY()
		FTransform Transform;
	UPROPERTY()
		TArray<uint8> ByteData;
};

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UVSK_SaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
		int32 Credits;

	UPROPERTY()
	TArray<FActorSaveData> SaveActors;
};
