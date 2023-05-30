// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ResetNotify.generated.h"

/**
 * 
 */
UCLASS()
class ASYLUM_API UResetNotify : public UAnimNotify
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void ResetValues(AActor* Fool);

	
	
};
