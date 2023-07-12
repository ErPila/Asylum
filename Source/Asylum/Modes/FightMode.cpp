// Fill out your copyright notice in the Description page of Project Settings.


#include "FightMode.h"
#include "Kismet/GameplayStatics.h"
#include "Asylum/Characters/BaseChar.h"

void AFightMode::BeginPlay()
{
	Super::BeginPlay();
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), Char, Players);
}
