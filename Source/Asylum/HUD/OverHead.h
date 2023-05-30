// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OverHead.generated.h"

/**
 * 
 */
UCLASS()
class ASYLUM_API UOverHead : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void SetDisplayText(FString ToDisplay);

	UFUNCTION(BlueprintCallable)
	void ShowPlayNetRole(APawn* PN, bool isLocalRole);

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DisplayText;



protected:

	virtual void NativeDestruct() override;


};
