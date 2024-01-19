// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimInterface.h"
#include "AnimNotify_Shoot.generated.h"

UCLASS()
class KJYPLUGINS_API UAnimNotify_Shoot : public UAnimNotify , public IAnimInterface
{
	GENERATED_BODY()
	
public:

	virtual void OnAnimNotifyInterface_Implementation(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference);
};
