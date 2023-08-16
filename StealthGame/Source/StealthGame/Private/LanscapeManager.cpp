// Fill out your copyright notice in the Description page of Project Settings.


#include "LanscapeManager.h"
#include "MeshGenerator.h"

// Sets default values
ALanscapeManager::ALanscapeManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALanscapeManager::BeginPlay()
{
	Super::BeginPlay();

	FVector Location(0.0f, 0.0f, 0.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	//FActorSpawnParameters SpawnParams;

	//ChunkSpawn->


	GetWorld()->SpawnActor<AMeshGenerator>(ChunkClass, Location, Rotation);

	//ChunkSpawn->SetLocation
	//GetWorld()->Spaw
}

// Called every frame
void ALanscapeManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

