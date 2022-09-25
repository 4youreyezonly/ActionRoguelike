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


AVSK_AICharacter::AVSK_AICharacter()
{

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
	AttributeComp = CreateDefaultSubobject<UVSK_AttributeComponent>("AttributeComp");
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	//WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("WidgetComponent");
	//WidgetComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

}
void AVSK_AICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AVSK_AICharacter::OnPawnSeen);
	AttributeComp->OnHealthChanged.AddDynamic(this, &AVSK_AICharacter::OnHealthChanged);
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
				ActiveHealthBar->AddToViewport();
			}
		}
		


		//UUserWidget* InfoWidget = WidgetComponent->GetUserWidgetObject();
		//if (InfoWidget)
		//{
		//	auto ProgressBar = Cast<UProgressBar>(InfoWidget->GetWidgetFromName(TEXT("Minion_Life")));
		//	if (ProgressBar)
		//	{
		//		ProgressBar->SetPercent((AttributeComp->GetHealth()) / (AttributeComp->GetHealthMax()));
		//	}
		//	auto HealthText = Cast<UTextBlock>(InfoWidget->GetWidgetFromName(TEXT("HealthText")));
		//	if (HealthText)
		//	{
		//		FString Health = FString::SanitizeFloat(AttributeComp->GetHealth());
		//		HealthText->SetText(FText::FromString(Health));
		//	}
		//}

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

void AVSK_AICharacter::SetTargetActor(AActor* NewTarget)
{
 	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);
	}
}

void AVSK_AICharacter::OnPawnSeen(APawn* Pawn)
{
	SetTargetActor(Pawn);
  	DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true);
}

void  AVSK_AICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
		
}