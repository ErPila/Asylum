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
	
	UPROPERTY(EditAnywhere)
	bool bOpen{ false };
public:	
	// Sets default values for this component's properties
	UDoor();

	void OpenDoor();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//virtual void OnCostruct() override;

	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
