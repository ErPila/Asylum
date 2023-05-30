// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Asylum/Enum/WeaponType.h"
#include "Combat.generated.h"

class AWeapon;
//class UCollectInterface;

USTRUCT(BlueprintType)
struct FBackpack
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnyWhere)
	AWeapon* Slot1 { nullptr };
	UPROPERTY(VisibleAnyWhere)
	AWeapon* Slot2 { nullptr };
	UPROPERTY(VisibleAnyWhere)
	AWeapon* Slot3 { nullptr };
};




UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ASYLUM_API UCombat : public UActorComponent
{
	GENERATED_BODY()

    
	friend class ABaseChar; // in questo modo  il character può accedere
	// a variabili private del componente

	UPROPERTY(EditDefaultsOnly)
	float Max_Hp;

	float Actual_Hp;

	UPROPERTY(EditDefaultsOnly)
	float Max_Sanity;

	float Actual_Sanity;

	UPROPERTY(EditDefaultsOnly)
	float StandWalkSpeed{  350.f };

	UPROPERTY(EditDefaultsOnly)
	float CrouchWalkSpeed{ 150.f };

	int8 BackpackSlot{ 0 };

	bool bIsFull{ false };


	ABaseChar* MyChar{ nullptr };
public:	

	FORCEINLINE float GetStandWalkSpeed() { return StandWalkSpeed; };
	FORCEINLINE float GetCrouchWalkSpeed() { return CrouchWalkSpeed; };

	UFUNCTION(BlueprintPure)
	AWeapon* GetFromBackPack(int32 index);

	UFUNCTION(BlueprintCallable)
	void RemoveFromBackPack(AWeapon* ToRemove, int32 index);

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetHpPercent() { return Actual_Hp / Max_Hp; };
	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetSanityPercent() { return Actual_Sanity / Max_Sanity; };

	// Sets default values for this component's properties
	UCombat();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(BlueprintReadWrite)
	uint8 ActualWeapon{ 0 };
	
	void EquipWeapon(AWeapon* WeaponToEquip);

	bool CollectWeapon(AWeapon* WeaponToCollect);

	void CheckBackpackFull();


	FORCEINLINE int8 GetBackpackSlot() { return BackpackSlot; };
	
	FORCEINLINE void SetBackpackSlot(int8 NewVal) { BackpackSlot = NewVal; };

	FORCEINLINE bool BackpackIsFull() { return bIsFull; };

	FORCEINLINE void SetBackpackIsFull(bool NewVal) { bIsFull = NewVal; };

	UFUNCTION(Server, Reliable)
	void SetWeaponStateServer(EWeaponState NewState,AWeapon* ActualW);


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	ABaseChar* Character;

	UPROPERTY(VisibleAnywhere)
	FBackpack BackPack; // weapons inside the backpack


	





		
};
