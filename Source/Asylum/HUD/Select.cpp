// Fill out your copyright notice in the Description page of Project Settings.


#include "Select.h"
#include "Asylum/Characters/AsylumInstance.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"


void USelect::MenuSetup()  // create this menù and add it to viewport
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

}


void USelect::SetAnge()
{
	Selected = 0;
	if (!Luce || !Angel) return;
	Luce->SetActorLocation(Angel->GetActorLocation() + FVector(-60, 0, 200));
}

void USelect::SetChri()
{
	Selected = 1;
	if (!Luce || !Chris) return;
	Luce->SetActorLocation(Chris->GetActorLocation() + FVector(-60, 0, 200));
}

void USelect::SetLuca()
{
	Selected = 2;
	if (!Luce || !Lucas) return;
	Luce->SetActorLocation(Lucas->GetActorLocation() + FVector(-60, 0, 200));
}

void USelect::StartPressed()
{
	auto GS{ Cast<UAsylumInstance>(GetWorld()->GetGameInstance()) };

	if (GS)	GS->SelectedCharacter = Selected;

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


	UGameplayStatics::OpenLevel(this, FName("HostJoin"));
}

void USelect::TestPressed()
{
	auto GS{ Cast<UAsylumInstance>(GetWorld()->GetGameInstance()) };

	if (GS) GS->SelectedCharacter = Selected;

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

	UGameplayStatics::OpenLevel(this, FName("Asylum"));
}

bool USelect::Initialize()
{
	if (!Super::Initialize()) return false;
	// bind all the available buttons.  now those buttons will be activated by buttons
	// with the same name from blueprints
	if (Ange)  Ange->OnClicked.AddDynamic(this, &ThisClass::SetAnge);
	if (Chri)  Chri->OnClicked.AddDynamic(this, &ThisClass::SetChri);
	if (Luca)  Luca->OnClicked.AddDynamic(this, &ThisClass::SetLuca);
	if (Start) Start->OnClicked.AddDynamic(this, &ThisClass::StartPressed);
	if (Test)  Test->OnClicked.AddDynamic(this, &ThisClass::TestPressed);

	for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		AActor* TMP = *ActorItr;

		if (TMP->ActorHasTag("Ange")) { Angel = TMP; UE_LOG(LogTemp, Error, TEXT("Found Ange")); };
		if (TMP->ActorHasTag("Chri")) { Chris = TMP; UE_LOG(LogTemp, Error, TEXT("Found Chri")); };
		if (TMP->ActorHasTag("Luca")) { Lucas = TMP; UE_LOG(LogTemp, Error, TEXT("Found Luca")); };
		if (TMP->ActorHasTag("Light")){ Luce = TMP;  UE_LOG(LogTemp, Error, TEXT("Found Light")); };
	}

	if (Luce && Luca)
	{
		//Selected = 2;
	//	Luce->SetActorLocation(Chris->GetActorLocation() + FVector(-60, 0, 200));
		SetLuca();
	}


	return true;
}
