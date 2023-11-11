// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "Projectile.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/InputSettings.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "MedKit.h"
#include "AmmoBox.h"
#include "Engine/World.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->AirControl = 0.2f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(GetMesh(), FName("head"));
	FollowCamera->bUsePawnControlRotation = false;

	GunMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	GunMeshComponent->CastShadow = false;
	GunMeshComponent->SetupAttachment(GetMesh(), "Weapon_Attach");

	Gun_Muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));
	Gun_Muzzle->SetupAttachment(GunMeshComponent);
	Gun_Muzzle->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	FullHealth = 100.0f;

	StartMoney = 0.0f;

	FullMagAmmo = 30;

	AllAmmo = 100;

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = FullHealth;

	Money = StartMoney;

	Ammo = FullMagAmmo;

	KillCount = 0;
	
	FireRate = 0.2f;

	Paused = false;

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnBeginOverlapMedKit);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnBeginOverlapAmmoBox);

	if (Player_Power_Widget_Class != nullptr) {

		Player_Power_Widget = CreateWidget(GetWorld(), Player_Power_Widget_Class);
		Player_Power_Widget->AddToViewport();

	}
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Health <= 0) {

		if (bIsAlive) {

			bIsAlive = false;

			GetMesh()->SetSimulatePhysics(true);

		}
	}
	if (Ammo > 0)
		IsAmmoLeft = true;
	else if (Ammo <= 0) {
		Ammo = 0;
		IsAmmoLeft = false;
	}
	if (AllAmmo <= 0 || Ammo == 30) {
		//AllAmmo = 0;
		CanReload = false;
	}
	else if (AllAmmo > 0)
		CanReload = true;
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCharacter::OnFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &APlayerCharacter::StopFire);

	FInputActionBinding& toggle = PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &APlayerCharacter::OnPause);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &APlayerCharacter::Reload);
	toggle.bExecuteWhenPaused = true;
}

void APlayerCharacter::UpdateHealth(float HealthChange)
{
	Health += HealthChange;
	if (Health > FullHealth) {
		Health = FullHealth;
	}
	
}

void APlayerCharacter::MoveForward(float Axis)
{

	if (bIsAlive) {

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Axis);

	}

}

void APlayerCharacter::MoveRight(float Axis)
{

	if (bIsAlive) {

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Axis);

	}

}

void APlayerCharacter::Reload()
{
	if (CanReload) {
		if (AllAmmo <= FullMagAmmo && AllAmmo + Ammo <= 30) {
			Ammo += AllAmmo;
			AllAmmo = 0;
		}
		else {
			AllAmmo -= (FullMagAmmo - Ammo);
			Ammo = 30;
		}
		if (ReloadSound != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(this, ReloadSound, GetActorLocation());
		}
	}
}

void APlayerCharacter::OnFire()
{
	IsOnFire = true;
	UE_LOG(LogTemp, Warning, TEXT("Fire!!"));
	if (IsAmmoLeft) {
		// try and fire a projectile
		if (ProjectileClass != NULL)
		{
			UWorld* const World = GetWorld();
			if (World != NULL)
			{

				const FRotator SpawnRotation = GetControlRotation();
				// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
				const FVector SpawnLocation = ((Gun_Muzzle != nullptr) ? Gun_Muzzle->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

				//Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

				// spawn the projectile at the muzzle
				World->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			}


		}
		Ammo--;
		// try and play the sound if specified
		if (FireSound != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		}
	}
}

void APlayerCharacter::OnPause()
{
	if (Pause_Widget_Class != nullptr) {
		
		if (!Paused) {
			Pause_Widget = CreateWidget(GetWorld(), Pause_Widget_Class);
			Pause_Widget->AddToViewport();
			UGameplayStatics::SetGamePaused(GetWorld(), true);
			Paused = true;
			//MyController->bShowMouseCursor = true;
		}
		else if (Paused) {
			Paused = false;
			Pause_Widget->RemoveFromParent();
			UGameplayStatics::SetGamePaused(GetWorld(), false);
			//MyController->bShowMouseCursor = false;
		}

	}
}

void APlayerCharacter::StopFire()
{
	IsOnFire = false;
}

void APlayerCharacter::RestartGame()
{

	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);

}

void APlayerCharacter::OnBeginOverlapMedKit(class UPrimitiveComponent* HitComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	AMedKit* MedKit = Cast<AMedKit>(OtherActor);
	if (MedKit) {

		Health += 10.0f;

		if (Health > FullHealth) {
			Health = FullHealth;
		}

		OtherActor->Destroy();
	}
	
}

void APlayerCharacter::OnBeginOverlapAmmoBox(class UPrimitiveComponent* HitComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	AAmmoBox* AmmoBox = Cast<AAmmoBox>(OtherActor);
	if (AmmoBox) {

		AllAmmo += 60;

		OtherActor->Destroy();
	}

}
