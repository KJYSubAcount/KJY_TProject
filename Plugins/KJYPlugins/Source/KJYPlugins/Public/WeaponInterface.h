// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WeaponInterface.generated.h"

 // UWeaponInterface �� UInterface Ŭ������ ��ӹ޾� ������ �𸮾��� �������̽��Դϴ�.
UINTERFACE(Blueprintable)
class KJYPLUGINS_API UWeaponInterface : public UInterface
{
	// GENERATED_BODY() �� �𸮾� ���� �ڵ� ���� �� �ʿ��� �߰� �ڵ带 �����ϱ� ���� ��ũ���Դϴ�.
	GENERATED_BODY()
};

/**
 * SHOOTINGCODEGAME_API �� Ŭ������ ��������(dll�� ������� ��⿡�� ��� �����ϵ��� �ϴ�) ��ũ���Դϴ�.
 * �̴� �ش� �������̽��� ��� �ܺο����� ���� �� �ֵ��� �ϴ� ������ �մϴ�.
 */
class KJYPLUGINS_API IWeaponInterface
{
	GENERATED_BODY()

public:
		// IWeaponInterface �� �Ʒ��� �Լ��� ������ �ֽ��ϴ�.
		// �̴� �������̽��� ������ Ŭ�������� �ݵ�� �����Ǿ�� �ϴ� �Լ��Դϴ�.
	// Ʈ�����̺�Ʈ
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventTrigger();

	// �Ѿ˹߻��̺�Ʈ
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventShoot();
	
	// ������
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventReload();
	
	// �Ѿ� �ٽ� ä���
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventResetAmmo();
	
	 //�ݱ�
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventPickUp(ACharacter* pOwnChar);
	
	 //������
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventDrop(ACharacter* pOwnChar);

};