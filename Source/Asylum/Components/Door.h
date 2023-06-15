// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Door.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ASYLUM_API UDoor : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere);
	UStaticMesh* MeshToOpen;

public:	
	// Sets default values for this component's properties
	UDoor();

	void OpenDoor();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
