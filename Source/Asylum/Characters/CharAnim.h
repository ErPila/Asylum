// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Asylum/Enum/TurnState.h"
#include "CharAnim.generated.h"

/**
 * 
 */
UCLASS()
class ASYLUM_API UCharAnim : public UAnimInstance
{
	GENERATED_BODY()


public:

	UCharAnim();

	virtual void NativeUpdateAnimation(float DeltaTime) override;

	virtual void NativeInitializeAnimation() override;
	
private:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"));
	class ABaseChar* Char;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"));
	float Speed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"));
	bool bIsInAir;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"));
	bool bIsAccelerating;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"));
	bool bIsCrouched;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"));
	bool bIsEquipped;

	// differenza tra direzione di movimento e direzione dello sguardo
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"));
	float YawOffet;
	
	// posizione e rotazione del socket sull'arma
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"));
	FTransform LeftHandTransform;

	// arma attualmente in uso
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"));
	class AWeapon* CurrentWeapon;

	// differenza tra direzione di movimento e direzione dello sguardo
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"));
	float AO_Yaw;

	// differenza tra direzione di movimento e direzione dello sguardo
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"));
	float AO_Pitch;

	// differenza tra direzione di movimento e direzione dello sguardo
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"));
	ETurnState TurnState;

	//qui memorizzo l'attuale rotazione del chatacter (sullo yaw)
	FRotator ActualCharRotation;

	// qui resta memorizzato la rotazione del frame precedente 
	FRotator LastCharRotation;

	// differenza tra direzione di movimento e direzione dello sguardo
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"));
	float Lean;


	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"));
	bool bFullBody;


};
