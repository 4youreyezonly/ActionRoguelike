// Fill out your copyright notice in the Description page of Project Settings.


#include "VSK_Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "VSK_InteractionComponent.h"
#include "DrawDebugHelpers.h"
#include "VSK_AttributeComponent.h"	
#include "VSK_ActionComponent.h"


// Sets default values
AVSK_Character::AVSK_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	InterActionComp = CreateDefaultSubobject<UVSK_InteractionComponent>("InteractionComp");

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	AttributeComp = CreateDefaultSubobject<UVSK_AttributeComponent>("AttributeComp");	

	ActionComp = CreateDefaultSubobject<UVSK_ActionComponent>("ActionComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	TimeToHitParamName = "TimeToHit";

}

void AVSK_Character::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComp->OnHealthChanged.AddDynamic(this, &AVSK_Character::OnHealthChanged);
}



// Called to bind functionality to input
void AVSK_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AVSK_Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AVSK_Character::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed,this, &AVSK_Character::PrimaryAttack);
	PlayerInputComponent->BindAction("Blackhole", IE_Pressed, this, &AVSK_Character::UltimateAttack);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AVSK_Character::TransAttack);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AVSK_Character::JumpStart);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AVSK_Character::JumpEnd);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Released, this, &AVSK_Character::PrimaryInteract);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AVSK_Character::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AVSK_Character::SprintStop);
}

void AVSK_Character::HealSelf(float Amount)
{
	AttributeComp->ApplyHealthChange(this, Amount/*=100*/);
}

void AVSK_Character::MoveForward(float value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	AddMovementInput(ControlRot.Vector(), value);
}

void AVSK_Character::MoveRight(float value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	FVector RIghtVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
																 
	AddMovementInput(RIghtVector, value);
}
//Ability
void AVSK_Character::PrimaryAttack()
{	
	ActionComp->StartActionByName(this, "PrimaryAttack");

}
void AVSK_Character::UltimateAttack()
{
	ActionComp->StartActionByName(this, "Blackhole");

} 
void AVSK_Character::TransAttack()
{
	ActionComp->StartActionByName(this, "Dash");
}
	//FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

	//FVector Start =GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();
	//FVector End = Start+ GetControlRotation().Vector()*10000;

	//FVector Look = End - Start;
	//FRotator FLook = Look.ToOrientationRotator();


	////RayCheckSet

	//FCollisionObjectQueryParams ObjectQueryParams;
	//ObjectQueryParams.AddObjectTypesToQuery(ECC_AIPawn);
	//ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	//

	///*FHitResult Hit;
	//bool bBlockingHit=GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);*/

	//TArray<FHitResult> Hits;

	//float Radius = 30.0f;

	//FCollisionShape Shape;
	//Shape.SetSphere(Radius);

	//bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, Start, End, FQuat::Identity, ObjectQueryParams, Shape);

	//FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	//for (FHitResult Hit : Hits)
	//{
	//	AActor* HitActor = Hit.GetActor();
	//	if (bBlockingHit)
	//	{
	//		FVector HStart = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();
	//		FVector HEnd = /*HitActor->GetActorLocation()*/Hit.Location;

	//		FVector HLook = HEnd - HandLocation;
	//		FRotator RHLook = HLook.ToOrientationRotator();

	//		FTransform SpawnTM = FTransform(RHLook, HandLocation);

	//		FActorSpawnParameters SpwanParams;
	//																							
	//		SpwanParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//		SpwanParams.Instigator = this;

	//		GetWorld()->SpawnActor<AActor>(CommonProjectileClass, SpawnTM, SpwanParams);
	//		UE_LOG(LogTemp, Warning, TEXT("Hited"));
	//		break;
	//	}
	//	
	//}
	//if (!bBlockingHit)
	//{
	//	FVector HStart = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();
	//	FVector HEnd = Start + GetControlRotation().Vector() * 3000;

	//	FVector HLook = HEnd - HandLocation;
	//	FRotator RHLook = HLook.ToOrientationRotator();

	//	FTransform SpawnTM = FTransform(RHLook, HandLocation);

	//	FActorSpawnParameters SpwanParams;

	//	SpwanParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//	SpwanParams.Instigator = this;

	//	GetWorld()->SpawnActor<AActor>(CommonProjectileClass, SpawnTM, SpwanParams);
	//	UE_LOG(LogTemp, Warning, TEXT("Hits Nothing"));
	//}

void AVSK_Character::JumpStart()
{
	//如果是真的话，角色跳跃
	bPressedJump = true;
}
void AVSK_Character::JumpEnd()
{
	//如果是假的话，结束跳跃
	bPressedJump = false;
}
void AVSK_Character::PrimaryInteract()
{
	if (InterActionComp)
	{
		InterActionComp->PrimaryInteract();
	}
}

void AVSK_Character::OnHealthChanged(AActor* InstigatorActor, UVSK_AttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);

		float RageDelta = FMath::Abs(Delta);
		AttributeComp->ApplyRageChange(InstigatorActor, RageDelta);
	}
	if (NewHealth <= 0.0f && Delta < 0.0f)
	{
		APlayerController* PC=Cast<APlayerController>(GetController());
		DisableInput(PC);

		SetLifeSpan(5.0f);
	}
}

FVector AVSK_Character::GetPawnViewLocation()const
{
	return CameraComp->GetComponentLocation();
}

void AVSK_Character::SprintStart()
{
	ActionComp->StartActionByName(this, "Sprint");
}
void AVSK_Character::SprintStop()
{
	ActionComp->StopActionByName(this, "Sprint");
}





