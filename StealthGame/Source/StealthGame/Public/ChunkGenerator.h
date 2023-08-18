#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "ChunkGenerator.generated.h"

class UProceduralMeshComponent;
class UMaterialInterface;

UCLASS()
class STEALTHGAME_API AChunkGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	AChunkGenerator();
	virtual void Tick(float DeltaTime) override;


	/*UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
	int XSize = 1; //number of squares in X axis
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
	int YSize = 1; //number of squares in Y axis*/

	UPROPERTY(EditAnywhere, Category = "Cellulare")
	float Jitter = 0;

	UPROPERTY(EditAnywhere, Category = "Noise")
	int Seed = 0;
	UPROPERTY(EditAnywhere, Category = "Noise")
	float Frequency = 1.f;
	UPROPERTY(EditAnywhere, Category = "Noise")
	int Octaves = 0;
	UPROPERTY(EditAnywhere, Category = "Noise")
	float Lacunarity = 1.f;
	UPROPERTY(EditAnywhere, Category = "Noise")
	float Gain = 1.f;

	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
	int Size = 0;
	UPROPERTY(EditAnywhere, Meta =(ClampMin = 0.00001))
	float VertexDistance = 100;
	UPROPERTY(EditAnywhere, Meta =(ClampMin = 0.00001))
	float UVScale = 1;
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
	float ZMultiplier = 100.f; 
	UPROPERTY(EditAnywhere)
	UMaterialInterface* Material;

	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
	float NoiseScale = 1.f; 
	UPROPERTY(EditAnywhere, Meta =(ClampMin = 0.00001))
	bool EnableCollision = true;

	void Generate();


protected:
	virtual void BeginPlay() override;



private:
	//UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true)) //see vertices in UE
	UProceduralMeshComponent* ProceduralMeshComp;
	TArray<FVector> Vertices;
	TArray<int> Triangles;
	TArray<FVector2D> UV0;
	TArray<FVector> Normals;
	TArray<struct FProcMeshTangent> Tangents;


	void CreateVertices(int XYSize);
	void CreateTriangles(int XYSize);
	float CalculateZ(int X, int Y);
	float CalculateCordinates(float Distance, int XYIndex);

};
