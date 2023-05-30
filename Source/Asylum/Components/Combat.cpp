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

void UCombat::PutInBackPack(AWeapon* ToInsert)
{
	/*if (BackPack.Num() < 5)
	{
		BackPack.Add(ToInsert);
	}
	else
	{

	}*/
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
	SetIsReplicated(true);
	//bReplicates = true;
	PrimaryComponentTick.bCanEverTick = true;
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

void UCombat::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, EquippedWeapon); // replica sempre la variabile
	//DOREPLIFETIME(ThisClass, BackPack);
	//DOREPLIFETIME_CONDITION(ThisClass, EquippedWeapon, COND_OwnerOnly);

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
	
      	EquippedWeapon = WeaponToEquip;
		EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipment);

		auto HandSocket = Character->GetMesh()->GetSocketByName("Grip_r");

		if (HandSocket)
		{
			HandSocket->AttachActor(EquippedWeapon, Character->GetMesh());
		}

		//CollectWeapon(EquippedWeapon); // inserisco arma nell'array




	//EquippedWeapon->SetActorLocation(EquippedWeapon->GetActorLocation() + EquippedWeapon->LocationOffset);
	//EquippedWeapon->AddActorLocalOffset(EquippedWeapon->LocationOffset);
	//Character->GetCharacterMovement()->bOrientRotationToMovement = false;
	//Character->bUseControllerRotationYaw = true;
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

	
	if (EquippedWeapon) // ho già un arma equipaggiata
	{
		UE_LOG(LogTemp, Warning, TEXT("Vai nel Backpack"));

		WeaponToCollect->SetWeaponStateServer(EWeaponState::EWS_Backpack);

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


// on rep viene eseguito su tutti i client ma non sul server
void UCombat::OnRep_EquipWeapon(AWeapon* EW)
{
	UE_LOG(LogTemp, Warning, TEXT("On rep Equip"));
	
	if(EquippedWeapon)
	{
		EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipment);
	}
	else
	{
		if (EW)
		{
			Character->bCanFire = true;
			EW->SetWeaponState(EWeaponState::EWS_Dropped);

			if (EW->GetRootSphere()->IsSimulatingPhysics())
			{
				EW->GetRootSphere()->AddImpulse(Character->GetActorForwardVector() * 1000 * EW->GetRootSphere()->GetMass());
			}
			else
			{
				//EW->GetRootSphere()->SetSimulatePhysics(true);
				//EW->GetRootSphere()->AddImpulse(Character->GetActorForwardVector() * 1000 * EW->GetRootSphere()->GetMass());
			}

		}
	}
}



