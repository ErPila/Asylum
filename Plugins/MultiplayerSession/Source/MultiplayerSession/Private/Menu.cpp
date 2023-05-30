// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu.h"
#include "Components/Button.h"
#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"


void UMenu::MenuSetup(int32 NumberOfPublicConnections, FString TypeOfMatch, FString LobbyPath )
{
	NumPublicConnetcions = NumberOfPublicConnections;
	MatchType = TypeOfMatch;

	PathToLobby = FString::Printf(TEXT("%s?listen"),*LobbyPath);


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

	UGameInstance* GI = GetGameInstance();

    if (GI)  MultiPlayerSessionsSS = GI->GetSubsystem<UMultiplayerSessionsSubsystem>();
	
	if (MultiPlayerSessionsSS) // bind subsystem delegates to the menu functions
	{
		MultiPlayerSessionsSS->MpOnCreateSessionComplete.AddDynamic(this,&ThisClass::OnCreateSession);
		// for not dynamic delegat to bind we use AddUObject
		MultiPlayerSessionsSS->MpOnFindSessionsComplete.AddUObject(this, &ThisClass::OnFindSessions);
		MultiPlayerSessionsSS->MpOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinSession);

		MultiPlayerSessionsSS->MpOnDestroySessionComplete.AddDynamic(this, &ThisClass::OnDestroySession);
		MultiPlayerSessionsSS->MpOnStartSessionComplete.AddDynamic(this, &ThisClass::OnStartSession);

	}

}

bool UMenu::Initialize() // initialize is just after the constructor 
{
	if (!Super::Initialize()) return false;


	ShowSubsystem();


	if (HostButton)  // the name here should be exactlly the same name we have inside the widget
	{   
		HostButton->OnClicked.AddDynamic(this, &ThisClass::HostButtonClicked);
	}

	if (JoinButton)
	{
		JoinButton->OnClicked.AddDynamic(this, &ThisClass::JoinButtonClicked);
	}

	return true;
}

void UMenu::ShowSubsystem()
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();

	if (OnlineSubsystem)
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage
		(-1, 15.f, FColor::Blue, FString::Printf(TEXT("Found subsystem %s"), *OnlineSubsystem->GetSubsystemName().ToString()));
	}
}

void UMenu::NativeDestruct()
{
	MenuTearDown();
	Super::NativeDestruct();

}

void UMenu::OnCreateSession(bool bWasSuccesful)
{
	if (bWasSuccesful)
	{
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, "Session Create");

		UWorld* World = GetWorld();

		if (World)
		{
			World->ServerTravel(PathToLobby);
      	}

	}
	else
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "Create Session Fail");
		HostButton->SetIsEnabled(true);

	}
}

void UMenu::OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful)
{  
	// this part should be refined for more tests can even stop here chosing the session etc.
	if (!MultiPlayerSessionsSS)
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString("Missing Subsystem"));
		return;
	}  // if no subsystem or fail find sessions i return

	if (!bWasSuccessful  || SessionResults.Num() <= 0)
	{
		JoinButton->SetIsEnabled(true);
		if (GEngine) GEngine->AddOnScreenDebugMessage (-1, 15.f, FColor::Red, FString("Find session Fail"));
		return;
	}  // if no subsystem or fail find sessions i return

//	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("Results number   %d "), SessionResults.Num()));

	for (auto Result : SessionResults)
	{
		FString Id   = Result.GetSessionIdStr();       // memorize session id and player just in case
		FString User = Result.Session.OwningUserName;

		if (GEngine && DEBUG) 
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("ID   %s "), *Id));
    		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("User %s "), *User));
		}
		FString SettingsValue; // get and save the session sttings
		Result.Session.SessionSettings.Get(FName("MatchType"), SettingsValue);

		//Result.Session.SessionSettings.

		if (SettingsValue == MatchType) // match type is my class variable editable from BP
		{

			MultiPlayerSessionsSS->JoinSession(Result);
			return;
		}

	}

}

void UMenu::OnJoinSession(EOnJoinSessionCompleteResult::Type Result)
{
	// get correct address and use client travel 
	// first we need to have here the SessionInterface and to do that we use same tecnique 
	// already did on the subsystem. 
	if (GEngine && DEBUG) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Purple, FString("Join"));

	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();

		if (!SessionInterface.IsValid()) return;


		FString Address;

		if (SessionInterface->GetResolvedConnectString(NAME_GameSession, Address)) 
		{
			//if is a success we retrive the Address and are ready to travel in the server
			if (GEngine && DEBUG) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("Connect to %s "), *Address));

			APlayerController* PC = GetGameInstance()->GetFirstLocalPlayerController();

			if (PC) PC->ClientTravel(Address, ETravelType::TRAVEL_Absolute);

		}
		else
		{
			// fail message
		if (GEngine && DEBUG) GEngine->AddOnScreenDebugMessage (-1, 15.f, FColor::Yellow, FString::Printf(TEXT("Failed in find the Address")));

		}
	}

	if (Result != EOnJoinSessionCompleteResult::Success)
	{

		JoinButton->SetIsEnabled(true);

	}


}

void UMenu::OnDestroySession(bool bWasSuccessful)
{
	// write here what the menù do if a session is destroyed.

}

void UMenu::OnStartSession(bool bWasSuccessful)
{
	// write here what happen if a game session start.

}

void UMenu::HostButtonClicked()
{

	HostButton->SetIsEnabled(false);

	if (MultiPlayerSessionsSS)
	{
		if (GEngine && DEBUG) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("Match %s "), *MatchType));
		MultiPlayerSessionsSS->CreateSession(NumPublicConnetcions, MatchType);
	}

}

void UMenu::JoinButtonClicked()
{
	JoinButton->SetIsEnabled(false);

	if (MultiPlayerSessionsSS)
	{
		MultiPlayerSessionsSS->FindSessions(10000);
	}
}

void UMenu::MenuTearDown()
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
