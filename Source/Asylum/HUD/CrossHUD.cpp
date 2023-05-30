// Fill out your copyright notice in the Description page of Project Settings.


#include "CrossHUD.h"
#include "Asylum/Characters/BaseChar.h"
#include "Engine/Canvas.h"

void ACrossHUD::DrawHUD()
{
	Super::DrawHUD();

	ScreenCenter = FVector2D(float(Canvas->SizeX >> 1)-32, float(Canvas->SizeY >> 1)-32);

	DrawTexture(CrosshairMiddle, ScreenCenter.X, ScreenCenter.Y, 64, 64, 0, 0, 1, 1);

	auto myChar = Cast<ABaseChar>(GetOwningPawn());

	if (myChar && myChar->GetEquippedWeapon())
	{
		float Spread = Offset;

		DrawTexture(CrosshairLeft, ScreenCenter.X - Spread, ScreenCenter.Y, 64, 64, 0, 0, 1, 1);
		DrawTexture(CrosshairRight, ScreenCenter.X + Spread, ScreenCenter.Y, 64, 64, 0, 0, 1, 1);
		DrawTexture(CrosshairTop, ScreenCenter.X, ScreenCenter.Y - Spread, 64, 64, 0, 0, 1, 1);
		DrawTexture(CrosshairBottom, ScreenCenter.X, ScreenCenter.Y + Spread, 64, 64, 0, 0, 1, 1);
	}
}
