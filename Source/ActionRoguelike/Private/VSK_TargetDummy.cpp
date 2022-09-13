// Fill out your copyright notice in the Description page of Project Settings.


#include "VSK_TargetDummy.h"
#include "VSK_AttributeComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AVSK_TargetDummy::AVSK_TargetDummy()
{
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
    RootComponent = MeshComp;

    AttributeComp = CreateDefaultSubobject<UVSK_AttributeComponent>("AttributeComp");

    AttributeComp->OnHealthChanged.AddDynamic(this, &AVSK_TargetDummy::OnHealthChanged);

}

void AVSK_TargetDummy::OnHealthChanged(AActor* InstigatorActor, UVSK_AttributeComponent* OwningComp, float NewHealth, float Delta)
{
    if (Delta < 0.0f)
    {
        MeshComp->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
    }
}


