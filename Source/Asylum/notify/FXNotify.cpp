// Fill out your copyright notice in the Description page of Project Settings.


#include "FXNotify.h"
#include "NiagaraFunctionLibrary.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

void UFXNotify::SpawnEffects(USkeletalMeshComponent* Mesh)
{
	if (Mesh) 
	{
		auto obj = Mesh->GetOwner();
		FVector Location{ Mesh->GetBoneLocation(BoneName) };

		if (Sound)
		{
			UGameplayStatics::PlaySoundAtLocation(obj,Sound, Location);
		}

		if (Sfx)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(obj, Sfx, Location);
		}
	}

}
