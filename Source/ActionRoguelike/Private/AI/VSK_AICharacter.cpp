// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/VSK_AICharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "VSK_AttributeComponent.h"
#include "BrainComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "VSK_WorldUserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "VSK_ActionComponent.h"


AVSK_AICharacter::AVSK_AICharacter()
{

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
	AttributeComp = CreateDefaultSubobject<UVSK_AttributeComponent>("AttributeComp");
	ActionComp = CreateDefaultSubobject<UVSK_ActionComponent>("ActionComp");
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	//WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("WidgetComponent");
	//WidgetComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	GetMesh()->SetGenerateOverlapEvents(true);

}
void AVSK_AICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AVSK_AICharacter::OnPawnSeen);
	AttributeComp->OnHealthChanged.AddDynamic(this, &AVSK_AICharacter::OnHealthChanged);
	AttributeComp->OnRageChanged.AddDynamic(this, &AVSK_AICharacter::OnRageChanged);
}

void AVSK_AICharacter::OnHealthChanged(AActor* InstigatorActor, UVSK_AttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (abs(Delta))
	{
		if (InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor);
		}
		if (ActiveHealthBar == nullptr)
		{
			 ActiveHealthBar = CreateWidget<UVSK_WorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if (ActiveHealthBar)
			{
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport(10);
			}
		}
		if (ActiveSpottedWidget == nullptr)
		{
			ActiveSpottedWidget = CreateWidget<UVSK_WorldUserWidget>(GetWorld(), SpottedWidgetClass);
			if (ActiveSpottedWidget)
			{
				ActiveSpottedWidget->AttachedActor = this;
				ActiveSpottedWidget->AddToViewport(10);
			}
		}


		if (NewHealth <= 0.0f)
		{
			AAIController* AIC = Cast<AAIController>(GetController());
			if (AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Killed");
			}
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();
			SetLifeSpan(10.0f);
		}
	}
}
void AVSK_AICharacter::OnRageChanged(AActor* InstigatorActor, UVSK_AttributeComponent* OwningComp, float NewRage, float Delta)
{
	if (ActiveRageBar == nullptr)
	{
		ActiveRageBar = CreateWidget<UVSK_WorldUserWidget>(GetWorld(), RageBarWidgetClass);
		if (ActiveRageBar)
		{
			ActiveRageBar->AttachedActor = this;
			ActiveRageBar->AddToViewport();
		}
	}
}

void AVSK_AICharacter::SetTargetActor(AActor* NewTarget)
{
 	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);
	}
}

AActor* AVSK_AICharacter::GetTargetActor()const
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		return Cast<AActor>(AIC->GetBlackboardComponent()->GetValueAsObject("TargetActor"));
	}
	return nullptr;
}

void AVSK_AICharacter::OnPawnSeen(APawn* Pawn)
{
	if (GetTargetActor() != Pawn)
	{
		SetTargetActor(Pawn);
		DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true);
	
		MulticastPawnSeen();
	
	}
}

void  AVSK_AICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
		
}

void AVSK_AICharacter::MulticastPawnSeen_Implementation()
{
	if (ActiveSpottedWidget == nullptr)
	{
		ActiveSpottedWidget = CreateWidget<UVSK_WorldUserWidget>(GetWorld(), SpottedWidgetClass);
		if (ActiveSpottedWidget)
		{
			ActiveSpottedWidget->AttachedActor = this;
			ActiveSpottedWidget->AddToViewport(10);
		}
	}
}