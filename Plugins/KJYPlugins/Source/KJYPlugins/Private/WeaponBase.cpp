// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h" // DOREPLIFETIME 사용을 위한 헤더파일 추가
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AWeaponBase::AWeaponBase() :m_Ammo(30)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("Weapon");
	WeaponMesh->SetCollisionProfileName("Weapon");
	WeaponMesh->SetSimulatePhysics(true);
	SetRootComponent(WeaponMesh);

	bReplicates = true;
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeaponBase, m_Ammo);
}

void AWeaponBase::EventTrigger_Implementation()
{
	//pOwnChar의 애니메이션 작동
	m_pOwnChar->PlayAnimMontage(ShootMontage);
	EventShoot_Implementation();
}

void AWeaponBase::EventShoot_Implementation()
{
	if (false == IsCanShoot())
	{
		return;
	}

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WeaponFireEffect,
		WeaponMesh->GetSocketLocation("Muzzle"),
		WeaponMesh->GetSocketRotation("Muzzle"),
		FVector(0.1f, 0.1f, 0.1f));

	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), WeaponSoundBase,
		WeaponMesh->GetSocketLocation("Muzzle"));

	APlayerController* pPlayer0 = GetWorld()->GetFirstPlayerController();
	if (pPlayer0 != m_pOwnChar->GetController())
	{
		return;
	}
	

	FVector vStart = WeaponMesh->GetSocketLocation("Muzzle");
	FVector vEnd = vStart + WeaponMesh->GetForwardVector() * 10000.0f;

	///Weapontype이 berreta면 연사기능이 없음
	if (KindOfWeapon == EWeaponType::Berreta)
	{
		ReqShoot(vStart, vEnd);
		return;
	}
	else
	{
		ReqShoot(vStart, vEnd);
		GetWorldTimerManager().SetTimer(TimerHandle_ShotDelay, this, &AWeaponBase::EventShoot_Implementation, ShotDelay, false);
	}
	m_pOwnChar->PlayAnimMontage(ShootMontage);

}

void AWeaponBase::EventReload_Implementation()
{
	m_pOwnChar->PlayAnimMontage(ReloadMontage);
}

void AWeaponBase::EventPickUp_Implementation(ACharacter* pOwnChar)
{
	m_pOwnChar = pOwnChar;
	WeaponMesh->SetSimulatePhysics(false);
	WeaponMesh->SetCollisionProfileName("NoCollision");
	WeaponMesh->AttachToComponent(pOwnChar->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "WeaponSocket");

	///OnUpdateAmmoToHud(m_Ammo);
}

void AWeaponBase::EventResetAmmo_Implementation()
{
}

void AWeaponBase::EventDrop_Implementation(ACharacter* PlayerOwnChar)
{
	OnUpdateAmmoToHud(0);
	WeaponMesh->SetSimulatePhysics(true);
	WeaponMesh->SetCollisionProfileName("Weapon");
	WeaponMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	// 무기 앞으로 날려보내기
	WeaponMesh->AddImpulse(PlayerOwnChar->GetActorForwardVector() * 300.0f);

	m_pOwnChar = nullptr;
}

void AWeaponBase::ReqShoot_Implementation(FVector vStart, FVector vEnd)
{
	if (false == UseAmmo())
		return;

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("ReqShoot_Implementation"));

	FHitResult result;
	FCollisionObjectQueryParams collisionObjParams;
	collisionObjParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
	collisionObjParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	collisionObjParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
	collisionObjParams.AddObjectTypesToQuery(ECollisionChannel::ECC_PhysicsBody);
	collisionObjParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Vehicle);
	collisionObjParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Destructible);

	FCollisionQueryParams collisionParams;
	collisionParams.AddIgnoredActor(m_pOwnChar);

	bool isHit = GetWorld()->LineTraceSingleByObjectType(result, vStart, vEnd, collisionObjParams, collisionParams);
	DrawDebugLine(GetWorld(), vStart, vEnd, FColor::Red, false, 5.0f);

	if (false == isHit)
		return;

	ACharacter* pHitChar = Cast<ACharacter>(result.GetActor());
	if (false == IsValid(pHitChar))
		return;

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("HitChar = %s"), *pHitChar->GetName()));

	UGameplayStatics::ApplyDamage(pHitChar, WeaponDamage, m_pOwnChar->GetController(), this, UDamageType::StaticClass());
}

float AWeaponBase::GetFireStartLength()
{
	if (false == IsValid(m_pOwnChar))
		return 0.0f;

	USpringArmComponent* pArm = Cast<USpringArmComponent>(m_pOwnChar->GetComponentByClass(USpringArmComponent::StaticClass()));
	if (false == IsValid(pArm))
		return 0.0f;

	return pArm->TargetArmLength + 100;
}

bool AWeaponBase::IsCanShoot()
{
	if (m_Ammo <= 0)
		return false;

	return true;
}

bool AWeaponBase::UseAmmo()
{
	if (false == IsCanShoot())
		return false;

	m_Ammo = m_Ammo - 1;
	m_Ammo = FMath::Clamp(m_Ammo, 0, 30);

	OnRep_Ammo();

	return true;
}

void AWeaponBase::SetAmmo(int Ammo)
{
	m_Ammo = Ammo;

	OnRep_Ammo();
}

void AWeaponBase::OnUpdateAmmoToHud(int Ammo)
{
}

void AWeaponBase::OnRep_Ammo()
{
	OnUpdateAmmoToHud(m_Ammo);
}