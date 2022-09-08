// Fill out your copyright notice in the Description page of Project Settings.


#include "VSK_Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "VSK_InteractionComponent.h"
#include "DrawDebugHelpers.h"

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
	PlayerInputComponent->BindAction("UltimateAttack", IE_Pressed, this, &AVSK_Character::UltimateAttack);
	PlayerInputComponent->BindAction("TransAttack", IE_Pressed, this, &AVSK_Character::TransAttack);

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
//Ability
void AVSK_Character::PrimaryAttack()
{	
	PlayAnimMontage(CommonAttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &AVSK_Character::PrimaryAttack_TimeElapsed, 0.2f);

}
void AVSK_Character::UltimateAttack()
{
	PlayAnimMontage(UltimateAttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_UltimateAttack, this, &AVSK_Character::UltimateAttack_TimeElapsed, 0.25f);

} 
void AVSK_Character::TransAttack()
{
	PlayAnimMontage(CommonAttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_TransAttack, this, &AVSK_Character::TransAttack_TimeElapsed, 0.2f);

}
void AVSK_Character::PrimaryAttack_TimeElapsed()
{
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

	FVector Start =GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();
	FVector End = Start+ GetControlRotation().Vector()*10000;

	FVector Look = End - Start;
	FRotator FLook = Look.ToOrientationRotator();


	//RayCheckSet

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);

	/*FHitResult Hit;
	bool bBlockingHit=GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);*/

	TArray<FHitResult> Hits;

	float Radius = 30.0f;

	FCollisionShape Shape;
	Shape.SetSphere(Radius);

	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, Start, End, FQuat::Identity, ObjectQueryParams, Shape);

	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	for (FHitResult Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if (bBlockingHit)
		{
			FVector HStart = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();
			FVector HEnd = /*HitActor->GetActorLocation()*/Hit.Location;

			FVector HLook = HEnd - HandLocation;
			FRotator RHLook = HLook.ToOrientationRotator();

			FTransform SpawnTM = FTransform(RHLook, HandLocation);

			FActorSpawnParameters SpwanParams;
																								
			SpwanParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			SpwanParams.Instigator = this;

			GetWorld()->SpawnActor<AActor>(CommonProjectileClass, SpawnTM, SpwanParams);
			UE_LOG(LogTemp, Warning, TEXT("Hited"));
			break;
		}
		
	}
	if (!bBlockingHit)
	{
		FVector HStart = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();
		FVector HEnd = Start + GetControlRotation().Vector() * 3000;

		FVector HLook = HEnd - HandLocation;
		FRotator RHLook = HLook.ToOrientationRotator();

		FTransform SpawnTM = FTransform(RHLook, HandLocation);

		FActorSpawnParameters SpwanParams;

		SpwanParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpwanParams.Instigator = this;

		GetWorld()->SpawnActor<AActor>(CommonProjectileClass, SpawnTM, SpwanParams);
		UE_LOG(LogTemp, Warning, TEXT("Hits Nothing"));
	}



}
void AVSK_Character::UltimateAttack_TimeElapsed()
{
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_02");

	FVector Start = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();
	FVector End = Start + GetControlRotation().Vector() * 10000;

	FVector Look = End - Start;
	FRotator FLook = Look.ToOrientationRotator();


	//RayCheckSet

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);

	/*FHitResult Hit;
	bool bBlockingHit=GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);*/

	TArray<FHitResult> Hits;

	float Radius = 30.0f;

	FCollisionShape Shape;
	Shape.SetSphere(Radius);

	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, Start, End, FQuat::Identity, ObjectQueryParams, Shape);

	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	for (FHitResult Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if (bBlockingHit)
		{
			FVector HStart = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();
			FVector HEnd = /*HitActor->GetActorLocation()*/Hit.Location;

			FVector HLook = HEnd - HandLocation;
			FRotator RHLook = HLook.ToOrientationRotator();

			FVector	Adjusted_HandLocation = HandLocation + GetActorUpVector().Normalize() * 70;

			FTransform SpawnTM = FTransform(RHLook, Adjusted_HandLocation);

			FActorSpawnParameters SpwanParams;

			SpwanParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			SpwanParams.Instigator = this;

			GetWorld()->SpawnActor<AActor>(UltimateProjectileClass, SpawnTM, SpwanParams);
			UE_LOG(LogTemp, Warning, TEXT("Hited"));
			break;
		}

	}
	if (!bBlockingHit)
	{
		FVector HStart = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();
		FVector HEnd = Start + GetControlRotation().Vector() * 3000;

		FVector HLook = HEnd - HandLocation;
		FRotator RHLook = HLook.ToOrientationRotator();

		FVector	Adjusted_HandLocation = HandLocation + GetActorUpVector().Normalize()*70;

		FTransform SpawnTM = FTransform(RHLook, Adjusted_HandLocation);

		FActorSpawnParameters SpwanParams;

		SpwanParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpwanParams.Instigator = this;

		GetWorld()->SpawnActor<AActor>(UltimateProjectileClass, SpawnTM, SpwanParams);
		UE_LOG(LogTemp, Warning, TEXT("Hits Nothing"));
	}



}
void AVSK_Character::TransAttack_TimeElapsed()
{
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

	FVector Start = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();
	FVector End = Start + GetControlRotation().Vector() * 10000;

	FVector Look = End - Start;
	FRotator FLook = Look.ToOrientationRotator();


	//RayCheckSet

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);

	/*FHitResult Hit;
	bool bBlockingHit=GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);*/

	TArray<FHitResult> Hits;

	float Radius = 30.0f;

	FCollisionShape Shape;
	Shape.SetSphere(Radius);

	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, Start, End, FQuat::Identity, ObjectQueryParams, Shape);

	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	for (FHitResult Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if (bBlockingHit)
		{
			FVector HStart = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();
			FVector HEnd = /*HitActor->GetActorLocation()*/Hit.Location;

			FVector HLook = HEnd - HandLocation;
			FRotator RHLook = HLook.ToOrientationRotator();

			FTransform SpawnTM = FTransform(RHLook, HandLocation);

			FActorSpawnParameters SpwanParams;

			SpwanParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			SpwanParams.Instigator = this;

			GetWorld()->SpawnActor<AActor>(TransProjectileClass, SpawnTM, SpwanParams);
			UE_LOG(LogTemp, Warning, TEXT("Hited"));
			break;
		}

	}
	if (!bBlockingHit)
	{
		FVector HStart = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();
		FVector HEnd = Start + GetControlRotation().Vector() * 3000;

		FVector HLook = HEnd - HandLocation;
		FRotator RHLook = HLook.ToOrientationRotator();

		FTransform SpawnTM = FTransform(RHLook, HandLocation);

		FActorSpawnParameters SpwanParams;

		SpwanParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpwanParams.Instigator = this;

		GetWorld()->SpawnActor<AActor>(TransProjectileClass, SpawnTM, SpwanParams);
		UE_LOG(LogTemp, Warning, TEXT("Hits Nothing"));
	}



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

