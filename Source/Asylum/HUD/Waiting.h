// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Waiting.generated.h"


class UButton;

UCLASS()
class ASYLUM_API UWaiting : public UUserWidget
{
	GENERATED_BODY()


public:
	virtual bool Initialize() override;

    UPROPERTY(meta = (BindWidget))
	UButton* BackButton;

	UFUNCTION()
	void Back();

	UFUNCTION(BlueprintCallable)
		void MenuSetup();

	UFUNCTION(BlueprintCallable)
		void MenuEnd();

	bool PrevMenu{ false };

};
