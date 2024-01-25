// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponInterface.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"


UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	AK47 UMETA(DisplayName = "AK47"),
	M4A1 UMETA(DisplayName = "M4A1"),
	Thompson UMETA(DisplayName = "THOMPSON"),
	Berreta UMETA(DisplayerName = "BERETTA"),
	Shotgun UMETA(DisplayName = "Shotgun"),
	End UMETA(Hidden)
};

class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class KJYPLUGINS_API AWeaponBase : public AActor, public IWeaponInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventTrigger();

	virtual void EventTrigger_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventShoot();

	virtual void EventShoot_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventReload();

	virtual void EventReload_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventPickUp(ACharacter* pOwnChar);

	virtual void EventPickUp_Implementation(ACharacter* pOwnChar) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventResetAmmo();

	virtual void EventResetAmmo_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventDrop(ACharacter* PlayerOwnChar);

	virtual void EventDrop_Implementation(ACharacter* PlayerOwnChar) override;

public:
	UFUNCTION(Server, Reliable)
	void ReqShoot(FVector vStart, FVector vEnd);

public:
	float GetFireStartLength();

	UFUNCTION(BlueprintPure)
	bool IsCanShoot();

	bool UseAmmo();

	void SetAmmo(int Ammo);

	UFUNCTION(BlueprintCallable)
	void OnUpdateAmmoToHud(int Ammo);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere)
	EWeaponType KindOfWeapon;

	UPROPERTY(BlueprintReadWrite)
	ACharacter* m_pOwnChar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* ShootMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* ReloadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* WeaponFireEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* WeaponSoundBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* WeaponReloadSoundBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WeaponDamage;

	UFUNCTION()
	void OnRep_Ammo();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, ReplicatedUsing = OnRep_Ammo)
	int m_Ammo;
};