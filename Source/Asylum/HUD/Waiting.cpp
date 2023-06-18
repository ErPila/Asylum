// Fill out your copyright notice in the Description page of Project Settings.


#include "Waiting.h"
#include "Components/Button.h"
#include "Asylum/Characters/AsylumInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

void UWaiting::MenuSetup()
{
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;

	UWorld* World = GetWorld();

	if (World)
	{
		APlayerController* PC = World->GetFirstPlayerController();

		if (PC)
		{
			// want use the input only in the menu.. not for the pawn
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget()); // take focus on this widget
			// keep the mouse move outside the window
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PC->SetInputMode(InputModeData); // now imput mode for this PC is defined 
			PC->SetShowMouseCursor(true);
		}
	}

	auto GS{ Cast<UAsylumInstance>(GetWorld()->GetGameInstance()) };

	//if (GS)	GS->SelectedCharacter;

	// qui da selectedCharacter possiamo far apparire il personaggio giusto nel loading

}

void UWaiting::MenuEnd()
{


	RemoveFromParent();

	UWorld* World = GetWorld();

	if (World)
	{
		APlayerController* PC = World->GetFirstPlayerController();
		if (PC)
		{
			// game only, focus on the game !
			FInputModeGameOnly InputModeData;
			PC->SetInputMode(InputModeData); // now imput mode for this PC is defined 
			PC->SetShowMouseCursor(false);  //mouse cursor disappear
		}
	}

}
