// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyAIController.generated.h"

/**
 *
 */


UCLASS()
class STEALTHGAME_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
		class UBehaviorTree* AIBehavior;

	UPROPERTY(EditAnywhere)
		FVector PatrolLocation = FVector(500.f, 550.f, 88.f);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		class UAIPerceptionComponent* AIPercComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		class UAISenseConfig_Hearing* HearConfig;

		APawn* PlayerPawn;

	void SetupPreceptionSystem();

	UFUNCTION()
		void OnTargetDetected(AActor* DetectedActor, FAIStimulus Stimulus);

		void OnTargetHearing(AActor* HearingActor, FAIStimulus Stimulus);

};
