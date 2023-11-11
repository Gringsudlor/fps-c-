// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "AI_Character.generated.h"

UCLASS()
class OOP_PROJECT_API AAI_Character : public AMyCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAI_Character();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AWaypoint* NextWaypoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USphereComponent* ClawBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bIsAttack = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* ZombieSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Damage;

	UFUNCTION()
		void OnBeginOverlap(class UPrimitiveComponent* HitComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnAttackHit(class UPrimitiveComponent* HitComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
};
