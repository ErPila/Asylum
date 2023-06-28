// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ThrowNotify.generated.h"

/**
 * 
 */
UCLASS()
class ASYLUM_API UThrowNotify : public UAnimNotify
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
	void ThrowItem(USkeletalMeshComponent* ActualMesh, float Intensity = 1000);



};
