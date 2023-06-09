// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CollectInterface.h"
#include "Asylum/Enum/WeaponType.h"
#include "Engine/DataTable.h"
#include "Sound/SoundCue.h"
#include "Weapon.generated.h"


USTRUCT(BlueprintType)
struct FWeaponTable : public FTableRowBase
{
	GENERATED_BODY()

		UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		UTexture2D* DTIcon { nullptr };

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	USoundCue*  DTPickupSound {
		nullptr
	};

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	USoundCue*  DTUseSound {
		nullptr
	};

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	USoundCue*  DTHitSound {
		nullptr
	};

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	uint8 DTDamage{ 0 };

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
    float DTFireRate{ 0.f };

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
    int32 DTAmmo {
		0
	};

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	USkeletalMesh* DTMesh {
		nullptr
	};

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	class UNiagaraSystem* DTFireParticle{ nullptr };

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	UNiagaraSystem* DTHitParticle {
		nullptr
	};

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	FVector DTLocationOffset {
		FVector(0)
	};

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	FRotator DTRotationOffset {
		FRotator(0)
	};

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FVector DTScaleOffset {
		FVector(1)
	};

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 DTCustomStencil {
		0
	};

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	UAnimMontage* DTMontage {
		nullptr
	};
};




UCLASS()
class ASYLUM_API AWeapon : public AActor , public ICollectInterface
{
	GENERATED_BODY()

private:

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	class USphereComponent* RootSphere{ nullptr };

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	USkeletalMeshComponent* WeaponMesh { nullptr };

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	USphereComponent* CollisionSphere { nullptr };
	
	UPROPERTY(EditAnywhere, Category = "Weapon")
	class UWidgetComponent* PickupWidget { nullptr };

	UPROPERTY(ReplicatedUsing = OnRep_WeaponState, VisibleAnywhere, Category = "Weapon")
	EWeaponState WeaponState{ EWeaponState::EWS_Initial };

	UPROPERTY(EditAnywhere, Category = "Weapon",Replicated)
	EWeaponType WeaponType{ EWeaponType::EWT_Gun };

	// Lista proprietà arma modificabili tramite data table
	
	USoundCue* PickupSound;
	USoundCue* UseSound;
	USoundCue* HitSound;
	uint8 Damage;
	float FireRate{ 0.f };
	int32 Ammo;
	USkeletalMesh*  Mesh;
	UNiagaraSystem* FireParticle;
	UNiagaraSystem* HitParticle;

	UPROPERTY(VisibleAnyWhere,Category = "Montage")
	UAnimMontage* WepMontage;


	bool bCanAttack{ false };

	UFUNCTION(NetMulticast, Unreliable)
	void SpawnSoundParticle(FVector Position, UNiagaraSystem* Particle = nullptr, USoundCue* Sound = nullptr);

public:	

	UFUNCTION()
	void OnRep_WeaponState(EWeaponState OldState);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetCanAttack(bool NewVal) { bCanAttack = NewVal; };

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	UTexture2D* Icon;

	
	UFUNCTION(BlueprintCallable)
	void SetWeaponType(uint8 RandMax);

	UFUNCTION(BlueprintPure)
	EWeaponState GetWeaponState() { return WeaponState; };

	UAnimMontage* GetWepMontage() { return WepMontage; }

	// Sets default values for this actor's properties
	AWeapon();

	void Tick(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE EWeaponType GetWeaponType() { return WeaponType; };

	float GetFireRate()   { return FireRate; };


	void SetWeaponState(EWeaponState NewState);

	//UFUNCTION(Server,Reliable)
	//void SetWeaponStateServer(EWeaponState NewState);


	void DropTimer();

	FORCEINLINE USphereComponent* GetCollisionSphere() { return CollisionSphere; };

	FORCEINLINE USkeletalMeshComponent* GetMesh() { return WeaponMesh; };

	FORCEINLINE USphereComponent* GetRootSphere() { return RootSphere; };
	
	UFUNCTION(BlueprintCallable)
	void ShowWidget(bool Visibility);

	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	// Called every frame

	virtual void BeginPlay() override;


	//virtual void OnConstruction(const FTransform& Transform) override;

	void SetWeaponData();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintPure)
	UWidgetComponent* GetWidgetPickUp() { return PickupWidget; };
};
