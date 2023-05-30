// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "FXNotify.generated.h"

/**
 * 
 */
UCLASS()
class ASYLUM_API UFXNotify : public UAnimNotify
{
	GENERATED_BODY()

		UPROPERTY(EditAnyWhere)
		class USoundCue* Sound;

	    UPROPERTY(EditAnyWhere)
		class UNiagaraSystem* Sfx;

		UPROPERTY(EditAnyWhere)
		FName BoneName;

public:
		UFUNCTION(BlueprintCallable)
		void SpawnEffects(USkeletalMeshComponent* Mesh);

};
