// Fill out your copyright notice in the Description page of Project Settings.


#include "PillsSpawner.h"
#include "Components/BoxComponent.h"
#include "Asylum/Sanity/Pills.h"
#include "Kismet/KismetMathLibrary.h"



// Sets default values
APillsSpawner::APillsSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	AreaSpawn = CreateDefaultSubobject<UBoxComponent>(TEXT("AreaSpawn"));
	RootComponent = AreaSpawn;
}
		
		

	
void APillsSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	for (int i = 0; i < 2; i++)
	{
		SpawnObj();
		Pillole++;
	}
	
	
}

void APillsSpawner::SpawnObj()
{
	if (Kind)
	{
		GetWorld()->SpawnActor<AActor>(Kind, FindPoint(), FRotator(0, 0, 0));
	}
}

// Called every frame
void APillsSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Pillole < 2)
	{
		SpawnObj();
	}

}

FVector APillsSpawner::FindPoint()
{
	FVector Estensione = AreaSpawn->Bounds.BoxExtent;
	FVector Origine = AreaSpawn->Bounds.Origin;
	return UKismetMathLibrary::RandomPointInBoundingBox(Origine, Estensione);
}

