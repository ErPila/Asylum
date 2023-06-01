// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "Asylum/Characters/BaseChar.h"
#include "NiagaraFunctionLibrary.h"



void AWeapon::SetWeaponType(uint8 RandMax)
{

	auto rnd{ FMath::RandRange(0, RandMax) };

	switch (rnd)
	{
	case 0:
	WeaponType = EWeaponType::EWT_Gun;
	break;

	case 1:
	WeaponType = EWeaponType::EWT_Club;
	break;

	case 2:
	WeaponType = EWeaponType::EWT_Gun;
	break;


	}

	SetWeaponData();
}

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;	// setta che questa classe deve essere replicata

	RootSphere = CreateDefaultSubobject<USphereComponent>(TEXT("RootSphere"));
	SetRootComponent(RootSphere);

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootSphere);

	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);	// tutti i canali in block
	//WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);	// ignora il pawn
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);	// collisione inizialmente disattivata

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetupAttachment(WeaponMesh);
	
	CollisionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);	// tutti i canali in block
	//CollisionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);	// ignora il pawn

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidget->SetupAttachment(RootComponent);

}

/*
void AWeapon::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	//SetWeaponData();
}*/

void AWeapon::SetWeaponData()
{

	FString TablePath(TEXT("DataTable'/Game/_Assets/Structures/DataTables/DT_WeaponTypes.DT_WeaponTypes'"));

	UDataTable* WeaponDT = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *TablePath));

	if (WeaponDT)
	{
		//UE_LOG(LogTemp, Error, TEXT("Data Table Trovata"));

		FWeaponTable* TypeRow{ nullptr };

		switch (WeaponType)
		{
		case EWeaponType::EWT_Gun:
			TypeRow = WeaponDT->FindRow<FWeaponTable>(FName("Pistol"), TEXT(""));
			break;
		case EWeaponType::EWT_Club:
			TypeRow = WeaponDT->FindRow<FWeaponTable>(FName("Club"), TEXT(""));
			break;
		case EWeaponType::EWT_Axe:
			TypeRow = WeaponDT->FindRow<FWeaponTable>(FName("Axe"), TEXT(""));
			break;
		case EWeaponType::EWT_Pipe:
			TypeRow = WeaponDT->FindRow<FWeaponTable>(FName("Pipe"), TEXT(""));
			break;
		case EWeaponType::EWT_Knife:
			TypeRow = WeaponDT->FindRow<FWeaponTable>(FName("Knife"), TEXT(""));
			break;
		case EWeaponType::EWT_Scissors:
			TypeRow = WeaponDT->FindRow<FWeaponTable>(FName("Scissors"), TEXT(""));
			break;
		case EWeaponType::EWT_Syringe:
			TypeRow = WeaponDT->FindRow<FWeaponTable>(FName("Syringe"), TEXT(""));
			break;
		case EWeaponType::EWT_Trap:
			TypeRow = WeaponDT->FindRow<FWeaponTable>(FName("Trap"), TEXT(""));
			break;
		case EWeaponType::EWT_Wire:
			TypeRow = WeaponDT->FindRow<FWeaponTable>(FName("Wire"), TEXT(""));
			break;
		case EWeaponType::EWT_Cans:
			TypeRow = WeaponDT->FindRow<FWeaponTable>(FName("Cans"), TEXT(""));
			break;
		case EWeaponType::EWT_Molotov:
			TypeRow = WeaponDT->FindRow<FWeaponTable>(FName("Molotov"), TEXT(""));
			break;

		}

		if (TypeRow)
		{
			Icon = TypeRow->DTIcon;
			PickupSound = TypeRow->DTPickupSound;
			UseSound = TypeRow->DTUseSound;
			HitSound = TypeRow->DTHitSound;
			Damage = TypeRow->DTDamage;
			FireRate = TypeRow->DTFireRate;
			Ammo = TypeRow->DTAmmo;
			Mesh = TypeRow->DTMesh;
			FireParticle = TypeRow->DTFireParticle;
			HitParticle = TypeRow->DTHitParticle;


			//UE_LOG(LogTemp, Error, TEXT("rate %f"), FireRate);

			//WeaponMesh->SetRenderCustomDepth(false);

			WeaponMesh->SetSkeletalMesh(Mesh);
			WeaponMesh->SetRelativeLocation(TypeRow->DTLocationOffset);
			WeaponMesh->SetRelativeRotation(TypeRow->DTRotationOffset);

			WeaponMesh->SetCustomDepthStencilValue(TypeRow->DTCustomStencil);

			WepMontage = TypeRow->DTMontage;
		}

	}
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, WeaponType); // replica sempre la variabile
	DOREPLIFETIME(ThisClass, WeaponState);
}



// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	SetWeaponData();
	ShowWidget(false);

	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);	// tutti i canali in block
	CollisionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	CollisionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}


void AWeapon::SetWeaponState(EWeaponState NewState)
{
	
	//if (WeaponState == NewState) return;

	WeaponState = NewState;
	

	switch (WeaponState)
	{
	case EWeaponState::EWS_Initial:


		if (!HasAuthority()) UE_LOG(LogTemp, Warning, TEXT("Back to initial state on the client"));

		ShowWidget(false);
		CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

		WeaponMesh->SetSimulatePhysics(false);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		RootSphere->SetSimulatePhysics(false);
		RootSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		RootSphere->BodyInstance.bLockXRotation = false;
		RootSphere->BodyInstance.bLockYRotation = false;
		RootSphere->BodyInstance.bLockZRotation = false;

		break;

	case EWeaponState::EWS_Equipment:

		ShowWidget(false);
		CollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		WeaponMesh->SetVisibility(true);
		RootSphere->SetSimulatePhysics(false);

		if (!HasAuthority()) UE_LOG(LogTemp, Warning, TEXT("cambio client"))
		else UE_LOG(LogTemp, Warning, TEXT("cambio server"));

	break;

	case EWeaponState::EWS_Dropped:
	{
		FTimerHandle Time;
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		SetActorRotation(FRotator(0, GetActorRotation().Yaw, 0));

		RootSphere->BodyInstance.bLockXRotation = true;
		RootSphere->BodyInstance.bLockYRotation = true;
		RootSphere->BodyInstance.bLockZRotation = true;

		RootSphere->SetSimulatePhysics(true);
		RootSphere->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		RootSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);	// tutti i canali in block
		RootSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);	// ignora il pawn
		RootSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

		UE_LOG(LogTemp, Error, TEXT("Ri Attivo la fisica "));

		if(HasAuthority()) GetWorldTimerManager().SetTimer(Time, this, &AWeapon::DropTimer, 2.5f);

		//RootSphere->AddImpulse(Character->GetActorForwardVector() * 1000 * EW->GetRootSphere()->GetMass());
	}
	break;


	case EWeaponState::EWS_Interpolation:

		break;

	case EWeaponState::EWS_Backpack:

		ShowWidget(false);

		CollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		WeaponMesh->SetSimulatePhysics(false);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		WeaponMesh->SetVisibility(false);

		RootSphere->SetSimulatePhysics(false);
		RootSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		break;


	}
}

/*
void AWeapon::SetWeaponStateServer_Implementation(EWeaponState NewState)
{
	SetWeaponState(NewState);
	UE_LOG(LogTemp, Warning, TEXT("Weapon set on server")); 
}
*/



void AWeapon::OnRep_WeaponState(EWeaponState OldState)
{
	UE_LOG(LogTemp, Warning, TEXT("Weapon set on replicate"));
	SetWeaponState(WeaponState);

}


void AWeapon::DropTimer()
{
	SetWeaponState(EWeaponState::EWS_Initial);
}

void AWeapon::ShowWidget(bool Visibility)
{
	if (PickupWidget)
	{
		Visibility ? PickupWidget->GetUserWidgetObject()->SetVisibility(ESlateVisibility::Visible) : PickupWidget->GetUserWidgetObject()->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AWeapon::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}


