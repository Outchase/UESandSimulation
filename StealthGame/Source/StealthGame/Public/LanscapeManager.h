#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LanscapeManager.generated.h"

UCLASS()
class STEALTHGAME_API ALanscapeManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ALanscapeManager();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<class AChunkGenerator> ChunkClass;

	UPROPERTY(EditAnywhere, Category = "Parameters")
	int Distance;

	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0), Category = "Chunk Parameters")
	int Size = 1;
	UPROPERTY(EditAnywhere, Meta =(ClampMin = 0.00001), Category = "Chunk Parameters")
	float Scale = 100;
	UPROPERTY(EditAnywhere, Meta =(ClampMin = 0.00001), Category = "Chunk Parameters")
	float UVScale = 1;
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0), Category = "Chunk Parameters")
	float ZMultiplier = 100.f; 
	UPROPERTY(EditAnywhere, Category = "Chunk Parameters")
	UMaterialInterface* Material;
	UPROPERTY(EditAnywhere, Category = "Chunk Parameters")
	bool EnableCollision = true;

	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0), Category = "Noise")
	float NoiseScale = 1.f;
 


protected:
	virtual void BeginPlay() override;

	//Enable Multithreading to test
	UPROPERTY(EditAnywhere, Category = MultiThreading)
	bool bTestMultiThreding;

	UPROPERTY(EditAnywhere, Category = MultiThreading)
	int32 MaxPrime;

	UPROPERTY(EditAnywhere, Category = MultiThreading)
	bool bIsAsync = false;

private:
	void SetChunkParameters(AChunkGenerator* Chunk);
	void SetVisible();
	class AChunkGenerator* ActorChunk = nullptr;
	FVector2D CurrentChunkPosition;
	FVector Visible;
	TSet<FVector> Generated;
	TSet<FString> Test;
	int X= 0;
	int Y= 0;
};