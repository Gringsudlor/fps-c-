// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyGameMode.h"
#include "CaveGameMode.generated.h"

/**
 * 
 */
UCLASS()
class OOP_PROJECT_API ACaveGameMode : public AMyGameMode
{
	GENERATED_BODY()
public:
	ACaveGameMode();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SpawnPlayerMedKit();

	virtual void SpawnAmmoBox();

	virtual void SpawnZombie();

	
};
