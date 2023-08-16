// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LanscapeManager.generated.h"

UCLASS()
class STEALTHGAME_API ALanscapeManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALanscapeManager();

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<class AMeshGenerator> ChunkClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
