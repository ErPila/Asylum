// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseChar.h"
//	Per gli advanced input
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
//	Utili
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerInput.h"
#include "GameFramework/CharacterMovementComponent.h"
//	Componenti
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"
#include "Asylum/Components/Combat.h"
// network
#include "Net/UnrealNetwork.h"
// Classi esterne
#include "Asylum/Weapons/Weapon.h"
#include "Asylum/Weapons/CollectInterface.h"
#include "Asylum/Characters/AsylumInstance.h"
// debug
#include "DrawDebugHelpers.h"




void ABaseChar::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, SelectedMesh); // replica sempre la variabile
	DOREPLIFETIME(ThisClass, EquippedWeapon);
	DOREPLIFETIME(ThisClass, Damage);
	//DOREPLIFETIME(ThisClass, TracedWeapon);
//	DOREPLIFETIME_CONDITION(ThisClass, SelectedMesh, COND_OwnerOnly); // replica sempre la variabile
}

// Sets default values
ABaseChar::ABaseChar():
	bIsInAir(false),
	bCanJump(true)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("ArmComponent"));
	ArmComponent->SetupAttachment(GetMesh());
	ArmComponent->TargetArmLength = 500;
	ArmComponent->bUsePawnControlRotation = true;	// lo spring arm segue la rotazione del controller

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(ArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;	// la camera segue lo spring arm

	CombatComponent = CreateDefaultSubobject<UCombat>(TEXT("CombatComponent"));
	CombatComponent->Character = this;

	bUseControllerRotationYaw = false;	// il personaggio non ruota con la camera
	GetCharacterMovement()->bOrientRotationToMovement = false; // Ruoto il character nella direzione in cui si muove
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

	DebugWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("DebugWidget"));
	DebugWidget->SetupAttachment(RootComponent);

	
}

AWeapon* ABaseChar::GetEquippedWeapon()
{
	return EquippedWeapon;
}

bool ABaseChar::IsEquipped()
{
	return EquippedWeapon ? true : false;
}

// Called when the game starts or when spawned
void ABaseChar::BeginPlay()
{
	Super::BeginPlay();

	AddContext(MC_Combat, 0);

	StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
	LocallyControlled = IsLocallyControlled();

		
  
    auto GS{ Cast<UAsylumInstance>(GetWorld()->GetGameInstance()) };

    if (IsLocallyControlled())
	   {
		   // case 1  Locally controlled Client
		   if (!HasAuthority())  // in this case I will change immediately my gfx then ask the server to do the same
		   {
			   Begin_Server(GS->SelectedCharacter); // call a  function to set up gfx on the server 
			   GetMesh()->SetSkeletalMesh(CharMeshes[GS->SelectedCharacter]);
			   SelectedMesh = GS->SelectedCharacter;
		   }
		   else	  
		   {  
			   GetMesh()->SetSkeletalMesh(CharMeshes[GS->SelectedCharacter]);		  
			   SelectedMesh = GS->SelectedCharacter;
		   }
	   }
		
}


void ABaseChar::Begin_Server_Implementation(uint8 Selected)
{
	GetMesh()->SetSkeletalMesh(CharMeshes[Selected]);	
	//Begin_Multicast(Selected);  
	SelectedMesh = Selected;
	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Blue, FString::Printf(TEXT("Change on server I %i"), Selected));
}


//Funzione per aggiungere il mapping context ad una data priorità

void ABaseChar::AddContext(UInputMappingContext* MC, int32 priority)
{
	auto PC = Cast<APlayerController>(Controller);
	if (!PC) return;
	auto Sub = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	if (!Sub) return;
	Sub->AddMappingContext(MC, priority);
}

void ABaseChar::OnRep_ChangeMesh()
{

//	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Red, FString::Printf(TEXT("On Replication %i"), SelectedMesh));

	GetMesh()->SetSkeletalMesh(CharMeshes[SelectedMesh]);
}

void ABaseChar::CollectPills()
{
	if (CombatComponent)
	{
		CombatComponent->Actual_Sanity += 15;

		if (CombatComponent->Actual_Sanity > CombatComponent->Max_Sanity)
		{
			CombatComponent->Actual_Sanity = CombatComponent->Max_Sanity;
		}
	}
	
}




// Called to bind functionality to input
void ABaseChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto EPI = Cast<UEnhancedInputComponent>(PlayerInputComponent);		//Cast agli input avanzati
	if (!EPI) return;

	//	Attack
	EPI->BindAction(InputActions[0], ETriggerEvent::Started, this, &ThisClass::Attack_Button);
	//	Collect
	EPI->BindAction(InputActions[1], ETriggerEvent::Started, this, &ThisClass::Equip_Button);
	//	Crouch
	EPI->BindAction(InputActions[2], ETriggerEvent::Started, this, &ThisClass::CrouchBP);
	//	Jump
	EPI->BindAction(InputActions[3], ETriggerEvent::Started, this, &ThisClass::JumpIA);
	EPI->BindAction(InputActions[3], ETriggerEvent::Completed, this, &ThisClass::JumpIA);
	//	Look
	EPI->BindAction(InputActions[4], ETriggerEvent::Triggered, this, &ThisClass::Look);
	//	Move
	EPI->BindAction(InputActions[5], ETriggerEvent::Triggered, this, &ThisClass::Move);

	EPI->BindAction(InputActions[6], ETriggerEvent::Started, this, &ThisClass::Drop_Button);

	EPI->BindAction(InputActions[7], ETriggerEvent::Started, this, &ThisClass::Slot1_Button);

	EPI->BindAction(InputActions[8], ETriggerEvent::Started, this, &ThisClass::Slot2_Button);

	EPI->BindAction(InputActions[9], ETriggerEvent::Started, this, &ThisClass::Slot3_Button);

	EPI->BindAction(InputActions[10], ETriggerEvent::Started, this, &ThisClass::TorchButton);

}



void ABaseChar::Attack_Button(const FInputActionValue& Value) { 
	//UE_LOG(LogTemp, Error, TEXT("Primo"));
	Attack_Server();
}
void ABaseChar::Attack_Server_Implementation() { Attack_Multicast(); }
bool ABaseChar::Attack_Server_Validate() { return true; }
void ABaseChar::Attack_Multicast_Implementation() { Attack_Execute(); }
bool ABaseChar::Attack_Multicast_Validate() { return true; }

void ABaseChar::Attack_Execute()
{
	//UE_LOG(LogTemp, Error, TEXT("Secondo"));

	if (!bCanFire)  return;

	auto wep{ EquippedWeapon };

	bool bRandAnim{ false };

	if (wep)
	{
		auto Type{ wep->GetWeaponType() };

		switch (Type)
		{
		case EWeaponType::EWT_Gun:
		case EWeaponType::EWT_Trap:
		case EWeaponType::EWT_Wire:
		case EWeaponType::EWT_Cans:
		case EWeaponType::EWT_Molotov:
		GetCharacterMovement()->MaxWalkSpeed = 0;
		break;

		case EWeaponType::EWT_Knife:
		case EWeaponType::EWT_Scissors:
		case EWeaponType::EWT_Syringe:
		GetCharacterMovement()->MaxWalkSpeed = 250;
		break;

		case EWeaponType::EWT_Club:
		GetCharacterMovement()->MaxWalkSpeed = 200;
		bRandAnim = true;
		break;

		case EWeaponType::EWT_Axe:
		case EWeaponType::EWT_Pipe:
		GetCharacterMovement()->MaxWalkSpeed = 150;
		bRandAnim = true;
		break;
		}

		if (wep->GetWepMontage())
		{
			//UE_LOG(LogTemp, Error, TEXT("Quarto"));
			bCanFire = false;

			auto MyAnim = GetMesh()->GetAnimInstance();

			//UE_LOG(LogTemp, Warning, TEXT("rate %f"), wep->GetFireRate());

			MyAnim->Montage_Play(wep->GetWepMontage(),wep->GetFireRate());
			if (bRandAnim)
			{
				switch (FMath::RandRange(1, 3))
				{
				case 1:	MyAnim->Montage_JumpToSection("Front"); break;
				case 2:	MyAnim->Montage_JumpToSection("Down"); break;
				case 3:	MyAnim->Montage_JumpToSection("Back"); break;
				}
			}
		}
	}

}

void ABaseChar::JumpIA(const FInputActionValue& Value)
{
	auto Val = Value.Get<bool>();

	if (!bCanJump) return;

	if (Val)
	{
		if (bIsCrouched)
		{
			UnCrouch();
		} 

		Jump();
	}
	else StopJumping();
}

void ABaseChar::Move(const FInputActionValue& Value)
{
	auto Val = Value.Get<FVector2D>();

	const FRotator YawRotation{ 0.f, Controller->GetControlRotation().Yaw, 0.f };
	const FVector Forward{ FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X) };
	const FVector Rotation{ FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y) };

	AddMovementInput(Forward, Val.Y);
	AddMovementInput(Rotation, Val.X);
}

void ABaseChar::Look(const FInputActionValue& Value)
{
	auto Val = Value.Get<FVector2D>();

	AddControllerYawInput(Val.X);
	AddControllerPitchInput(Val.Y);
}


void ABaseChar::CrouchBP(const FInputActionValue& Value)
{
	auto Val = Value.Get<bool>();

	if (!bIsCrouched) 
	{ 
		Crouch(); 
	}
	else
	{
		UnCrouch();
	}

}



void ABaseChar::Equip_Button(const FInputActionValue& Value)
{
	if (CombatComponent && TracedWeapon)
	{		
		//Gestisco il menù in locale senza utilizzare funzioni server
		CombatComponent->CollectWeapon(TracedWeapon);		
	}
}

// La funzione di tipo server verra scomposta in "sotto funzioni"
// _Implementation rappresenta l'implementazione vera e propria del codice
void ABaseChar::Equip_Server_Implementation(AWeapon* Traced)
{
	// questa funzione verrà eseguita su richiesta del client
	// // immediately equip the weapon
	//EquippedWeapon = Traced;
	CombatComponent->EquipWeapon(Traced);
	//CombatComponent->CollectWeapon(Traced); // put weapon in the backpack

}

// è possibile aggiungere un Validate
// questo serve per evitare cheat online

bool ABaseChar::Equip_Server_Validate(AWeapon* Traced)
{
	return true;
}
// se sono il server eseguo la funzione
 //se sono client chiedo al server di eseguirla
void ABaseChar::Drop_Button(const FInputActionValue& Value) 
{ 
	if (EquippedWeapon == nullptr || TracedWeapon || !bCanFire) return;

	Drop_Server(); 
	CombatComponent->RemoveFromBackPack(nullptr, CombatComponent->GetBackpackSlot());
	//CombatComponent->SetBackpackIsFull(false);
	//CombatComponent->BackPack[CombatComponent->GetBackpackSlot()] = nullptr;
}

// viene eseguito ovunque server e client
void ABaseChar::Drop_Server_Implementation() { Drop_Multicast(); }
// è possibile aggiungere un Validate
// questo serve per evitare cheat online
bool ABaseChar::Drop_Server_Validate() { return true;  }
// un metodo multicast viene lanciato su tutti i client ma non sul server
void ABaseChar::Drop_Multicast_Implementation() { Drop_Execute(); }
bool ABaseChar::Drop_Multicast_Validate() { return true; }

void ABaseChar::Drop_Execute()
{
	if (!bCanFire)  return;

	if (EquippedWeapon)
	{
		if (DropWeaponMontage)
		{
			bCanFire = false;

			auto MyAnim = GetMesh()->GetAnimInstance();
			MyAnim->Montage_Play(DropWeaponMontage);

			EquippedWeapon->SetOwner(nullptr);

			UE_LOG(LogTemp, Warning, TEXT(" Sto per animare %s"), *EquippedWeapon->GetName());

		}
	}
}

void ABaseChar::Slot1_Button(const FInputActionValue& Value)
{
	if (bCanFire == false || !CombatComponent->GetFromBackPack(0)) return;
	if (EquippedWeapon) CombatComponent->SetWeaponStateServer(EWeaponState::EWS_Backpack, EquippedWeapon);
	
	Equip_Server(CombatComponent->BackPack.Slot1);
	//CombatComponent->EquipWeapon(CombatComponent->BackPack.Slot1);
	CombatComponent->SetBackpackSlot(0);
}

void ABaseChar::Slot2_Button(const FInputActionValue& Value)
{
	if (bCanFire == false || !CombatComponent->GetFromBackPack(1)) return;
	if (EquippedWeapon) CombatComponent->SetWeaponStateServer(EWeaponState::EWS_Backpack, EquippedWeapon);
	
	Equip_Server(CombatComponent->BackPack.Slot2);
	//CombatComponent->EquipWeapon(CombatComponent->BackPack.Slot2);
	CombatComponent->SetBackpackSlot(1);
}

void ABaseChar::Slot3_Button(const FInputActionValue& Value)
{
	if (bCanFire == false || !CombatComponent->GetFromBackPack(2)) return;
	if (EquippedWeapon) CombatComponent->SetWeaponStateServer(EWeaponState::EWS_Backpack, EquippedWeapon);

	Equip_Server(CombatComponent->BackPack.Slot3);
	//CombatComponent->EquipWeapon(CombatComponent->BackPack.Slot3);
	CombatComponent->SetBackpackSlot(2);
}

void ABaseChar::TorchButton(const FInputActionValue& Value)
{
	// se ho la torcia posso usarla
	if(bHaveTorch) AttivaDisattivaTorcia();
	UE_LOG(LogTemp, Warning, TEXT("Tasto Torcia"));

}

void ABaseChar::ChooseWeapon(uint8 selected)
{
	

	//CombatComponent->BackPack[selected]


}

void ABaseChar::Throw()
{
	//UE_LOG(LogTemp, Error, TEXT("Ora Lancio"));
		// sono il client chiedo al server

	//CombatComponent->EquippedWeapon->SetWeaponState(EWeaponState::EWS_Dropped);
	//CombatComponent->EquippedWeapon->GetRootSphere()->AddImpulse(GetActorForwardVector() * 1000 * CombatComponent->EquippedWeapon->GetRootSphere()->GetMass());

	bCanFire = true;
	UE_LOG(LogTemp, Error, TEXT("Throw from notify "));
	if (IsLocallyControlled()) ServerThrow();
	
}

void ABaseChar::ServerThrow_Implementation()	// avviene per la copia del personaggio che sta sul server
{
	if (HasAuthority())
	{
		if (IsLocallyControlled()) { UE_LOG(LogTemp, Error, TEXT("Throw Server Local")); }
	    else                         UE_LOG(LogTemp, Error, TEXT("Throw Server remote")); 
	}
	else
	{
		if (IsLocallyControlled()) { UE_LOG(LogTemp, Error, TEXT("Throw client Local")); }
		else                         UE_LOG(LogTemp, Error, TEXT("Throw client remote"));

	}

	if (EquippedWeapon)
	{
		auto EW = EquippedWeapon;

		EW->SetWeaponState(EWeaponState::EWS_Dropped);

		if (EW->GetRootSphere()->IsSimulatingPhysics())
		{
			EW->GetRootSphere()->AddImpulse(GetActorForwardVector() * 1000 * EW->GetRootSphere()->GetMass());
		}
		
		EquippedWeapon = nullptr;
	}

	//GetCharacterMovement()->bOrientRotationToMovement = true;
	//bUseControllerRotationYaw = false;
}



bool ABaseChar::ServerThrow_Validate()
{

	return true;
}


// Called every frame
void ABaseChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*if (!HasAuthority())
	{
		if (CombatComponent->EquippedWeapon)  UE_LOG(LogTemp, Error, TEXT("Equipped"));
	}*/
	JumpCheck();

	if (IsLocallyControlled()) TraceForWeapon();

	auto Velocity{ GetVelocity() };
	Velocity.Z = 0;
	auto Speed{ Velocity.Size() };
	//auto bIsInAir{ GetCharacterMovement()->IsFalling() };

	if (Speed == 0.f && !bIsInAir) // stand in place and aiming
	{
		// prendo la rotazione della visuale (solo Yaw)
		FRotator CurrentAimRotation{ 0.f, GetBaseAimRotation().Yaw,0.f };
		FRotator ForwardRot{ GetActorForwardVector().Rotation() };
		ForwardRot.Pitch = 0;
		ForwardRot.Roll  = 0;

		FRotator DeltaAimRotation{UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRotation,StartingAimRotation) };
		bUseControllerRotationYaw = true;

		AO_Yaw = DeltaAimRotation.Yaw;


		if (TurnState == ETurnState::ETS_Stand)
		{
			AO_Yaw_Interp = AO_Yaw;
		}

		//GetBaseAimRotation()
		AO_Pitch = GetControlRotation().Pitch;
		if (AO_Pitch > 180) AO_Pitch -= 360;


		if (AO_Yaw > 90.f)
		{
			TurnState = ETurnState::ETS_Right;
		}
		else if (AO_Yaw < -90.f)
		{
			TurnState = ETurnState::ETS_Left;
		}

		if (TurnState != ETurnState::ETS_Stand)
		{
			AO_Yaw_Interp = FMath::FInterpTo(AO_Yaw_Interp, 0, DeltaTime, 5.f);

			AO_Yaw = AO_Yaw_Interp;


			if (FMath::Abs(AO_Yaw) < 10.f)
			{
				StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
				TurnState = ETurnState::ETS_Stand;
			}
		}

		/*
		FString Offset{ FString::Printf(TEXT(" Offset Yaw = %s"),*DeltaAimRotation.ToString()) };
		GEngine->AddOnScreenDebugMessage(1, -1, FColor::Cyan, Offset);*/

	}
	else // running or jumping 
	{   // se ci stiamo muovendo tutte le rotazioni sul posto vengono abortite e si torna 
		// alla situazione normale
		bUseControllerRotationYaw = true;
		AO_Yaw   = 0;
		AO_Pitch = 0;
		StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		TurnState = ETurnState::ETS_Stand; 
	}



}

void ABaseChar::JumpCheck()
{
	if (bIsInAir && !GetCharacterMovement()->IsFalling()) // quando la variabile attuale è vera e sta per essere aggiornata con un false
	{
		bCanJump = false; //non si potrà saltare per alcuni frame
		FTimerHandle TimerJump;
		GetWorldTimerManager().SetTimer(TimerJump, this, &ThisClass::ResetJump, 0.2f);
	}

	bIsInAir = GetCharacterMovement()->IsFalling();
}

/// <summary>
/// FUNZIONI PER IL TRACE E RACCOLTA ARMI/OGGETTI
/// </summary>

void ABaseChar::TraceForWeapon()
{
	FHitResult MyHit;
	bool hit = ItemTrace(MyHit);	// la funzione se trova un hit restituisce true e dentro la variabile passata per riferimento trovo i dati
	//UE_LOG(LogTemp, Error, TEXT("Ciao sono %s"), *GetName());
	if (hit)
	{
		
		//UE_LOG(LogTemp, Error, TEXT("Hit Generico %s"), *MyHit.GetComponent()->GetName());
		auto myact = Cast<ICollectInterface>(MyHit.GetActor());
		if (myact)
		{
			
			SetTracedWeapon(Cast<AWeapon>(MyHit.GetActor()));
		}
		else SetTracedWeapon(nullptr);

	}
	else SetTracedWeapon(nullptr);
}

bool ABaseChar::ItemTrace(FHitResult& OutHit)
{
	FVector2D ViewPortSize;
	//localizza il mirino e converti la sua posizione da bidimensionale a tridimensionale
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewPortSize);
	}

	// trovo il centro dello schermo
	FVector2D ScreenCenter = FVector2D(ViewPortSize.X * 0.5, ViewPortSize.Y * 0.5);

	FVector WorldPos, WorldDir; // vettori dove salvare i risultati
	APlayerController* MC = Cast<APlayerController>(GetController());

	if (UGameplayStatics::DeprojectScreenToWorld(MC, ScreenCenter, WorldPos, WorldDir))
	{
		auto EndPoint{ WorldPos + WorldDir * 500.f };
		FCollisionQueryParams Params;
		// metto il character tra gli oggetti ignorati per non interferire col trace
		Params.AddIgnoredActor(this);	
		if(GetWorld()->LineTraceSingleByChannel(OutHit,WorldPos,EndPoint, 
			ECC_Visibility,Params)) return true;
	}

	return false;
}

//richiamata dal trace per gli oggetti
void ABaseChar::SetTracedWeapon(AWeapon* NewWeapon)
{

	if ((!NewWeapon || NewWeapon != TracedWeapon) && TracedWeapon)
	{
		TracedWeapon->ShowWidget(false);
		TracedWeapon->GetMesh()->SetRenderCustomDepth(false);
	}

	TracedWeapon = NewWeapon;

	if (TracedWeapon)
	{
		TracedWeapon->ShowWidget(true); // il nuovo valore è
		TracedWeapon->GetMesh()->SetRenderCustomDepth(true);
	}

}


void ABaseChar::OnRep_Damage(uint8 PrevDamage)
{
	GetCombat()->Actual_Hp -= Damage;
	UE_LOG(LogTemp, Warning, TEXT("On Rep Damage"));
}

// on rep viene eseguito su tutti i client ma non sul server
void ABaseChar::OnRep_EquipWeapon(AWeapon* EW)
{
	//UE_LOG(LogTemp, Warning, TEXT("On rep Equip"));

	if (EquippedWeapon)
	{
		EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipment);
		//UE_LOG(LogTemp, Error, TEXT("Scambio arma"));
	}
	else
	{
		if (EW)
		{
			bCanFire = true;
			EW->SetWeaponState(EWeaponState::EWS_Dropped);

			if (EW->GetRootSphere()->IsSimulatingPhysics())
			{
				EW->GetRootSphere()->AddImpulse(GetActorForwardVector() * 1000 * EW->GetRootSphere()->GetMass());
			}
			else
			{
				//EW->GetRootSphere()->SetSimulatePhysics(true);
				//EW->GetRootSphere()->AddImpulse(Character->GetActorForwardVector() * 1000 * EW->GetRootSphere()->GetMass());
			}

		}
	}
}




UCombat* ABaseChar::GetCombat()
{
	return CombatComponent;
}




