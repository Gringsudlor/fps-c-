// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameMode.h"
#include "PlayerCharacter.h"

AMyGameMode::AMyGameMode() {
	//TSubclassOf<APlayerCharacter> DefaultPawnClass;
	PrimaryActorTick.bCanEverTick = true;
}
void AMyGameMode::BeginPlay() {
	FTimerHandle MedHandle;
	FTimerHandle AmmoHandle;
	FTimerHandle ZombieHandle;
	GetWorldTimerManager().SetTimer(
		MedHandle, this, &AMyGameMode::SpawnPlayerMedKit, SpawnMedDelay, true);

	GetWorldTimerManager().SetTimer(
		AmmoHandle, this, &AMyGameMode::SpawnAmmoBox, SpawnAmmoDelay, true);

	GetWorldTimerManager().SetTimer(
		ZombieHandle, this, &AMyGameMode::SpawnZombie, SpawnZombieDelay, true);


	NumZom = 2;
	ZombieIncreaseRate = 2;
	CanSpawn = true;
}

void AMyGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyGameMode::SpawnPlayerMedKit()
{
	FVector SpawnPosition = FVector(SpawnMedX, SpawnMedY, SpawnMedZ);
	FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);

	GetWorld()->SpawnActor(MedKit, &SpawnPosition, &SpawnRotation);
}

void AMyGameMode::SpawnAmmoBox()
{
	FVector SpawnPosition = FVector(SpawnAmmoX, SpawnAmmoY, SpawnAmmoZ);
	FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);

	GetWorld()->SpawnActor(AmmoBox, &SpawnPosition, &SpawnRotation);
}

void AMyGameMode::SpawnZombie()
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
