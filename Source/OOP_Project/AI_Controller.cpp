// Fill out your copyright notice in the Description page of Project Settings.

#include "AI_Controller.h"
#include "AI_Character.h"
#include "Waypoint.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"



AAI_Controller::AAI_Controller()
{
	PrimaryActorTick.bCanEverTick = true;

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
	SightConfig->SetMaxAge(AISightAge);

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AAI_Controller::OnPawnDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);

	
}

void AAI_Controller::BeginPlay()
{
	Super::BeginPlay();
}

void AAI_Controller::OnPossess(APawn* Pawns)
{
	Super::OnPossess(Pawns);
}

void AAI_Controller::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	AAI_Character* Zombie = Cast<AAI_Character>(GetPawn());

	if(Zombie->bIsAlive){
		if (DistanceToPlayer > AISightRadius) 
		{
			bIsPlayerDetected = false;
		}

		if (Zombie->NextWaypoint != nullptr && !bIsPlayerDetected)
		{
			MoveToActor(Zombie->NextWaypoint, 5.0f);
			Zombie->GetCharacterMovement()->MaxWalkSpeed = SpeedBase;
		}
		/*else if (bIsPlayerDetected)
		{
			APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			MoveToActor(Player, 5.0f);

			Zombie->GetCharacterMovement()->MaxWalkSpeed = SpeedBase + SpeedRun;	
		}
		*/
		
	}
	
}

FRotator AAI_Controller::GetControlRotation() const
{
	if (GetPawn() == nullptr)
	{
		return FRotator(0.0f, 0.0f, 0.0f);
	}

	return FRotator(0.0f, GetPawn()->GetActorRotation().Yaw, 0.0f);

}

void AAI_Controller::OnPawnDetected(const TArray<AActor*>& DetectedPawns)
{

	for (size_t i = 0; i < DetectedPawns.Num(); i++) 
	{
		DistanceToPlayer = GetPawn()->GetDistanceTo(DetectedPawns[i]);
		APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), DistanceToPlayer);
	}

	bIsPlayerDetected = true;

}


