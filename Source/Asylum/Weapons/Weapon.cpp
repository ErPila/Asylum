// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Asylum/Components/Combat.h"
#include "Net/UnrealNetwork.h"
#include "Asylum/Characters/BaseChar.h"
#include "NiagaraFunctionLibrary.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"



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
	WeaponType = EWeaponType::EWT_Axe;
	break;

	case 3:
	WeaponType = EWeaponType::EWT_Pipe;
	break;

	case 4:
	WeaponType = EWeaponType::EWT_Scissors;
	break;

	case 5:
	WeaponType = EWeaponType::EWT_Knife;
	break;

	case 6:
		WeaponType = EWeaponType::EWT_Molotov;
	break;

	case 7:
		WeaponType = EWeaponType::EWT_Syringe;
	break;
	

	}

	SetWeaponData();
}

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
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
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bCanAttack)
	{
		if(HasAuthority())
		{
			ExecuteAttack();
		}
	}
}

void AWeapon::ExecuteAttack()
{
	FHitResult Hit;
	FTransform StartTrace = WeaponMesh->GetSocketTransform(FName("DamageStartSocket"), RTS_World);
	FTransform EndTrace = WeaponMesh->GetSocketTransform(FName("DamageEndSocket"), RTS_World);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	bool Found = false;

	switch (WeaponType)
	{
	case EWeaponType::EWT_Gun:
		
		Found = GetWorld()->SweepSingleByChannel(Hit, StartTrace.GetLocation(), EndTrace.GetLocation(), FQuat{ 0 }, ECC_Visibility, FCollisionShape::MakeSphere(20.f), Params);
		

		break;
	case EWeaponType::EWT_Club:
	case EWeaponType::EWT_Axe:
	case EWeaponType::EWT_Pipe:
	case EWeaponType::EWT_Knife:
	case EWeaponType::EWT_Scissors:
	case EWeaponType::EWT_Syringe:

		GetWorld()->LineTraceSingleByChannel(Hit, StartTrace.GetLocation(), EndTrace.GetLocation(), ECC_Visibility, Params);

		break;
	case EWeaponType::EWT_Trap:
		break;
	case EWeaponType::EWT_Wire:
		break;
	case EWeaponType::EWT_Cans:
		break;
	case EWeaponType::EWT_Molotov:

		TArray<FHitResult> OggettiColpiti;

		Found = GetWorld()->SweepMultiByChannel(OggettiColpiti, GetActorLocation(), GetActorLocation() + FVector(0, 0, 1), FQuat{ 0 }, ECC_Visibility, FCollisionShape::MakeSphere(100.f), Params);
		DrawDebugSphere(GetWorld(), GetActorLocation(), 100.f, 8, FColor::Blue, true, 5.f);

		for (int i = 0; i < OggettiColpiti.Num(); i++)
		{
			if (Cast<ABaseChar>(OggettiColpiti[i].GetActor()))
			{
				Hit = OggettiColpiti[i];
				break;
			}
		}
		break;

	}


	auto Player = Cast<ABaseChar>(Hit.GetActor());
	if (Player)
	{
		// se colpisco un character imposto il bcanattack a false 
		// cos� interrompo il trace e applico una volta il danno
		bCanAttack = false;

		DrawDebugSphere(GetWorld(), Hit.Location, 5.f, 8, FColor::Green, false, 5.f);

		// se ho inserito il sistema particellari lo spawno sul punto di impatto
		//SpawnSoundParticle(Hit.Location, HitParticle, HitSound);

		// damage del player on rep, ad ogni cambio applica il danno e lo replica sui client
		Player->Damage = Damage;

		// funzione per applicare il danno se siamo il server
		Player->GetCombat()->ReceiveDamage(Damage);
	}
	//DrawDebugLine(GetWorld(), StartTrace.GetLocation(), EndTrace.GetLocation(), (Player ? FColor::Green : FColor::Red), false, 5.f);
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
			WeaponMesh->SetRelativeScale3D(TypeRow->DTScaleOffset);

			CollisionSphere->SetRelativeScale3D(FVector(1) / TypeRow->DTScaleOffset);

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
	
	//CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnSphereBeginOverlap);

	RootSphere->OnComponentHit.AddDynamic(this, &AWeapon::OnHit);

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


		//if (!HasAuthority()) UE_LOG(LogTemp, Warning, TEXT("Back to initial state on the client"));

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

		//if (!HasAuthority()) UE_LOG(LogTemp, Warning, TEXT("cambio client"))
		//else UE_LOG(LogTemp, Warning, TEXT("cambio server"));

	break;

	case EWeaponState::EWS_Dropped:
	{
		
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


		//UE_LOG(LogTemp, Error, TEXT("Ri Attivo la fisica "));

		if(HasAuthority()) GetWorldTimerManager().SetTimer(Time, this, &AWeapon::DropTimer, 3.f);

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

void AWeapon::SpawnSoundParticle_Implementation(FVector Position, UNiagaraSystem* Particle, USoundCue* Sound)
{

	if (Sound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, Position);
	}

	if (Particle)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Particle, Position);
	}
}

void AWeapon::OnRep_WeaponState(EWeaponState OldState)
{
	//UE_LOG(LogTemp, Warning, TEXT("Weapon set on replicate"));
	SetWeaponState(WeaponState);

}


void AWeapon::DropTimer()
{
	switch (WeaponType)
	{
	case EWeaponType::EWT_Gun:
	case EWeaponType::EWT_Club:
	case EWeaponType::EWT_Axe:
	case EWeaponType::EWT_Pipe:
	case EWeaponType::EWT_Knife:
	case EWeaponType::EWT_Scissors:
	case EWeaponType::EWT_Syringe:
		SetWeaponState(EWeaponState::EWS_Initial);
		break;
	case EWeaponType::EWT_Trap:
		break;
	case EWeaponType::EWT_Wire:
		break;
	case EWeaponType::EWT_Cans:
		break;
	case EWeaponType::EWT_Molotov:
		if (Explode)
		{
			ExecuteAttack();
			Destroy();
		}
		else SetWeaponState(EWeaponState::EWS_Initial);
		

		break;
	}
	
}

void AWeapon::ShowWidget(bool Visibility)
{
	if (PickupWidget)
	{
		Visibility ? PickupWidget->GetUserWidgetObject()->SetVisibility(ESlateVisibility::Visible) : PickupWidget->GetUserWidgetObject()->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AWeapon::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	if (WeaponState == EWeaponState::EWS_Dropped)
	{
		if (OtherComp->ComponentHasTag("Floor"))
		{
			DropTimer();
			GetWorldTimerManager().ClearTimer(Time);
		}
	}
	
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}


