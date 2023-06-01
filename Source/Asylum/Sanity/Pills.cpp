// Fill out your copyright notice in the Description page of Project Settings.


#include "Pills.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"

// Sets default values
APills::APills()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

// Called when the game starts or when spawned
void APills::BeginPlay()
{
	Super::BeginPlay();
	
	MySystem = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		this,
		VFX_Pills,
		GetActorLocation(),
		GetActorRotation());
	
}

void APills::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	MySystem->DestroyComponent();

}

// Called every frame
void APills::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

