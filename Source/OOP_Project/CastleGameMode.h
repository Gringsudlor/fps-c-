// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyGameMode.h"
#include "CastleGameMode.generated.h"

/**
 * 
 */
UCLASS()
class OOP_PROJECT_API ACastleGameMode : public AMyGameMode
{
	GENERATED_BODY()

public:
	ACastleGameMode();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SpawnPlayerMedKit();

	virtual void SpawnAmmoBox();

	virtual void SpawnZombie();
	
};
