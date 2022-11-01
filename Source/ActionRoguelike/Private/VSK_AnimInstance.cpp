// Fill out your copyright notice in the Description page of Project Settings.


#include "VSK_AnimInstance.h"
#include "GameplayTagContainer.h"
#include "VSK_ACtionComponent.h"

void UVSK_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	AActor* OwningActor = GetOwningActor();
	if (OwningActor)
	{
		ActionComp = Cast<UVSK_ActionComponent>(OwningActor->GetComponentByClass(UVSK_ActionComponent::StaticClass()));
	}
}

void UVSK_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	static FGameplayTag StunnedTag = FGameplayTag::RequestGameplayTag("Status.Stunned");
	if (ActionComp)
	{
		bIsStunned = ActionComp->ActiveGameplayTags.HasTag(StunnedTag);
	}
}
