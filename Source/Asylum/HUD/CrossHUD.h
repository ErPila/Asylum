// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CrossHUD.generated.h"

/**
 * 
 */
UCLASS()
class ASYLUM_API ACrossHUD : public AHUD
{
	GENERATED_BODY()
	
protected:

    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "HUD")
        UTexture2D* CrosshairMiddle;

    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "HUD")
        UTexture2D* CrosshairLeft;

    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "HUD")
        UTexture2D* CrosshairRight;

    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "HUD")
        UTexture2D* CrosshairTop;

    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "HUD")
        UTexture2D* CrosshairBottom;

    UPROPERTY(BlueprintReadOnly, Category = "HUD")
        FVector2D ScreenCenter;

    UPROPERTY(EditAnywhere, Category = "HUD")
        float Offset{ 32.f };

    UPROPERTY(EditAnywhere, Category = "HUD")
        float MaxSpread{ 32.f };

    virtual void DrawHUD() override;



};
