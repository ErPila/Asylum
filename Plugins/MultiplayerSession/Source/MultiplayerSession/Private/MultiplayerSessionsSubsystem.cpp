// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

UMultiplayerSessionsSubsystem::UMultiplayerSessionsSubsystem():
	CreateSessionCompleteD(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete)),
	FindSessionsCompleteD(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionsComplete)),
	JoinSessionCompleteD(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionComplete)),
	DestroySessionCompleteD(FOnDestroySessionCompleteDelegate::CreateUObject(this, &ThisClass::OnDestroySessionComplete)),
	StartSessionCompleteD(FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartSessionComplete))
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		SessionInterface = Subsystem->GetSessionInterface();

	}
}

void UMultiplayerSessionsSubsystem::CreateSession(int32 NumPublicConnections, FString MatchType)
{
	

	if (!SessionInterface.IsValid()) return;

	auto ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession);

	if (ExistingSession)
	{
		bCreateSessionOnDestroy = true;
		LastNumPublicConnection = NumPublicConnections;
		LastMatchType = MatchType;
		DestroySession();	
		return; // remember totest it
	}

	// store the delegate in the FDelegateHandle, so we can later remove from delegate list
	CreateSessionCompleteDH = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteD);

	LastSessionSettings = MakeShareable(new FOnlineSessionSettings());
	// if we are online lan is false if the subsystem is NULL (unreal default) we are in a lan match
	LastSessionSettings->bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;
	// the num public connections is thefined from the arguments
	LastSessionSettings->NumPublicConnections = NumPublicConnections;

	LastSessionSettings->bAllowJoinInProgress  = true; // always can
	LastSessionSettings->bAllowJoinViaPresence = true; // is something used by steam
	
	LastSessionSettings->bShouldAdvertise = true;
	LastSessionSettings->bUsesPresence    = true;

	// this at 1 prevent us tu stuck at the first session we find joining. so if 
	// more session are available we can see all and join the one we want.
	LastSessionSettings->BuildUniqueId = 1;

	LastSessionSettings->bUseLobbiesIfAvailable = true;

	// the kind of the match is also from argumetns // MatchType
	LastSessionSettings->Set(FName("MatchType"), MatchType, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	
	if (!SessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *LastSessionSettings))
	{
		// if fail to create session, remove the delegate Handle
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDH);
		// Broadcast our own custom delegate
		MpOnCreateSessionComplete.Broadcast(false); 
	}
	

}

void UMultiplayerSessionsSubsystem::FindSessions(int32 MaxSearchResults)
{
	if (!SessionInterface.IsValid()) return;
	
	// bind the delegate to the find session complete event catch in the handle to clean it up later
	FindSessionsCompleteDH = SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteD);

	LastSessionSearch = MakeShareable(new FOnlineSessionSearch());

	LastSessionSearch->MaxSearchResults = MaxSearchResults; // high number because now we using stam test port.. so probably many people use it
	LastSessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false; // lan or not ?
	LastSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	// in the way above we want be sure che the session i find use SEARCH_PRESENCE , for that 
	// the onlinecomparison options is Equal 
	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();

	if (!SessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), LastSessionSearch.ToSharedRef()))
	{
		// if we not find any session. i clear the delegate and callback the delegate with an empty array
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDH);
		MpOnFindSessionsComplete.Broadcast(TArray<FOnlineSessionSearchResult>(),false);
	}
	
		// if the search is a success it call automatically the function we bind at the FinSessionCompleteD (delegate)
	    // OnFindSessionsComplete 

}

void UMultiplayerSessionsSubsystem::JoinSession(const FOnlineSessionSearchResult& Session)
{
	if (!SessionInterface.IsValid())
	{
		// something fail we can send to menu an error
		MpOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		if (GEngine) GEngine->AddOnScreenDebugMessage (-1, 15.f, FColor::Red, FString("Session interface not found"));
		return; // no session interface, return

	}

	// save the delegate handle to remove the bind when later will be unnecessary
	JoinSessionCompleteDH = SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteD);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	// try to join session using our ID and the result we find before

	
	if (!SessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, Session))
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDH); // free the delegate
    	MpOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);

		// if fail joining we go here, if success the callback redirect to the OnJoinSessionComplete function down here
	}

}

void UMultiplayerSessionsSubsystem::DestroySession()
{
	if (!SessionInterface.IsValid()) // something go wrong and the interface disappear
	{
		MpOnDestroySessionComplete.Broadcast(false); // so i fail to destroy the session
		return;			
	}

	// I bind my delegate to destroy session funcion and save the bind in an handle to clear it later
	DestroySessionCompleteDH = SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteD);

	if (!SessionInterface->DestroySession(NAME_GameSession)) // the destroy session function fail
	{
		SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDH);
		MpOnDestroySessionComplete.Broadcast(false); // so i fail to destroy the session
		return;
	}

}

void UMultiplayerSessionsSubsystem::StartSession()
{

}

void UMultiplayerSessionsSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccsesful)
{
	// we not need the handle anymore, the callback is already done.
	if(SessionInterface)SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDH);
	MpOnCreateSessionComplete.Broadcast(bWasSuccsesful); // if it succed this onw is true...

}

void UMultiplayerSessionsSubsystem::OnFindSessionsComplete(bool bWasSuccsesful)
{

	
	if (SessionInterface)
	{   // clear the delegate
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDH);		
	}

	if (LastSessionSearch->SearchResults.Num() <= 0)
	{   // we find no sessions so it is false anyway
		MpOnFindSessionsComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		
	}
	else
	{
		// send the correct TArray and  a Sucess
		MpOnFindSessionsComplete.Broadcast(LastSessionSearch->SearchResults, bWasSuccsesful);
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("more than zero session ")));

	}
}

void UMultiplayerSessionsSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (SessionInterface)
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDH);
	}

	MpOnJoinSessionComplete.Broadcast(Result); // now the join is complete and we can tell it to the menu 
	// so Subsystem communicate to the menu that the join is done succesfully 

}

void UMultiplayerSessionsSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccsesful)
{ // a session is destroyed successfully

	if (SessionInterface)
	{
		//After the callback we can clear the handle
		SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDH);
		//MpOnDestroySessionComplete.Broadcast(true); // so i fail to destroy the session

	}

	// we try to create a new session only if the flag is true
	if (bWasSuccsesful  && bCreateSessionOnDestroy)  
	{
		bCreateSessionOnDestroy = false; 
		CreateSession(LastNumPublicConnection,LastMatchType);
	}

	MpOnDestroySessionComplete.Broadcast(bWasSuccsesful); // now we can tell to the menu the destroy is succedded

}

void UMultiplayerSessionsSubsystem::OnStartSessionComplete(FName SessionName, bool bWasSuccsesful)
{

}
