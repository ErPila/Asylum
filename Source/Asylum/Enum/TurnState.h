#pragma once

UENUM(BlueprintType)
enum class ETurnState : uint8
{
	ETS_Left   UMETA(DisplayName   = "Turn Left"),
	ETS_Right  UMETA(DisplayName   = "Turn Right"),
	ETS_Stand  UMETA(DisplayName   = "Stand"),
	ETS_MAX    UMETA(DisplayName   = "DefualtMAX")

};