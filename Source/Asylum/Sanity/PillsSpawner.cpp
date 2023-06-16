// Fill out your copyright notice in the Description page of Project Settings.


#include "PillsSpawner.h"
#include "Components/BoxComponent.h"
#include "Asylum/Sanity/Pills.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"



// Sets default values
APillsSpawner::APillsSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AreaSpawn = CreateDefaultSubobject<UBoxComponent>(TEXT("AreaSpawn"));
	RootComponent = AreaSpawn;
}
		
		

	
void APillsSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	for (int i = 0; i < MaxPills; i++)
	{
		SpawnObj();
		
	}
	
	
}

void APillsSpawner::SpawnObj()
{
	if (Kind)
	{
		Pillole++;

		auto point{ FindPoint() };

		auto mypill{ GetWorld()->SpawnActor<AActor>(Kind,point , FRotator(0, 0, 0)) };
		
		
		/*UNiagaraFunctionLibrary::SpawnSystemAttached(
			VFX_Pills,
			mypill->GetRootComponent(),
			NAME_None,
			mypill->GetActorLocation(),
			mypill->GetActorRotation(),
			EAttachLocation::SnapToTarget,
			false
		);*/
		
		
		
		
		

		//UE_LOG(LogTemp, Error, TEXT("%s")*mypill->GetActorLocation().ToString());

		mypill->SetOwner(this);

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

bool APillsSpawner::VerifySphere(FVector NewLoc)
{
	FHitResult Hit;
	FQuat Rot{ FRotator(0).Quaternion() };

	bool Found = GetWorld()->SweepSingleByChannel(Hit, NewLoc,
		NewLoc + FVector(0, 0, 100),
		Rot,
		ECC_Visibility,
		FCollisionShape::MakeSphere(20.f));

	//DrawDebugSphere(GetWorld(), NewLoc,  20.f, 8, (Found ? FColor::Green : FColor::Red), true, 5.f);
	//UE_LOG(LogTemp, Error, TEXT("Verifico") );


	return Found;
}

FVector APillsSpawner::FindPoint()
{
	FVector Estensione = AreaSpawn->Bounds.BoxExtent;
	FVector Origine = AreaSpawn->Bounds.Origin;
	FVector PuntoRandom; 
	
		do {

			//trovo un punto random
			PuntoRandom = UKismetMathLibrary::RandomPointInBoundingBox(Origine, Estensione);
	

		} while (VerifySphere(PuntoRandom));// se fa collisione continuo a rientrarci dentro


	return PuntoRandom;


}

