// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VSK_AttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeChanged,AActor*,InstigatorActor,UVSK_AttributeComponent*,OwningComp, float, NewValue, float, ActualDelta);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API UVSK_AttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFUNCTION(BlueprintCallable)
	bool Kill(AActor* InstigatorActor);

	UVSK_AttributeComponent();

	UFUNCTION(BlueprintCallable, Category = "Attributes")
		static UVSK_AttributeComponent* GetAttributeComponent(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category = "Attributes",meta = (DisplayName="isAlive"))
		static bool isActorAlive(AActor* Actor);

protected:
 	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Replicated, Category = "Attributes")
		float Health;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly,Replicated, Category = "Attributes")
		float HealthMax;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Replicated, Category = "Attributes")
		float Rage;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Replicated, Category = "Attributes")
		float RageMax;

	UFUNCTION(NetMulticast,Reliable)
		void MulticastHealthChanged(AActor* InstigatorActor, float NewHealth,float Delta);

	UFUNCTION(NetMulticast, Reliable)
		void MulticastRageChanged(AActor* InstigatorActor, float NewHealth, float Delta);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:												
	UPROPERTY(BlueprintAssignable,Category = "Attributes")
		FOnAttributeChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable,Category = "Attributes")
		FOnAttributeChanged OnRageChanged;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
		bool ApplyHealthChange(AActor* InstigatorActor,float Delta);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
		bool ApplyRageChange(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable,Category = "Attributes")
		bool IsAlive()const;
	UFUNCTION(Category = "Attributes")
		bool IsFullHealthAlive()const;
	UFUNCTION(BlueprintCallable, Category = "Attributes")
		float GetHealthMax()const;
	UFUNCTION(BlueprintCallable,Category = "Attributes")
		float GetHealth()const;
	UFUNCTION(BlueprintCallable, Category = "Attributes")
		void HealToMax();
};
