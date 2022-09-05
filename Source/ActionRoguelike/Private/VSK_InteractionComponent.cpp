// Fill out your copyright notice in the Description page of Project Settings.


#include "VSK_InteractionComponent.h"
#include "VSK_GameplayInterface.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UVSK_InteractionComponent::UVSK_InteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UVSK_InteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UVSK_InteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UVSK_InteractionComponent::PrimaryInteract()
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	AActor* MyOwner = GetOwner();


	FVector EyeLocation;
	FRotator EyeRotation;

	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() * 300);

	/*FHitResult Hit;
	bool bBlockingHit=GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);*/

	TArray<FHitResult> Hits;

	float Radius = 30.0f;

	FCollisionShape Shape;
	Shape.SetSphere(Radius);

	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits,EyeLocation,End,FQuat::Identity,ObjectQueryParams,Shape);

   	FColor LineColor = bBlockingHit ? FColor::Red : FColor::Green;

	for (FHitResult Hit : Hits)
	{
		 AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			if (HitActor->Implements<UVSK_GameplayInterface>())
			{
				APawn* MyPawn = Cast<APawn>(MyOwner);
				IVSK_GameplayInterface::Execute_Interact(HitActor, MyPawn);
			}
		}
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, LineColor, false, 2.0f);
		break;
	}
	

	DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0, 2.0f);

};

