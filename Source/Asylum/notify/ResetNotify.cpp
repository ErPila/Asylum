// Fill out your copyright notice in the Description page of Project Settings.


#include "ResetNotify.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Asylum/Components/Combat.h"
#include "Asylum/Characters/BaseChar.h"

void UResetNotify::ResetValues(AActor* Fool)
{
	if (!Fool) return;

	auto F = Cast<ABaseChar>(Fool);

	if (!F) return;

	F->GetCharacterMovement()->MaxWalkSpeed = F->GetCombat()->GetStandWalkSpeed();
	F->GetCharacterMovement()->MaxWalkSpeedCrouched = F->GetCombat()->GetCrouchWalkSpeed();

	F->bCanFire = true;

}
