
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
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
	int XSize = 1; //number of squares in X axis
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
	int YSize = 1; //number of squares in Y axis
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
	float ZMultiplier = 100.f; 
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
	float NoiseScale = 1.f; 

	UPROPERTY(EditAnywhere, Meta =(ClampMin = 0.00001))
	float Scale = 100;
	UPROPERTY(EditAnywhere, Meta =(ClampMin = 0.00001))
	float UVScale = 1;
	UPROPERTY(EditAnywhere, Meta =(ClampMin = 0.00001))
	bool EnableCollision = true;
	

	/*UPROPERTY(EditAnywhere)
	UStaticMeshComponent* SuperMesh;*/

protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* SandMaterial;

private:
	//UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true)) //see vertices in UE
	UProceduralMeshComponent* ProceduralMeshComp;
	TArray<FVector> Vertices;
	TArray<int> Triangles;
	TArray<FVector2D> UV0;
	TArray<FVector> Normals;
	TArray<struct FProcMeshTangent> Tangents;


	void CreateVertices();
	void CreateTriangles();

};
