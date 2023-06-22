// Fill out your copyright notice in the Description page of Project Settings.


#include "CharAnim.h"
#include "BaseChar.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Asylum/Weapons/Weapon.h"
#include "Asylum/Enum/WeaponType.h"


UCharAnim::UCharAnim():
	Char(nullptr),
	Speed(0),
	bIsAccelerating(false),
	bIsInAir(false),
	bIsCrouched(false),
	ActualCharRotation(FRotator(0.f)),
	LastCharRotation(FRotator(0.f)),
	Lean(0.f)
{

}

void UCharAnim::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	if (!Char) return;

	//	Definisco la speed in base alla velocita (escludo la Z per evitare l'animazione della camminata durante la caduta)
	auto Velocity{ Char->GetVelocity() };
	Velocity.Z = 0;

	Speed = Velocity.Size();

	bIsInAir = Char->GetCharacterMovement()->IsFalling();
	bIsAccelerating = Char->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f;
	bIsCrouched = Char->GetIsCrouched();
	bIsEquipped = Char->IsEquipped();

	// diamo un valore all'arma corrente
	CurrentWeapon = Char->GetEquippedWeapon();

	if (CurrentWeapon)
	{
		bFullBody = (CurrentWeapon->GetWeaponType() == EWeaponType::EWT_Gun);
	}
	else bFullBody = true;


	auto AimRot  = Char->GetBaseAimRotation();	// angolo dove sta guardando
	auto MoveRot = UKismetMathLibrary::MakeRotFromX(Char->GetVelocity()); // angolo dove sta muovendo

	FRotator DR{ UKismetMathLibrary::NormalizedDeltaRotator(MoveRot, AimRot) };
	YawOffet = DR.Yaw;


	//FString Offset{ FString::Printf(TEXT(" Offset Yaw = %f"),YawOffet) };
	//GEngine->AddOnScreenDebugMessage(1, -1, FColor::Cyan, Offset);

	//Gestione del socket per il braccio sinistro

	AO_Yaw    = Char->GetAo_Yaw();
	AO_Pitch  = Char->GetAo_Pitch();
	TurnState = Char->GetTurnState();




	// se sono equipaggiato con un arma
	if (bIsEquipped && CurrentWeapon)
	{
		auto mesh = CurrentWeapon->GetMesh();
		LeftHandTransform = mesh->GetSocketTransform(FName("GripL"), RTS_World);
		FVector OutPosition;
		FRotator OutRotation;

		// per attivare il nodo Fabrik IK ho bisogno di un target (socket) e ti un bone di riferimento per il movimento, 
		// in questo caso la mano opposta che sta tenendo il fucile/pistola.
		// il nodo ha bisogno delle coordinate in formato BoneSpace.		

		Char->GetMesh()->TransformToBoneSpace(FName("hand_r"), LeftHandTransform.GetLocation(), FRotator(0), OutPosition, OutRotation);

		// qui adesso LeftHandTransform rilevato precedentemente rispetto al mondo è stato convertito
		// in un offset rispetto alla posizione della mano destra
		LeftHandTransform.SetLocation(OutPosition);
		LeftHandTransform.SetRotation(OutRotation.Quaternion());
	}

	LastCharRotation = ActualCharRotation;
	ActualCharRotation = Char->GetActorRotation();

	FRotator DeltaRot{ UKismetMathLibrary::NormalizedDeltaRotator(ActualCharRotation,LastCharRotation) };
	auto  Target{ DeltaRot.Yaw / DeltaTime };
	float Interp{ float ( FMath::FInterpTo(Lean, Target, DeltaTime, 4.f) ) };
	Lean = FMath::Clamp(Interp, -90.f, 90.f);


}

void UCharAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Char = Cast<ABaseChar>(TryGetPawnOwner());
}
