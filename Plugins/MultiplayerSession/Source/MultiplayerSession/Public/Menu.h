// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define DEBUG 0

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Menu.generated.h"

class UButton;

UCLASS()
class MULTIPLAYERSESSION_API UMenu : public UUserWidget
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable)
	void MenuSetup(int32 NumberOfPublicConnections = 4, FString TypeOfMatch = FString(TEXT("FreeForAll")), FString LobbyPath = TEXT("/Game/ThirdPerson/Maps/Lobby") );

protected:

	virtual bool Initialize() override;
	void ShowSubsystem();
	// this happen when a level disappear (so when we travel to another one)
	virtual void NativeDestruct() override;


	// Callbacks for the custom delegates on multiplayer sessions subsistem

	UFUNCTION() // because our delegate is dynamic here we need ufunction
	void OnCreateSession(bool bWasSuccesful);

	void OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful);

	void OnJoinSession(EOnJoinSessionCompleteResult::Type Result);

	UFUNCTION() // because our delegate is dynamic here we need ufunction
	void OnDestroySession(bool bWasSuccessful);

	UFUNCTION() // because our delegate is dynamic here we need ufunction
	void OnStartSession(bool bWasSuccessful);

	// so only the dynamic delegates hav ufucntion


private :

	// this meta link this variable to the button on the children WBP
	// name of the variable should be exactly the same of the button
	UPROPERTY(meta = (BindWidget))  
	UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinButton;

	UFUNCTION()
	void HostButtonClicked();

	UFUNCTION()
	void JoinButtonClicked();


	void MenuTearDown();

	// pointer to our subsystem ( it handle online sessions functionality)
	class UMultiplayerSessionsSubsystem* MultiPlayerSessionsSS;

	int32   NumPublicConnetcions{ 4 };

	UPROPERTY(EditAnyWhere)
	FString MatchType { TEXT("FreeForAll") };


	FString PathToLobby{ TEXT("Game/ThirdPerson/Maps/ThirdPersonMap.umap") };


};
