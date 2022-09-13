// Fill out your copyright notice in the Description page of Project Settings.


#include "VSK_MagicProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "VSK_AttributeComponent.h"
#include "DrawDebugHelpers.h"
#include "VSK_Character.h"


// Sets default values
AVSK_MagicProjectile::AVSK_MagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	MovementComp->InitialSpeed = 2000.0f;
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AVSK_MagicProjectile::OnActorOverlap);
	SphereComp->IgnoreActorWhenMoving(GetInstigator(),true);

}

void AVSK_MagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor&&OtherActor != GetInstigator())
	{
		UVSK_AttributeComponent* AttributeComp = Cast<UVSK_AttributeComponent>(OtherActor->GetComponentByClass(UVSK_AttributeComponent::StaticClass()));
		if (AttributeComp)
		{
			AttributeComp->ApplyHealthChange(-20.0f);
			AVSK_Character* HitedCharacter = Cast<AVSK_Character>(OtherActor);
			if (HitedCharacter)
			{
				HitedCharacter->GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
			}
			Destroy();
		}
	}
}

// Called when the game starts or when spawned
void AVSK_MagicProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AVSK_MagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

