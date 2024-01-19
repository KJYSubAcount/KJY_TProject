// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_Shoot.h"
#include "WeaponInterface.h"

void UAnimNotify_Shoot::OnAnimNotifyInterface_Implementation(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	// 애니메이션 노티파이가 실행되는 애니메이션을 소유한 액터를 찾습니다.
	AActor* pOwner = MeshComp->GetOwner();
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("powner"));
	// 애니메이션을 소유한 액터가 IWeaponInterface 를 상속받았는지 확인합니다.

	IWeaponInterface* pWeaponInterface = Cast<IWeaponInterface>(pOwner);

	if (nullptr != pWeaponInterface)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("pownergood"));
		// IWeaponInterface 를 상속받았다면 EventShoot 함수를 호출합니다.
		pWeaponInterface->Execute_EventShoot(pOwner);
	}
}
