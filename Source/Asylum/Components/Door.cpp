// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"

// Sets default values for this component's properties
UDoor::UDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;	

	// ...
}

void UDoor::OpenDoor()
{
	
	if(Mesh)
	{
		//UE_LOG(LogTemp, Error, TEXT("%s"), *p->GetName());

		if (!bOpen)
		{
			UE_LOG(LogTemp, Error, TEXT("Porta aperta"));
			Mesh->SetActorRelativeRotation(FRotator(0, 90, 0));
			//p->SetRelativeRotation(FRotator(0, 0, 90));
			bOpen = true;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Porta Chiusa"));
			Mesh->SetActorRelativeRotation(FRotator(0, 0, 0));
			//p->SetRelativeRotation(FRotator(0, 0, 0));
			bOpen = false;
		}
	}
}


// Called when the game starts
void UDoor::BeginPlay()
{
	Super::BeginPlay();

	Mesh = Cast<AActor>(GetOwner());

	OpenDoor();

	
	
}


// Called every frame
void UDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}

