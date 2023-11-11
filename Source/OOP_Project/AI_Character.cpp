// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_Character.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/MeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Projectile.h"
#include "PlayerCharacter.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AAI_Character::AAI_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 600.0f, 0.0f);

	ClawBox = CreateDefaultSubobject<USphereComponent>(TEXT("Claw Box"));
	ClawBox->SetupAttachment(GetMesh(), FName("AttackingCollision"));

	FullHealth = 150;

	Damage = 20;

}

void AAI_Character::OnAttackHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if (Player && bIsAttack) {
		Player->UpdateHealth(-Damage);
	}

}

// Called when the game starts or when spawned
void AAI_Character::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AAI_Character::OnBeginOverlap);

	ClawBox->OnComponentBeginOverlap.AddDynamic(this, &AAI_Character::OnAttackHit);

	Health = FullHealth;

	if (ZombieSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ZombieSound, GetActorLocation());
	}
	
}

// Called every frame
void AAI_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	APlayerCharacter* Players = Cast<APlayerCharacter>(GetMesh());
	if (Health <= 0) {
		bIsAlive = false;
	}
}

// Called to bind functionality to input
void AAI_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AAI_Character::OnBeginOverlap(class UPrimitiveComponent* HitComp,
	class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AProjectile* Bullet = Cast<AProjectile>(OtherActor);
	if (Bullet) {
		Health -= 20;
		UE_LOG(LogTemp, Warning, TEXT("Health: %f"), Health);
	}
	
}