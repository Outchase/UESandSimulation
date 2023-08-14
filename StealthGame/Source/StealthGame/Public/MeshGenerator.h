
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MeshGenerator.generated.h"

class UProceduralMeshComponent;
class UMaterialInterface;

UCLASS()
class STEALTHGAME_API AMeshGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	AMeshGenerator();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
	int XSize = 0; //number of squares in X axis
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
	int YSize = 0; //number of squares in Y axis
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
	float ZMultiplier = 1.f; 
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
	float NoiseScale = 1.f; 

	UPROPERTY(EditAnywhere, Meta =(ClampMin = 0.00001))
	float Scale = 0;
	UPROPERTY(EditAnywhere, Meta =(ClampMin = 0.00001))
	float UVScale = 0;
	UPROPERTY(EditAnywhere, Meta =(ClampMin = 0.00001))
	bool EnableCollision = true;
	

	/*UPROPERTY(EditAnywhere)
	UStaticMeshComponent* SuperMesh;*/

protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
	UMaterialInterface* SandMaterial;

private:
	//UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true)) //see vertices in UE
	UProceduralMeshComponent* ProceduralMeshComp;
	TArray<FVector> Vertices;
	TArray<int> Triangles;
	TArray<FVector2D> UV0;

	void CreateVertices();
	void CreateTriangles();

	float GenerateVoronoiNoise(float X, float Y, int32 NumCells);

};
