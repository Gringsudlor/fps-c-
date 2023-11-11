// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Blueprint/UserWidget.h"

#include "PlayerCharacter.generated.h"

UCLASS()
class OOP_PROJECT_API APlayerCharacter : public AMyCharacter
{
	GENERATED_BODY()

public:

	APlayerCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
		UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Money)
		float Money;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Money)
		float StartMoney;
	
	UPROPERTY(EditAnywhere, Category = "UI HUD")
		TSubclassOf<UUserWidget> Player_Power_Widget_Class;
	UUserWidget* Player_Power_Widget;

	UPROPERTY(EditAnywhere, Category = "UI HUD")
		TSubclassOf<UUserWidget> Pause_Widget_Class;
	UUserWidget* Pause_Widget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
		USkeletalMeshComponent* GunMeshComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
		class USceneComponent* Gun_Muzzle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float FireRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector GunOffset;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		bool IsOnFire = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ammo)
		int32 Ammo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo)
		int32 FullMagAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo)
		int32 AllAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo)
		bool IsAmmoLeft = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo)
		class USoundBase* ReloadSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo)
		bool CanReload = false;

	UFUNCTION(BlueprintCallable, Category = Gameplay)
		void OnFire();

	UFUNCTION(BlueprintCallable, Category = Gameplay)
		void OnPause();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		bool Paused = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		int32 KillCount;

protected:
	// Called when the game starts or when spawned
	
	void MoveForward(float Axis);
	void MoveRight(float Axis);

	void Reload();
	
	void StopFire();


public:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Health")
		void UpdateHealth(float HealthChange);

	UFUNCTION()
		void OnBeginOverlapMedKit(class UPrimitiveComponent* HitComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnBeginOverlapAmmoBox(class UPrimitiveComponent* HitComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
		virtual void RestartGame();

};
