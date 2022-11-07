// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "Engine/DataTable.h"
#include "VSK_GameModeBase.generated.h"

class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;
class UVSK_SaveGame;
class UDataTable;
class UVSK_MonsterData;

USTRUCT(BlueprintType)
struct FMonsterInfoRow : public FTableRowBase
{
	GENERATED_BODY()

public:

	FMonsterInfoRow()
	{
		Weight = 1.0f;
		SpawnCost = 5.0f;
		KillReward = 20.0f;
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FPrimaryAssetId MonsterId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float Weight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float SpawnCost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float KillReward;
};

UCLASS()
class ACTIONROGUELIKE_API AVSK_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:

	FString SlotName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		UDataTable* MonsterTable;

	UPROPERTY()
		UVSK_SaveGame* CurrentSaveGame;

	//UPROPERTY(EditDefaultsOnly, Category = "AI")
	//	TSubclassOf<AActor> MinionClass;

	UPROPERTY(EditDefaultsOnly,Category = "AI")
		UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		UEnvQuery* PowerupSpawnQuery;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		UCurveFloat* DifficultyCurve;

	FTimerHandle TimerHandle_SpawnBots;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		float SpawnTimerInterval;

	UFUNCTION()
		void SpawnBotTimerElapsed();

	UFUNCTION()
		void RespawnPlayerElapsed(AController* Controller);

	UFUNCTION()
		void OnSpawnBotQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
		void OnPowerupSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	void OnMonsterLoaded(FPrimaryAssetId LoadedId, FVector SpawnLocation);

	UPROPERTY(EditAnywhere, Category = "Powerups")
		float CreditsPerKill;

	UPROPERTY(EditAnywhere, Category = "Powerups")
		float DesiredPowerupCount;

	UPROPERTY(EditAnywhere, Category = "Powerups")
		float RequiredPowerupDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
		TArray<TSubclassOf<AActor>> PowerupClasses;

public:

	AVSK_GameModeBase();

	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void StartPlay() override;

	void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)override;

	UFUNCTION(Exec)
	void KillAll();

	UFUNCTION(BlueprintCallable,Category="SaveGame")
	void WriteSaveGame();

	void LoadSaveGame();


	
};
