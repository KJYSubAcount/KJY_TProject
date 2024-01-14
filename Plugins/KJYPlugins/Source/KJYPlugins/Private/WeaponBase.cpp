// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h" // DOREPLIFETIME 사용을 위한 헤더파일 추가
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AWeaponBase::AWeaponBase():m_Ammo(30)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("Weapon");
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
	//pOwnChar->PlayAnimMontage();
}

void AWeaponBase::EventShoot_Implementation()
{
	if (false == IsCanShoot())
	{
		return;
	}

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WeaponFireEffect,
		WeaponMesh->GetSocketLocation("muzzle"),
		WeaponMesh->GetSocketRotation("muzzle"),
		FVector(0.1f, 0.1f, 0.1f));

	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), WeaponSoundBase,
		WeaponMesh->GetSocketLocation("muzzle"));

	APlayerController* pPlayer0 = GetWorld()->GetFirstPlayerController();
	if (pPlayer0 != pOwnChar->GetController())
	{
		return;
	}

	FVector vStart = WeaponMesh->GetSocketLocation("Muzzle");
	FVector vEnd = vStart + WeaponMesh->GetForwardVector() * 10000.0f;

	ReqShoot(vStart, vEnd);
}

void AWeaponBase::EventReload_Implementation()
{
}

void AWeaponBase::EventPickUp_Implementation(ACharacter* PlayerOwnChar)
{
}

void AWeaponBase::EventResetAmmo_Implementation()
{
}

void AWeaponBase::EventDrop_Implementation(ACharacter* PlayerOwnChar)
{
}

void AWeaponBase::ReqShoot_Implementation(FVector vStart, FVector vEnd)
{

}

float AWeaponBase::GetFireStartLength()
{
	return 0.0f;
}

bool AWeaponBase::IsCanShoot()
{
	return false;
}

bool AWeaponBase::UseAmmo()
{
	return false;
}

void AWeaponBase::SetAmmo(int Ammo)
{
}

void AWeaponBase::OnUpdateAmmoToHud(int Ammo)
{
}

void AWeaponBase::OnRep_Ammo()
{
}

