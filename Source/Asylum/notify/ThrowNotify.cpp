// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrowNotify.h"
#include "Asylum/Characters/BaseChar.h"

void UThrowNotify::ThrowItem(USkeletalMeshComponent* ActualMesh)
{
	auto MyChar = Cast<ABaseChar>(ActualMesh->GetOwner());

	if (MyChar)
	{
		MyChar->Throw();
	}
}
