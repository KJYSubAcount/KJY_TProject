// Copyright Epic Games, Inc. All Rights Reserved.

#include "KJY_TProjectCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Net/UnrealNetwork.h"
//플러그인 weaponbase추가
#include "WeaponBase.h"
#include "WeaponInterface.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AKJY_TProjectCharacter

AKJY_TProjectCharacter::AKJY_TProjectCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AKJY_TProjectCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AKJY_TProjectCharacter::EquipWeapon(TSubclassOf<class AWeaponBase> WeaponClass)
{
	if (false == HasAuthority())
	{
		return;
	}

	m_EquipWeapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass);

	AWeaponBase* pWeapon = Cast<AWeaponBase>(m_EquipWeapon);
	if (pWeapon == nullptr)
	{
		return;
	}

	pWeapon->m_pOwnChar = this;

	WeaponSetOwner();

	m_EquipWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("WeaponSocket"));
}

void AKJY_TProjectCharacter::WeaponSetOwner()
{
	if (IsValid(GetController()))
	{
		m_EquipWeapon->SetOwner(GetController());
		return;
	}

	FTimerManager& tm = GetWorld()->GetTimerManager();
	tm.SetTimer(WeaponSetOwnerTimer, this, &AKJY_TProjectCharacter::WeaponSetOwner, 0.1f, false, 0.1f);
}

AActor* AKJY_TProjectCharacter::FindNearestWeapon()
{
	TArray<AActor*> actors;
	GetCapsuleComponent()->GetOverlappingActors(actors, AWeaponBase::StaticClass());

	double nearestDist = 9999999.0f;
	AActor* pNearestActor = nullptr;
	for (AActor* pTarget : actors)
	{
		if (m_EquipWeapon == pTarget)
			continue;

		double dist = FVector::Distance(GetActorLocation(), pTarget->GetActorLocation());
		if (dist >= nearestDist)
			continue;

		nearestDist = dist;
		pNearestActor = pTarget;
	}

	return pNearestActor;
}


//////////////////////////////////////////////////////////////////////////
// Input

void AKJY_TProjectCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AKJY_TProjectCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AKJY_TProjectCharacter::Look);

		// Firing
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AKJY_TProjectCharacter::Fire);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &AKJY_TProjectCharacter::StopFire);

		// PickUp
		EnhancedInputComponent->BindAction(PickUpAction, ETriggerEvent::Triggered, this, &AKJY_TProjectCharacter::PickUp);

		// Reload
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &AKJY_TProjectCharacter::Reload);

		// Drop
		EnhancedInputComponent->BindAction(DropAction, ETriggerEvent::Triggered, this, &AKJY_TProjectCharacter::Drop);

	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AKJY_TProjectCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AKJY_TProjectCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AKJY_TProjectCharacter::Fire(const FInputActionValue& Value)
{
	AWeaponBase* pWeapon = Cast<AWeaponBase>(m_EquipWeapon);
	// pweapon의 m_ammo가 0이면 return
	if (IsValid(pWeapon) && pWeapon->m_Ammo <= 0)
		return;
	ReqTrigger();
	UE_LOG(LogTemp, Warning, TEXT("Fire"));

}

void AKJY_TProjectCharacter::StopFire(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("StopFire"));
}

void AKJY_TProjectCharacter::PickUp(const FInputActionValue& Value)
{
	ReqPickUp();
}

void AKJY_TProjectCharacter::Reload(const FInputActionValue& Value)
{
	ReqReload();
}

void AKJY_TProjectCharacter::Drop(const FInputActionValue& Value)
{
	ReqDrop();
}

/// ============ implementation ===============///

void AKJY_TProjectCharacter::ReqPickUp_Implementation()
{
	AActor* pNearestActor = FindNearestWeapon();

	if (false == IsValid(pNearestActor))
		return;

	if (nullptr != m_EquipWeapon)
	{
		m_EquipWeapon->SetOwner(nullptr);
	}

	pNearestActor->SetOwner(GetController());

	ResPickUp(pNearestActor);
}

void AKJY_TProjectCharacter::ResPickUp_Implementation(AActor* PickUpActor)
{
	m_EquipWeapon = PickUpActor;

	if (nullptr != m_EquipWeapon)
	{
		AWeaponBase* pWeapon = Cast<AWeaponBase>(m_EquipWeapon);
		if (pWeapon == nullptr)
		{
			return;
		}

		pWeapon->SetOwner(this);
		IWeaponInterface* WeaponInterface = Cast<IWeaponInterface>(m_EquipWeapon);
		if (nullptr == WeaponInterface)
			return;

		if (WeaponInterface == nullptr)
		{
			return;
		}
		WeaponInterface->Execute_EventDrop(m_EquipWeapon, this);
	}

	IWeaponInterface* WeaponInterface = Cast<IWeaponInterface>(m_EquipWeapon);
	if (WeaponInterface == nullptr)
	{
		return;
	}
	WeaponInterface->Execute_EventPickUp(m_EquipWeapon, this);
}

void AKJY_TProjectCharacter::ResPressFClient_Implementation()
{
}

void AKJY_TProjectCharacter::ReqTrigger_Implementation()
{
	ResTrigger();
}

void AKJY_TProjectCharacter::ResTrigger_Implementation()
{
	IWeaponInterface* WeaponInterface = Cast<IWeaponInterface>(m_EquipWeapon);
	if (WeaponInterface == nullptr)
	{
		return;
	}

	WeaponInterface->Execute_EventTrigger(m_EquipWeapon);
	EventShoot_Implementation();
}

void AKJY_TProjectCharacter::ReqReload_Implementation()
{
	ResReload();
}

void AKJY_TProjectCharacter::ResReload_Implementation()
{
	IWeaponInterface* InterfaceObj = Cast<IWeaponInterface>(m_EquipWeapon);
	if (nullptr == InterfaceObj)
		return;

	InterfaceObj->Execute_EventReload(m_EquipWeapon);
}

void AKJY_TProjectCharacter::ReqDrop_Implementation()
{
	if (false == IsValid(m_EquipWeapon))
		return;

	m_EquipWeapon->SetOwner(nullptr);
	ResDrop();
}

void AKJY_TProjectCharacter::ResDrop_Implementation()
{
	IWeaponInterface* InterfaceObj = Cast<IWeaponInterface>(m_EquipWeapon);
	if (nullptr == InterfaceObj)
		return;

	InterfaceObj->Execute_EventDrop(m_EquipWeapon, this);
	m_EquipWeapon = nullptr;
}

// 아래는 이벤트(인터페이스) 임시방편

void AKJY_TProjectCharacter::EventTrigger_Implementation()
{
}

void AKJY_TProjectCharacter::EventShoot_Implementation()
{
	IWeaponInterface* InterfaceObj = Cast<IWeaponInterface>(this->m_EquipWeapon);
	if (nullptr == InterfaceObj)
		return;

	InterfaceObj->Execute_EventShoot(this->m_EquipWeapon);
}

void AKJY_TProjectCharacter::EventReload_Implementation()
{
}

void AKJY_TProjectCharacter::EventResetAmmo_Implementation()
{
}

void AKJY_TProjectCharacter::EventPickUp_Implementation(ACharacter* pOwnChar)
{
}

void AKJY_TProjectCharacter::EventDrop_Implementation(ACharacter* pOwnChar)
{
}

void AKJY_TProjectCharacter::EventGetItem_Implementation(EItemType itemType)
{
	switch (itemType)
	{
	case EItemType::IT_MAG:
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("EventGetItem Mag"));

		
	}
	break;

	case EItemType::IT_HEAL:
	{
		
	}
	break;

	default:
		break;
	}
}