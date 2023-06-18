// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Door.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ASYLUM_API UDoor : public UActorComponent
{
	GENERATED_BODY()

	AActor* Mesh;
	
	uint8 OpenState{ 0 };  // closed // opening // open // closing

	UPROPERTY(EditAnyWhere)
	float OpenAngle{ 90.f };


public:	
	// Sets default values for this component's properties
	UDoor();

	void OpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//virtual void OnCostruct() override;	
	float StartYaw;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
