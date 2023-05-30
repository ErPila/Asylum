// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStats.generated.h"


class ABaseChar;

UCLASS()
class ASYLUM_API UPlayerStats : public UUserWidget
{
	GENERATED_BODY()
	

public:

	UPROPERTY(EditAnyWhere, Category = "Bind")
	ABaseChar* MadMan{ nullptr };





};
