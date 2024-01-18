// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WeaponInterface.generated.h"

 // UWeaponInterface 는 UInterface 클래스를 상속받아 구현한 언리얼의 인터페이스입니다.
UINTERFACE(Blueprintable)
class KJYPLUGINS_API UWeaponInterface : public UInterface
{
	// GENERATED_BODY() 는 언리얼 엔진 코드 생성 시 필요한 추가 코드를 생성하기 위한 매크로입니다.
	GENERATED_BODY()
};

/**
 * SHOOTINGCODEGAME_API 는 클래스를 내보내는(dll로 만들어진 모듈에서 사용 가능하도록 하는) 매크로입니다.
 * 이는 해당 인터페이스가 모듈 외부에서도 사용될 수 있도록 하는 역할을 합니다.
 */
class KJYPLUGINS_API IWeaponInterface
{
	GENERATED_BODY()

public:
		// IWeaponInterface 는 아래의 함수를 가지고 있습니다.
		// 이는 인터페이스를 구현한 클래스에서 반드시 구현되어야 하는 함수입니다.
	// 트리거이벤트
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventTrigger();

	// 총알발사이벤트
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventShoot();
	
	// 재장전
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventReload();
	
	// 총알 다시 채우기
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventResetAmmo();
	
	 //줍기
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventPickUp(ACharacter* pOwnChar);
	
	 //버리기
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventDrop(ACharacter* pOwnChar);

};