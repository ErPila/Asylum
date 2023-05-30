// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "Asylum/Enum/TurnState.h"
#include "BaseChar.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UWidgetComponent;
class AWeapon;

UCLASS()
class ASYLUM_API ABaseChar : public ACharacter
{
	GENERATED_BODY()


	friend class UCombat;

	UPROPERTY(EditAnywhere)
	UInputMappingContext* MC_Combat;

	UPROPERTY(EditAnywhere)
	TArray<UInputAction*> InputActions;

	void AddContext(UInputMappingContext* MC, int32 priority);

	UPROPERTY(VisibleAnywhere, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* ArmComponent;

	UPROPERTY(VisibleAnywhere, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* DebugWidget{ nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combact", meta = (AllowPrivateAccess = "true"))
	UCombat* CombatComponent{ nullptr };

	// variabile replicata legata ad una funzione
	// ogni volta che tale variabile cambia di valore la funzione relativa viene eseguita
	UPROPERTY(ReplicatedUsing = OnRep_ChangeMesh)
	uint8 SelectedMesh { 0 };

	UFUNCTION()
	void OnRep_ChangeMesh();

	//UPROPERTY(ReplicatedUsing = OnRep_TracedWeapon)
	AWeapon* TracedWeapon;

	//UFUNCTION()
	//void OnRep_TracedWeapon(AWeapon* PrevTracedWeapon);


	// l'argomento contiene il valore precedente della variabile modificata
	//UFUNCTION()
	//void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

	UPROPERTY(EditAnyWhere, Category = "MyMadman")
	TArray<USkeletalMesh*> CharMeshes;

	//UFUNCTION(NetMulticast, Reliable, WithValidation)
	//void MulticastThrow();

	bool bIsInAir;

	bool bCanJump;	// finchè è vera posso saltare

	void ResetJump() { bCanJump = true; }	// resetto a vera can jump

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* DropWeaponMontage;

	// Yaw e Pitch da passare all'anim instance
	float AO_Yaw;
	float AO_Pitch;

	float AO_Yaw_Interp;

	//rotazione iniziale 
	FRotator StartingAimRotation;
	// lo stato stand prevede l'utilizzo dell' aim offset gli altri dell'animazione di turn
	ETurnState TurnState{ ETurnState::ETS_Stand };

public:

	UFUNCTION(BlueprintPure)
	UCombat* GetCombat();

	FORCEINLINE ETurnState GetTurnState() { return TurnState; };
	FORCEINLINE float GetAo_Yaw()   { return AO_Yaw; };
	FORCEINLINE float GetAo_Pitch() { return AO_Pitch; };

//	UFUNCTION(Server, Reliable)
//	void ServerSetWeapon(AWeapon* NewWeapon);

	void SetTracedWeapon(AWeapon* NewWeapon);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	bool GetIsCrouched(){ return bIsCrouched; };

	AWeapon* GetEquippedWeapon();

	// Sets default values for this character's properties
	ABaseChar();

	UPROPERTY(BlueprintReadOnly)
	bool LocallyControlled;

	bool IsEquipped();

	bool bCanFire{ true };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable)  // we use this function in order to setup player mesh on the server
	void Begin_Server(uint8 Selected);


	void Equip_Button(const FInputActionValue& Value);
	UFUNCTION(Server, Reliable, WithValidation)
	void Equip_Server(AWeapon* Traced);



	void Attack_Button(const FInputActionValue& Value);
	UFUNCTION(Server, Reliable, WithValidation)
	void Attack_Server();
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Attack_Multicast();
	void Attack_Execute();


	// server = il clien la chiama e viene eseguita sul server
  // multicast = il server la chiama e viene eseguita su tutti i client
	void Drop_Button(const FInputActionValue& Value);
	UFUNCTION(Server, Reliable, WithValidation)
	void Drop_Server();
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Drop_Multicast();
	void Drop_Execute();

	void Slot1_Button(const FInputActionValue& Value);

	void Slot2_Button(const FInputActionValue& Value);

	void Slot3_Button(const FInputActionValue& Value);


	void ChooseWeapon(uint8 selected);

	//UFUNCTION(BlueprintCallable)
	

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerThrow();



	void CrouchBP(const FInputActionValue& Value);

	void JumpIA(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void Move(const FInputActionValue& Value);
	
	bool ItemTrace(FHitResult& OutHit);

public:	
	// chaimo Throw direttamente dalla notifica C++
	void Throw();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void JumpCheck();

	void TraceForWeapon();

	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
