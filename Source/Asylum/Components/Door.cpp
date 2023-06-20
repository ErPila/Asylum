// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"

// Sets default values for this component's properties
UDoor::UDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.

	PrimaryComponentTick.bCanEverTick = true;
	//PrimaryComponentTick.bTickEvenWhenPaused = true;
	//PrimaryComponentTick.TickGroup = TG_PrePhysics;
	// ...
}

void UDoor::OpenDoor()
{
	
	if(Mesh)
	{
		switch (OpenState)
		{
		case 0:
		case 2:
		OpenState++;
		break;

		}
	}
}


// Called when the game starts
void UDoor::BeginPlay()
{
	Super::BeginPlay();

	Mesh = Cast<AActor>(GetOwner());
    StartYaw = Mesh->GetActorRotation().Yaw;
		
}


// Called every frame
void UDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	switch (OpenState)
	{
	case 0:  
		
	break; // close


	case 1:   
	{
		FRotator ActualRot{ Mesh->GetActorRotation() };

		if (ActualRot.Yaw < StartYaw + OpenAngle)
		{
			//Mesh->SetActorRelativeRotation(ActualRot + FRotator(0, 90 * DeltaTime, 0));

			Mesh->AddActorLocalRotation(FRotator(0, 90 * DeltaTime, 0));

		}
		else OpenState++;

	}
	break; // opening


	case 2: 
		

	break;  // openstate


	case 3:
	{
		FRotator ActualRot{ Mesh->GetActorRotation() };

		if (ActualRot.Yaw > StartYaw)
		{
			Mesh->AddActorLocalRotation(-FRotator(0, 90 * DeltaTime, 0));

			//Mesh->SetActorRelativeRotation(ActualRot - FRotator(0, 90 * DeltaTime, 0));
		}
		else OpenState = 0;
	}
	break;


	}


}

