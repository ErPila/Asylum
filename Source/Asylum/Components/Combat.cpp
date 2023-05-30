// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat.h"
#include "Asylum/Weapons/Weapon.h"
#include "Asylum/Characters/BaseChar.h"
#include "Net/UnrealNetwork.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SphereComponent.h"



AWeapon* UCombat::GetFromBackPack(int32 index)
{

	switch (index)
	{
	case 0: return BackPack.Slot1; break;
	case 1: return BackPack.Slot2; break;
	case 2: return BackPack.Slot3; break;
	default: return nullptr;
	}

}

void UCombat::RemoveFromBackPack(AWeapon* ToRemove, int32 index)
{
	switch (index)
	{
	case 0: BackPack.Slot1 = nullptr; break;
	case 1: BackPack.Slot2 = nullptr; break;
	case 2: BackPack.Slot3 = nullptr; break;
	}


}

// Sets default values for this component's properties
UCombat::UCombat():
Max_Hp(100),
Actual_Hp(Max_Hp),
Max_Sanity(60),
Actual_Sanity(Max_Sanity)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//bReplicatesComp = true;
	//bReplicates = true;
	PrimaryComponentTick.bCanEverTick = true;
}


void UCombat::SetWeaponStateServer_Implementation(EWeaponState NewState, AWeapon* ActualW)
{
	UE_LOG(LogTemp, Warning, TEXT("Weapon set on server"));

	ActualW->SetWeaponState(NewState);
}

// Called when the game starts
void UCombat::BeginPlay()
{
	Super::BeginPlay();
	Actual_Hp     = Max_Hp;
	Actual_Sanity = Max_Sanity;

	

// tre slot per il backpack
//	auto HandSocket = Character->GetMesh()->GetSocketByName("Grip_r");
//	BackupLocation =  HandSocket->RelativeLocation;

}


// Called every frame
void UCombat::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Actual_Sanity > 0)
	{
		Actual_Sanity -= DeltaTime;
	}
}

void UCombat::EquipWeapon(AWeapon* WeaponToEquip)
{


	if (!Character || !WeaponToEquip) return;	// se manca uno dei due elementi indispensalibi, esco

	GEngine->AddOnScreenDebugMessage(1, 10.f, FColor::Red, FString("OnReplication"));
	if (Character->HasAuthority())  UE_LOG(LogTemp, Warning, TEXT("equipping the weapons %s"),*Character->GetName() );
	
	    Character->EquippedWeapon = WeaponToEquip;
		SetWeaponStateServer(EWeaponState::EWS_Equipment, Character->EquippedWeapon);
		//Character->EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipment);

		auto HandSocket = Character->GetMesh()->GetSocketByName("Grip_r");

		if (HandSocket)
		{
			HandSocket->AttachActor(Character->EquippedWeapon, Character->GetMesh());
		}

		//CollectWeapon(EquippedWeapon); // inserisco arma nell'array

}

bool UCombat::CollectWeapon(AWeapon* WeaponToCollect)
{
	// l'oggetto tracciato diventa "da collezionare" 
	// nel caso ci sia uno spazio libero nel Backpack lo si inserisce
	// altrimenti la funzione restituisce false // qui stiamo ancora in locale 

	if      (!BackPack.Slot1) { BackPack.Slot1 = WeaponToCollect; }
	else if (!BackPack.Slot2) { BackPack.Slot2 = WeaponToCollect; }
	else if (!BackPack.Slot3) { BackPack.Slot3 = WeaponToCollect; }
	else    return false;

	
	if (Character->EquippedWeapon) // ho già un arma equipaggiata
	{
		UE_LOG(LogTemp, Warning, TEXT("Vai nel Backpack"));

		SetWeaponStateServer(EWeaponState::EWS_Backpack, WeaponToCollect);
	}
	else
	{
		Character->Equip_Server(WeaponToCollect);
		UE_LOG(LogTemp, Warning, TEXT("Equipaggia subito"));		
	}
	
	return true;
	//CheckBackpackFull();
}


// Controllo se lo zaino è pieno
void UCombat::CheckBackpackFull()
{
/*	for (int i = 0; i < BackPack.Num(); i++)
	{

		if (BackPack[i] == nullptr)
		{
			bIsFull = false;
			return;
		}
	}

	bIsFull = true;*/
}




