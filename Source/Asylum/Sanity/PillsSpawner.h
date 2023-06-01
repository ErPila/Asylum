// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PillsSpawner.generated.h"

UCLASS()
class ASYLUM_API APillsSpawner : public AActor
{
	GENERATED_BODY() 
	
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawn", meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* AreaSpawn;

	FVector FindPoint();

public:	
	// Sets default values for this actor's properties
	APillsSpawner();

	FORCEINLINE UBoxComponent* GetAreaSpawn() const { return AreaSpawn; }

	UPROPERTY(EditAnyWhere, Category = "Spawn")
		TSubclassOf<class APills> Kind;

	UPROPERTY( BlueprintReadWrite, Category = "Pillole")
	int Pillole { 0 };

	UPROPERTY(EditAnyWhere, Category = "Pillole")
		int MaxPills{ 2 };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Pillole")
	void SpawnObj();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool VerifySphere(FVector NewLoc);


};
