// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Select.generated.h"

class UButton;

UCLASS()
class ASYLUM_API USelect : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	UButton* Ange;

	UPROPERTY(meta = (BindWidget))
	UButton* Chri;

	UPROPERTY(meta = (BindWidget))
	UButton* Luca;

	UPROPERTY( meta = (BindWidget))
	UButton* Start;

	UPROPERTY(meta = (BindWidget))
	UButton* Test;

	UFUNCTION(BlueprintCallable)
	void MenuSetup();


	UFUNCTION()
	void SetAnge();
	UFUNCTION()
	void SetChri();
	UFUNCTION()
	void SetLuca();

	UFUNCTION()
	void StartPressed();


	UFUNCTION()
	void TestPressed();

public :

	UPROPERTY(BlueprintReadWrite, Category = "Player")
	uint8 Selected;

	virtual bool Initialize() override;

	AActor* Angel{ nullptr };
	AActor* Chris{ nullptr };
	AActor* Lucas{ nullptr };
	AActor* Luce{ nullptr };

};
