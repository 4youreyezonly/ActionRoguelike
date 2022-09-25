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

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("VSK.SpawnBots"),true,TEXT("Enable Spawning of bots via timer."),ECVF_Cheat);	  


AVSK_GameModeBase::AVSK_GameModeBase()
{
	SpawnTimerInterval = 2.0f;
}
												
void AVSK_GameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &AVSK_GameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
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
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AVSK_GameModeBase::OnQueryCompleted);	
	}

}

void AVSK_GameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed!"));
		return;
	}
	
	
	TArray<FVector>Locations = QueryInstance->GetResultsAsLocations();
	if (Locations.Num() > 0)
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);

		DrawDebugSphere(GetWorld(), Locations[0], 50.0f, 20, FColor::Blue, false, 60.0f);
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
	UE_LOG(LogTemp,Log,TEXT("OnActorKilled: Victim: %s, Killer: %s"),*GetNameSafe(VictimActor),*GetNameSafe(Killer))
}

