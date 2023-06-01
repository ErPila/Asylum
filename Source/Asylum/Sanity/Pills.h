// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pills.generated.h"

UCLASS()
class ASYLUM_API APills : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APills();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnyWhere, Category = "Vfx")
		class UNiagaraSystem* VFX_Pills;

	class UNiagaraComponent* MySystem;

};
