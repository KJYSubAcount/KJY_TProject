// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemInterface.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	IT_MAG UMETA(DisplayName = "Mag"),
	IT_HEAL UMETA(DisplayName = "Heal"),
};


UINTERFACE(Blueprintable)
class KJYPLUGINS_API UItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * SHOOTINGCODEGAME_API �� Ŭ������ ��������(dll�� ������� ��⿡�� ��� �����ϵ��� �ϴ�) ��ũ���Դϴ�.
 * �̴� �ش� �������̽��� ��� �ܺο����� ���� �� �ֵ��� �ϴ� ������ �մϴ�.
 */
class KJYPLUGINS_API IItemInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventGetItem(EItemType itemType);

};