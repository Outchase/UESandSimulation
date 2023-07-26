// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"

// Sets default values
AEnemyAIController::AEnemyAIController()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetupPreceptionSystem();
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetDetected);

}

// Called when the game starts or when spawned
void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	if (AIBehavior != nullptr) {
		RunBehaviorTree(AIBehavior);

		PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

		GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
		GetBlackboardComponent()->SetValueAsVector(TEXT("PatrolLocation"), PatrolLocation);
		GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());

		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetDetected);
	}
}

// Called every frame
void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyAIController::SetupPreceptionSystem() {
	AIPercComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception Component"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfigComp"));

	if (AIPercComp != nullptr)
	{

		SetPerceptionComponent(*AIPercComp);
		if (SightConfig != nullptr) {

			SightConfig->SightRadius = 1000.f;
			SightConfig->LoseSightRadius = SightConfig->SightRadius + 500.f;
			SightConfig->PeripheralVisionAngleDegrees = 85.f;
			SightConfig->NearClippingRadius = 20.f;
			SightConfig->DetectionByAffiliation.bDetectEnemies = true;
			SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
			SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

			if (GetPerceptionComponent() != nullptr) {

				GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
				GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetDetected);
				GetPerceptionComponent()->ConfigureSense(*SightConfig);
			}
		}

		if (HearConfig != nullptr) {
			HearConfig->HearingRange = 600.f;
			HearConfig->DetectionByAffiliation.bDetectEnemies = true;
			HearConfig->DetectionByAffiliation.bDetectNeutrals = true;
			HearConfig->DetectionByAffiliation.bDetectFriendlies = true;

			GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetHearing);
			GetPerceptionComponent()->ConfigureSense(*SightConfig);
		}
	}


}



void AEnemyAIController::OnTargetDetected(AActor* DetectedActor, FAIStimulus Stimulus) {
	//follow AI on Alert to backup
	//Both AI detecs eachoter make unique dialogue
	//Set AI who detect as Leader that other cna Follow
	if (Stimulus.WasSuccessfullySensed() && DetectedActor == PlayerPawn) {
		GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), DetectedActor);
		//SetFocus(DetectedActor, 1);
	}
	else {
		GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), nullptr);
		GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), DetectedActor->GetActorLocation());

	}
}

void AEnemyAIController::OnTargetHearing(AActor* HearingActor, FAIStimulus Stimulus){

	if (Stimulus.WasSuccessfullySensed() && HearingActor == PlayerPawn) {
		GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), HearingActor->GetActorLocation());
	}
}