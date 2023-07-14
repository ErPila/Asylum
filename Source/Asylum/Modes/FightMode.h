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

	FTimerHandle CheckHealth;

public:


	// Array inizializzato da blueprint nel beginplay, all'interno ci sono tutti i giocatori in partita
	UPROPERTY(BlueprintReadWrite, Category = "Giocatori")
	TArray<AActor*> Players;

	
	void EndGame();

	void AddWidget();

	
};
