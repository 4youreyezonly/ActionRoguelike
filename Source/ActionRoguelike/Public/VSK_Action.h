// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "VSK_Action.generated.h"

class UWord;
class UVSK_ActionComponent;

USTRUCT()
struct FActionRepData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	bool bIsRunning;

	UPROPERTY()
	AActor* Instigator;
};

/**
 * 
 */
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API UVSK_Action : public UObject
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
		TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(Replicated)
		UVSK_ActionComponent* ActionComp;

	UFUNCTION(BlueprintCallable, Category = "Action")
		UVSK_ActionComponent* GetOwningComponent()const;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
		FGameplayTagContainer GrantsTags;
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
		FGameplayTagContainer BlockedTags;

	UPROPERTY(ReplicatedUsing = "OnRep_RepData")
		FActionRepData RepData;
	//bool bIsRunning;
	
	UPROPERTY(Replicated)
	float TimeStarted;

	UFUNCTION()
	void OnRep_RepData();


public:

	void Initialize(UVSK_ActionComponent* NewActionComp);

	UPROPERTY(EditDefaultsOnly, Category = "Action")
		bool bAutoStart;

	UFUNCTION(BlueprintCallable, Category = "Action")
		bool IsRunning()const;

	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
		bool CanStart(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
		void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent, Category = "Action")
		void StopAction(AActor* Instigator);

	UPROPERTY(EditDefaultsOnly, Category = "Action")
		FName ActionName;

	UWorld* GetWorld()const override;

	virtual bool IsSupportedForNetworking()const override
	{
		return true;
	}
	
};
