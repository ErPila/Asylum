// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MultiplayerSessionsSubsystem.generated.h"

// Declaring our own custom delegates for the menu class to binds the callbacks to

/*
create session can take time, so we need a delegate can be called when and just when it happen,  
but here the problem is, how the menu will take the information from those callbacks?
If we use a cast or something then in future we need to use that menu with that subsystem. So is not that independent.
The more efficient way is create other delegates callback so nobody has to know anything about the others.. 
just listening waiting for the callback
*/


// multicast mean multiple classes can binfd this delegate 
// dynamic mean the delegate can be serialized and called from a bluprint class  (in BP event dispatchers)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMpOnCreateSessionComplete, bool, bWasSuccessful);
// i want pass as argument in this delegate the session list this is a TARray of OnlineSessionSearchResult
// this type is not compatible with blueprints so I can't use Dynamic Delegate 
// here we can see for a not dynamic delegate is not necessary separate with a comma type from variable name
DECLARE_MULTICAST_DELEGATE_TwoParams(FMpOnFindSessionsComplete,const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful);
DECLARE_MULTICAST_DELEGATE_OneParam(FMpOnJoinSessionComplete, EOnJoinSessionCompleteResult::Type Result);
// also EOnJoinSessionCompleteResult is not compatible with Blueprints

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMpOnDestroySessionComplete, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMpOnStartSessionComplete, bool, bWasSuccessful);




UCLASS()
class MULTIPLAYERSESSION_API UMultiplayerSessionsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UMultiplayerSessionsSubsystem(); // constructor function

	// to handle session functionality : the menu class will call those functions

	// open a session specifing number of maximum connection and the match type name
	void CreateSession(int32 NumPublicConnections, FString MatchType);

	// fin an existing session
	void FindSessions(int32 MaxSearchResults);

	// join in a session from the previous found
	void JoinSession(const FOnlineSessionSearchResult& Session);

	void DestroySession(); // destroy the session in use

	void StartSession();   

	// variable for our delegate

	FMpOnCreateSessionComplete  MpOnCreateSessionComplete;
	FMpOnFindSessionsComplete   MpOnFindSessionsComplete;
	FMpOnJoinSessionComplete    MpOnJoinSessionComplete;
	FMpOnDestroySessionComplete MpOnDestroySessionComplete;
	FMpOnStartSessionComplete   MpOnStartSessionComplete;

protected:

	// internal callbacks functions for the delegates.  

	void OnCreateSessionComplete(FName SessionName, bool bWasSuccsesful);
	void OnFindSessionsComplete(bool bWasSuccsesful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccsesful);
	void OnStartSessionComplete(FName SessionName, bool bWasSuccsesful);

private:

	IOnlineSessionPtr SessionInterface;

	// a sharedpointer we can access from all the functions
	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;

	// Add to Online session interface some delegates
	// we' ll bind our MultiplayerSessionSubsystem internal callbacks to the delegates. 
	FOnCreateSessionCompleteDelegate  CreateSessionCompleteD;
	FDelegateHandle CreateSessionCompleteDH;

	FOnFindSessionsCompleteDelegate   FindSessionsCompleteD;
	FDelegateHandle FindSessionsCompleteDH;

	FOnJoinSessionCompleteDelegate    JoinSessionCompleteD;
	FDelegateHandle JoinSessionCompleteDH;

	FOnDestroySessionCompleteDelegate DestroySessionCompleteD;
	FDelegateHandle DestroySessionCompleteDH;

	FOnStartSessionCompleteDelegate   StartSessionCompleteD;
	FDelegateHandle StartSessionCompleteDH;

	TSharedPtr<FOnlineSessionSearch> LastSessionSearch;

	bool    bCreateSessionOnDestroy{ false };

	int32   LastNumPublicConnection;

	FString LastMatchType;


};
