// Fill out your copyright notice in the Description page of Project Settings.


#include "CaveGameMode.h"

ACaveGameMode::ACaveGameMode() {

	PrimaryActorTick.bCanEverTick = true;

}

void ACaveGameMode::BeginPlay()
{
	FTimerHandle MedHandle;
	FTimerHandle AmmoHandle;
	FTimerHandle ZombieHandle;
	GetWorldTimerManager().SetTimer(
		MedHandle, this, &ACaveGameMode::SpawnPlayerMedKit, SpawnMedDelay, true);

	GetWorldTimerManager().SetTimer(
		AmmoHandle, this, &ACaveGameMode::SpawnAmmoBox, SpawnAmmoDelay, true);

	GetWorldTimerManager().SetTimer(
		ZombieHandle, this, &ACaveGameMode::SpawnZombie, SpawnZombieDelay, true);


	NumZom = 2;
	ZombieIncreaseRate = 2;
	CanSpawn = true;
}

void ACaveGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACaveGameMode::SpawnPlayerMedKit()
{
	FVector SpawnPosition = FVector(SpawnMedX, SpawnMedY, SpawnMedZ);
	FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);

	GetWorld()->SpawnActor(MedKit, &SpawnPosition, &SpawnRotation);
}

void ACaveGameMode::SpawnAmmoBox()
{
	FVector SpawnPosition = FVector(SpawnAmmoX, SpawnAmmoY, SpawnAmmoZ);
	FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);

	GetWorld()->SpawnActor(AmmoBox, &SpawnPosition, &SpawnRotation);
}

void ACaveGameMode::SpawnZombie()
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
