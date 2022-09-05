// Fill out your copyright notice in the Description page of Project Settings.


#include "VSK_Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "VSK_InteractionComponent.h"

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

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

}

// Called when the game starts or when spawned
void AVSK_Character::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void AVSK_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AVSK_Character::JumpStart);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AVSK_Character::JumpEnd);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Released, this, &AVSK_Character::PrimaryInteract);
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
void AVSK_Character::PrimaryAttack()
{	
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &AVSK_Character::PrimaryAttack_TimeElapsed, 0.2f);

}
void AVSK_Character::PrimaryAttack_TimeElapsed()
{
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

	FTransform SpawnTM = FTransform(GetControlRotation(), HandLocation);

	FActorSpawnParameters SpwanParams;
	SpwanParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpwanParams);
}
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

