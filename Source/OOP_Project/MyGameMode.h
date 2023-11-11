// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "AI_Character.h"
#include "AmmoBox.h"
#include "MyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class OOP_PROJECT_API AMyGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AMyGameMode();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> MedKit;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> AmmoBox;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AAI_Character> Zombie;

	UPROPERTY(EditAnywhere)
		float SpawnMedDelay;

	UPROPERTY(EditAnywhere)
		float SpawnAmmoDelay;

	UPROPERTY(EditAnywhere)
		float SpawnZombieDelay;

	UPROPERTY(EditAnywhere)
		float SpawnMedX;

	UPROPERTY(EditAnywhere)
		float SpawnMedY;

	UPROPERTY(EditAnywhere)
		float SpawnMedZ;

	UPROPERTY(EditAnywhere)
		float SpawnAmmoX;

	UPROPERTY(EditAnywhere)
		float SpawnAmmoY;

	UPROPERTY(EditAnywhere)
		float SpawnAmmoZ;

	UPROPERTY(EditAnywhere)
		float SpawnZombieX;

	UPROPERTY(EditAnywhere)
		float SpawnZombieY;

	UPROPERTY(EditAnywhere)
		float SpawnZombieZ;

	virtual void SpawnPlayerMedKit();

	virtual void SpawnAmmoBox();

	virtual void SpawnZombie();

	bool CanSpawn;

	int32 NumZom;
	int32 ZombieIncreaseRate;
};
