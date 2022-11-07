// Fill out your copyright notice in the Description page of Project Settings.


#include "VSK_MagicProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "VSK_AttributeComponent.h"
#include "DrawDebugHelpers.h"
#include "VSK_Character.h"
#include "Kismet/GameplayStatics.h"
#include "AI/VSK_AICharacter.h"
#include "VSK_GameplayFunctionLibrary.h"
#include "VSK_ActionComponent.h"
#include "VSK_ActionEffect.h"


// Sets default values
AVSK_MagicProjectile::AVSK_MagicProjectile()
{
	SphereComp->SetSphereRadius(20.0f);
	InitialLifeSpan = 10.0f;
	DamageAmount = 20.0f;
}

void AVSK_MagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// More consistent to bind here compared to Constructor which may fail to bind if Blueprint was created before adding this binding (or when using hotreload)
	// PostInitializeComponent is the preferred way of binding any events.
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AVSK_MagicProjectile::OnActorOverlap);
}

void AVSK_MagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor&&OtherActor != GetInstigator())
	{
		//UVSK_ActionComponent* ActionComp = Cast<UVSK_ActionComponent>(OtherActor->GetComponentByClass(UVSK_ActionComponent::StaticClass()));
		//if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
		//{
		//	MovementComp->Velocity = -MovementComp->Velocity;

		//	SetInstigator(Cast<APawn>(OtherActor));
		//	return;
		//}

		//UE_LOG(LogTemp, Log, TEXT("Overlap_Instigator: %s."), GetInstigator());
		//UE_LOG(LogTemp, Log, TEXT("Overlap_OtherActor: %s."), OtherActor);
		//UVSK_AttributeComponent* AttributeComp = Cast<UVSK_AttributeComponent>(OtherActor->GetComponentByClass(UVSK_AttributeComponent::StaticClass()));
		//if (UVSK_GameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, DamageAmount, SweepResult)&& UVSK_GameplayFunctionLibrary::AddRage(GetInstigator(), OtherActor, RageAmount))
		//{
		//	/*AttributeComp->ApplyHealthChange(GetInstigator(),-DamageAmount);*/

		//	AVSK_Character* HitedCharacter = Cast<AVSK_Character>(OtherActor);
		//	AVSK_AICharacter* HitedAICharacter = Cast<AVSK_AICharacter>(OtherActor);
		//	if (HitedCharacter)
		//	{
		//		HitedCharacter->GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
		//		if (ActionComp)
		//		{
		//			ActionComp->AddAction(GetInstigator(),BurningActionClass);
		//		}
		//		Explode()

		//	}
		//	if (HitedAICharacter && HasAuthority())
		//	{
		//		HitedAICharacter->GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
		//		UGameplayStatics::SpawnEmitterAtLocation(this, ExplodeFX, GetActorLocation(), GetActorRotation());
		//		ActionComp->AddAction(GetInstigator(), BurningActionClass);

		//	}
		//	UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
		//	Destroy();
		//}
		UVSK_ActionComponent* ActionComp = Cast<UVSK_ActionComponent>(OtherActor->GetComponentByClass(UVSK_ActionComponent::StaticClass()));
		if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
		{
			MovementComp->Velocity = -MovementComp->Velocity;

			SetInstigator(Cast<APawn>(OtherActor));
			return;
		}

		// Apply Damage & Impulse
		if (UVSK_GameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, DamageAmount, SweepResult))
		{
			// We only explode if the target can be damaged, it ignores anything it Overlaps that it cannot Damage (it requires an AttributeComponent on the target)
			Explode();

			if (ActionComp && BurningActionClass && HasAuthority())
			{
				ActionComp->AddAction(GetInstigator(), BurningActionClass);
			}
		}
	}
}


