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
 * SHOOTINGCODEGAME_API 는 클래스를 내보내는(dll로 만들어진 모듈에서 사용 가능하도록 하는) 매크로입니다.
 * 이는 해당 인터페이스가 모듈 외부에서도 사용될 수 있도록 하는 역할을 합니다.
 */
class KJYPLUGINS_API IItemInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventGetItem(EItemType itemType);

};