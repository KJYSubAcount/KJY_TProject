// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_Shoot.h"
#include "WeaponInterface.h"

void UAnimNotify_Shoot::OnAnimNotifyInterface_Implementation(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	// �ִϸ��̼� ��Ƽ���̰� ����Ǵ� �ִϸ��̼��� ������ ���͸� ã���ϴ�.
	AActor* pOwner = MeshComp->GetOwner();
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("powner"));
	// �ִϸ��̼��� ������ ���Ͱ� IWeaponInterface �� ��ӹ޾Ҵ��� Ȯ���մϴ�.

	IWeaponInterface* pWeaponInterface = Cast<IWeaponInterface>(pOwner);

	if (nullptr != pWeaponInterface)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("pownergood"));
		// IWeaponInterface �� ��ӹ޾Ҵٸ� EventShoot �Լ��� ȣ���մϴ�.
		pWeaponInterface->Execute_EventShoot(pOwner);
	}
}
