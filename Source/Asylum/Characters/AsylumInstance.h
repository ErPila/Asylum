// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AsylumInstance.generated.h"

/**
 * 
 */
UCLASS()
class ASYLUM_API UAsylumInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	uint8 SelectedCharacter{ 0 };

};
