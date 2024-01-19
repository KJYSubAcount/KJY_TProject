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
	// IWeaponInterface 는 아래의 함수를 가지고 있습니다.
	// 이는 인터페이스를 구현한 클래스에서 반드시 구현되어야 하는 함수입니다.
// 트리거이벤트
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnAnimNotifyInterface(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation, const FAnimNotifyEvent& NotifyEvent);
};