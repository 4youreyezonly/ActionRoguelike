// Fill out your copyright notice in the Description page of Project Settings.


#include "VSK_GameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "AI/VSK_AICharacter.h"
#include "VSK_AttributeComponent.h"
#include "EngineUtils.h"
#include "DrawDebugHelpers.h"
#include "VSK_Character.h"
#include "VSK_PlayerState.h"
#include "VSK_SaveGame.h"
#include "Kismet\GameplayStatics.h"
#include "GameFramework/GameStateBase.h"
#include "VSK_GameplayInterface.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "VSK_MonsterData.h"
#include "ActionRoguelike\ActionRoguelike.h"
#include "VSK_ActionComponent.h"
#include "Engine/AssetManager.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("VSK.SpawnBots"),true,TEXT("Enable Spawning of bots via timer."),ECVF_Cheat);	  


AVSK_GameModeBase::AVSK_GameModeBase()
{
	SpawnTimerInterval = 5.0f;
	CreditsPerKill = 20;

	DesiredPowerupCount = 10;
	RequiredPowerupDistance = 2000;

	PlayerStateClass = AVSK_PlayerState::StaticClass();

	SlotName = "SaveGame01";
}

void AVSK_GameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	FString SelectedSaveSlot = UGameplayStatics::ParseOption(Options, "SaveGame");
	if (SelectedSaveSlot.Len() > 0)
	{
		SlotName = SelectedSaveSlot;
	}
	LoadSaveGame();
}
												
void AVSK_GameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &AVSK_GameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);

	//UEnvQueryInstanceBlueprintWrapper* PowerupSpawnQueryInstance = UEnvQueryManager::RunEQSQuery(this, PowerupSpawnQuery, this, EEnvQueryRunMode::AllMatching, nullptr);
	//if (ensure(PowerupSpawnQueryInstance))
	//{
	//	PowerupSpawnQueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AVSK_GameModeBase::OnPowerupSpawnQueryCompleted);
	//}
	if (ensure(PowerupClasses.Num() > 0))
	{
		// Run EQS to find potential power-up spawn locations
		UEnvQueryInstanceBlueprintWrapper* PowerupSpawnQueryInstance = UEnvQueryManager::RunEQSQuery(this, PowerupSpawnQuery, this, EEnvQueryRunMode::AllMatching, nullptr);
		if (ensure(PowerupSpawnQueryInstance))
		{
			PowerupSpawnQueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AVSK_GameModeBase::OnPowerupSpawnQueryCompleted);
		}
	}
}

void AVSK_GameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{

	AVSK_PlayerState* PS = NewPlayer->GetPlayerState<AVSK_PlayerState>();
	if (PS)
	{
		PS->LoadPlayerState(CurrentSaveGame);
	}
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}

void AVSK_GameModeBase::KillAll()
{
	for (TActorIterator<AVSK_AICharacter> It(GetWorld()); It; ++It)
	{
		int32 NrOfAliveBots = 0;
		AVSK_AICharacter* Bot = *It;
		UVSK_AttributeComponent* AttributeComp = UVSK_AttributeComponent::GetAttributeComponent(Bot);
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			AttributeComp->Kill(this);
		}
	}
}

void AVSK_GameModeBase::SpawnBotTimerElapsed()
{
	if(!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp,Warning,TEXT("Bot Spawning disable via cvar'CVarSpawnBots'."))
		return;
	}
	int32 NrOfAliveBots = 0;
	for (TActorIterator<AVSK_AICharacter> It(GetWorld()); It; ++It)
	{
		AVSK_AICharacter* Bot = *It;
		UVSK_AttributeComponent* AttributeComp = UVSK_AttributeComponent::GetAttributeComponent(Bot);
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			NrOfAliveBots++;
		}
	}
	UE_LOG(LogTemp, Log, TEXT("Found %i alive bots."), NrOfAliveBots);

	float MaxBotCount = 10.0f;
	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}
	if (NrOfAliveBots >= MaxBotCount)
	{
		UE_LOG(LogTemp, Log, TEXT("At Maximum bot capacity. Skipping bot spawn."));
		return;
	}
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AVSK_GameModeBase::OnSpawnBotQueryCompleted);	
	}

}

void AVSK_GameModeBase::OnSpawnBotQueryCompleted(UEnvQueryInstanceBlueprintWrapper* SpawnBotQueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed!"));
		return;
	}
	
	
	TArray<FVector>Locations = SpawnBotQueryInstance->GetResultsAsLocations();
	if (Locations.Num() > 0)
	{
		if (MonsterTable)
		{
			TArray<FMonsterInfoRow*> Rows;
			MonsterTable->GetAllRows("", Rows);

			int32 RandomIndex = FMath::RandRange(0, Rows.Num() - 1);
			FMonsterInfoRow* SelectedRow = Rows[RandomIndex];

			UAssetManager* Manager = UAssetManager::GetIfValid();
			if (Manager)
			{
				LogOnScreen(this, "Loading monster...", FColor::Green);
				TArray<FName> Bundles;
				FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &AVSK_GameModeBase::OnMonsterLoaded,SelectedRow->MonsterId, Locations[0]);
				Manager->LoadPrimaryAsset(SelectedRow->MonsterId, Bundles,Delegate);
			}

		}


		DrawDebugSphere(GetWorld(), Locations[0], 50.0f, 20, FColor::Blue, false, 60.0f);
	}
}

void AVSK_GameModeBase::OnMonsterLoaded(FPrimaryAssetId LoadedId, FVector SpawnLocation)
{
	LogOnScreen(this, "Finished Loading.", FColor::Green);
	UAssetManager* Manager = UAssetManager::GetIfValid();
	if (Manager)
	{
		UVSK_MonsterData* MonsterData = Cast<UVSK_MonsterData>(Manager->GetPrimaryAssetObject(LoadedId));
		if (MonsterData)
		{

			AActor* NewBot = GetWorld()->SpawnActor<AActor>(MonsterData->MonsterClass, SpawnLocation, FRotator::ZeroRotator);
			if (NewBot)
			{
				LogOnScreen(this, FString::Printf(TEXT("Spawned enemy: %s(%s)"), *GetNameSafe(NewBot), *GetNameSafe(MonsterData)));
				UVSK_ActionComponent* ActionComp = Cast<UVSK_ActionComponent>(NewBot->GetComponentByClass(UVSK_ActionComponent::StaticClass()));
				if (ActionComp)
				{
					for (TSubclassOf<UVSK_Action> ActionClass : MonsterData->Actions)
					{
						ActionComp->AddAction(NewBot, ActionClass);
					}
				}
			}
		}
	}


}


void AVSK_GameModeBase::OnPowerupSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* PowerupSpawnQueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn Powerup EQS Query Failed!"));
		return;
	}

	TArray<FVector> Locations = PowerupSpawnQueryInstance->GetResultsAsLocations();

	// Keep used locations to easily check distance between points
	TArray<FVector> UsedLocations;

	int32 SpawnCounter = 0;
	// Break out if we reached the desired count or if we have no more potential positions remaining
	while (SpawnCounter < DesiredPowerupCount && Locations.Num() > 0)
	{
		// Pick a random location from remaining points.
		int32 RandomLocationIndex = FMath::RandRange(0, Locations.Num() - 1);

		FVector PickedLocation = Locations[RandomLocationIndex];
		// Remove to avoid picking again
		Locations.RemoveAt(RandomLocationIndex);

		// Check minimum distance requirement
		bool bValidLocation = true;
		for (FVector OtherLocation : UsedLocations)
		{
			float DistanceTo = (PickedLocation - OtherLocation).Size();

			if (DistanceTo < RequiredPowerupDistance)
			{
				// Show skipped locations due to distance
				//DrawDebugSphere(GetWorld(), PickedLocation, 50.0f, 20, FColor::Red, false, 10.0f);

				// too close, skip to next attempt
				bValidLocation = false;
				break;
			}
		}

		// Failed the distance test
		if (!bValidLocation)
		{
			continue;
		}

		// Pick a random powerup-class
		int32 RandomClassIndex = FMath::RandRange(0, PowerupClasses.Num() - 1);
		TSubclassOf<AActor> RandomPowerupClass = PowerupClasses[RandomClassIndex];
		PickedLocation += FVector(0,0,1) * 20;

		GetWorld()->SpawnActor<AActor>(RandomPowerupClass, PickedLocation, FRotator::ZeroRotator);

		// Keep for distance checks
		UsedLocations.Add(PickedLocation);
		SpawnCounter++;
	}
}

void AVSK_GameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();

		RestartPlayer(Controller);
	}

}

void AVSK_GameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	AVSK_Character* Player = Cast<AVSK_Character>(VictimActor);
	if (Player)
	{
		FTimerHandle TimerHandle_RespawnDelay;

		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());

		float RespawnDelay = 2.0f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay,Delegate, RespawnDelay,false);	
	}
	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim: %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer))

	AVSK_Character* KillerPawn = Cast<AVSK_Character>(Killer);
	if (KillerPawn)
	{
		AVSK_PlayerState* PS = KillerPawn->GetPlayerState<AVSK_PlayerState>();
		if (PS)
		{
			PS->AddCredits(CreditsPerKill);
		}
	}
}


void AVSK_GameModeBase::WriteSaveGame()
{
	for (int32 i = 0; i < GameState->PlayerArray.Num();i++)
	{
		AVSK_PlayerState* PS = Cast<AVSK_PlayerState>(GameState->PlayerArray[i]);
		if(PS)
		{
			PS->SavePlayerState(CurrentSaveGame);
			break;//single player only at first
		}
	}

	CurrentSaveGame->SaveActors.Empty();

	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (!Actor->Implements<UVSK_GameplayInterface>())
		{
			continue;
		}
		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetName();
		ActorData.Transform = Actor->GetActorTransform();

		FMemoryWriter MemWriter(ActorData.ByteData);

		FObjectAndNameAsStringProxyArchive Ar(MemWriter,true);
		Ar.ArIsSaveGame = true;

		Actor->Serialize(Ar);

		CurrentSaveGame->SaveActors.Add(ActorData);
	}
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
}

void AVSK_GameModeBase::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CurrentSaveGame = Cast<UVSK_SaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Log, TEXT("Failed to load SaveGame Data."));
			return;
		}
		UE_LOG(LogTemp, Log, TEXT("Loaded SaveGame Data."));

		for (FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;
			if (!Actor->Implements<UVSK_GameplayInterface>())
			{
				continue;
			}
			for (FActorSaveData ActorData : CurrentSaveGame->SaveActors)
			{
				if (ActorData.ActorName == Actor->GetName())
				{
					Actor->SetActorTransform(ActorData.Transform);

					FMemoryReader MemReader(ActorData.ByteData);

					FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
					Ar.ArIsSaveGame = true;

					Actor->Serialize(Ar);

					IVSK_GameplayInterface::Execute_OnActorLoaded(Actor);

					break;
				}
			}
		}
	}
	else
	{
		CurrentSaveGame = Cast<UVSK_SaveGame>(UGameplayStatics::CreateSaveGameObject(UVSK_SaveGame::StaticClass()));
	}
	
}


