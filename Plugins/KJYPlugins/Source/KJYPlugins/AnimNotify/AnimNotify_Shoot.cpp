// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_Shoot.h"
#include "WeaponInterface.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"


void UAnimNotify_Shoot::OnAnimNotifyInterface_Implementation(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	
	// Cast the owner to the IWeaponInterface
	IWeaponInterface* WeaponInterface = Cast<IWeaponInterface>(MeshComp->GetOwner());
	if (WeaponInterface)
	{
		// Call the Shoot method
		WeaponInterface->EventShoot();
	}

}
