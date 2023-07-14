// Fill out your copyright notice in the Description page of Project Settings.


#include "FightMode.h"
#include "Kismet/GameplayStatics.h"
#include "Asylum/Characters/BaseChar.h"
#include "Asylum/Components/Combat.h"

void AFightMode::BeginPlay()
{
	Super::BeginPlay();

	//Faccio partire il timer per il controllo della vita
	GetWorldTimerManager().SetTimer(CheckHealth, this, &AFightMode::EndGame, 1.f, true, 5.f);
}

//funzione che controlla la vita dei giocatori una volta al secondo,
//se un giocatore ha la vita <= 0 richiamo la funzione morte di quel giocatore, imposto la win a false, 
//richiamo la funzione per il widget e cancello il loop del timer.
void AFightMode::EndGame()
{
	for (int i = 0; i < Players.Num(); i++)
	{
		auto Giocatore = Cast<ABaseChar>(Players[i]);
		if (Giocatore)
		{
			if (Giocatore->GetCombat()->GetActualHp() <= 0)
			{
				Giocatore->GetCombat()->Die_Multicast();
				Giocatore->Win = false;
				AddWidget();
				GetWorldTimerManager().ClearTimer(CheckHealth);
				return;
			}
		}
	}
}


//Richiamo la funzione per far apparire il widget su tutti i giocatori
void AFightMode::AddWidget()
{
	for (int i = 0; i < Players.Num(); i++)
	{
		auto Giocatore = Cast<ABaseChar>(Players[i]);
		if (Giocatore)
		{
			Giocatore->AddEndWidget();
		}
	}
}
