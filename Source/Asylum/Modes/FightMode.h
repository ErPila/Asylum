// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "FightMode.generated.h"

/**
 * 
 */
UCLASS()
class ASYLUM_API AFightMode : public AGameMode
{
	GENERATED_BODY()

		void BeginPlay() override;

	TSubclassOf<class ABaseChar> Char;
	TArray<AActor*> Players;

public:

		UFUNCTION(BlueprintImplementableEvent)
		void EndGame();

	
};
