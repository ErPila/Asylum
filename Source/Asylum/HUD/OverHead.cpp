// Fill out your copyright notice in the Description page of Project Settings.


#include "OverHead.h"
#include "Components/TextBlock.h"

void UOverHead::SetDisplayText(FString ToDisplay)
{
	if (!DisplayText) return;
	DisplayText->SetText(FText::FromString(ToDisplay));
}

void UOverHead::ShowPlayNetRole(APawn* PN, bool isLocalRole)
{

	//net role e' il ruolo che abbiamo in rete

	//PN->HasAuthority(); // mi dice se sono in authority (server) o meno

	//ENetRole LocalRole{ isLocalRole ? PN->GetLocalRole() : PN->GetRemoteRole() };

	ENetRole LocalRole = PN->GetLocalRole();

	FString Role;

	switch (LocalRole)
	{
	case ROLE_None:
		Role = FString("Role");
		break;

	case ROLE_SimulatedProxy:
		Role = FString("Simulated");
		break;

	case ROLE_AutonomousProxy:
		Role = FString("Autonomous");
		break;

	case ROLE_Authority:
		Role = FString("Autority");
		break;
	}


	/*
	if (PN->HasAuthority())
	{
		Role = FString("Autority");
	} 
	else
	{
		Role = FString("Client");
	}
	*/
	SetDisplayText(Role);
	

}

void UOverHead::NativeDestruct()
{
	Super::NativeDestruct();

	RemoveFromParent();	// quando un livello finisce rimuovo la widget
}
