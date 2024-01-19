// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AnimInterface.generated.h"

UINTERFACE(Blueprintable)
class KJYPLUGINS_API UAnimInterface : public UInterface
{
	GENERATED_BODY()
	
};

class KJYPLUGINS_API IAnimInterface
{
	GENERATED_BODY()

public:
	// IWeaponInterface �� �Ʒ��� �Լ��� ������ �ֽ��ϴ�.
	// �̴� �������̽��� ������ Ŭ�������� �ݵ�� �����Ǿ�� �ϴ� �Լ��Դϴ�.
// Ʈ�����̺�Ʈ
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnAnimNotifyInterface(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation, const FAnimNotifyEvent& NotifyEvent);
};