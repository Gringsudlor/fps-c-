// Fill out your copyright notice in the Description page of Project Settings.


#include "CityGameMode.h"

ACityGameMode::ACityGameMode() {

	PrimaryActorTick.bCanEverTick = true;
}

void ACityGameMode::BeginPlay()
{
	FTimerHandle MedHandle;
	FTimerHandle AmmoHandle;
	FTimerHandle ZombieHandle;
	GetWorldTimerManager().SetTimer(
		MedHandle, this, &ACityGameMode::SpawnPlayerMedKit, SpawnMedDelay, true);

	GetWorldTimerManager().SetTimer(
		AmmoHandle, this, &ACityGameMode::SpawnAmmoBox, SpawnAmmoDelay, true);

	GetWorldTimerManager().SetTimer(
		ZombieHandle, this, &ACityGameMode::SpawnZombie, SpawnZombieDelay, true);


	NumZom = 2;
	ZombieIncreaseRate = 2;
	CanSpawn = true;
}

void ACityGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACityGameMode::SpawnPlayerMedKit()
{
	FVector SpawnPosition = FVector(SpawnMedX, SpawnMedY, SpawnMedZ);
	FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);

	GetWorld()->SpawnActor(MedKit, &SpawnPosition, &SpawnRotation);
}

void ACityGameMode::SpawnAmmoBox()
{
	FVector SpawnPosition = FVector(SpawnAmmoX, SpawnAmmoY, SpawnAmmoZ);
	FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);

	GetWorld()->SpawnActor(AmmoBox, &SpawnPosition, &SpawnRotation);
}

void ACityGameMode::SpawnZombie()
{
	FVector SpawnPosition = FVector(SpawnZombieX, SpawnZombieY, SpawnZombieZ);
	FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);
	for (int32 i = 0; i <= NumZom; i++) {
		if (CanSpawn)
			GetWorld()->SpawnActor(Zombie, &SpawnPosition, &SpawnRotation);
		i++;
		if (i == NumZom) {
			CanSpawn = false;
		}
	}
	NumZom += ZombieIncreaseRate;
}
