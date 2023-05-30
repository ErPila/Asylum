// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyMode.h"
#include "GameFramework/GameStateBase.h"

void ALobbyMode::PostLogin(APlayerController* NewPlayer)
{

	Super::PostLogin(NewPlayer);

	int32 NumberOfPlayers{ GameState.Get()->PlayerArray.Num() }; // this tell the number of players

	if (NumberOfPlayers >= 2)
	{
		UWorld* World{ GetWorld() };

		if (World) // we want travel without lost connection when the players are two or more
		{
			bUseSeamlessTravel = true; // travel without disconnect
			//this server travel should have different 
			World->ServerTravel(FString("/Game/_Assets/Maps/Asylum?listen"));

		}
	}

}
