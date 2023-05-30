#pragma once

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_Gun      UMETA(DisplayName = "Gun"),
	EWT_Club     UMETA(DisplayName = "Club"),
	EWT_Axe      UMETA(DisplayName = "Axe"),
	EWT_Pipe     UMETA(DisplayName = "Pipe"),
	EWT_Knife    UMETA(DisplayName = "Knife"),
	EWT_Scissors UMETA(DisplayName = "Scissor"),
	EWT_Syringe  UMETA(DisplayName = "Syringe"),
	EWT_Trap     UMETA(DisplayName = "Trap"),
	EWT_Wire     UMETA(DisplayName = "Wire"),
	EWT_Cans     UMETA(DisplayName = "Cans"),
	EWT_Molotov  UMETA(DisplayName = "Molotov"),
	EWT_MAX      UMETA(DisplayName = "DefualtMAX")
};
