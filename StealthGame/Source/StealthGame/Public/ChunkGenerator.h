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

	float XOffset;
	float YOffset;

	//Generates the Chunks
	void Generate();

	//Calculates prime numbers in the game thread
	UFUNCTION(BlueprintCallable, Category = MultiThreading)
	void CalculatePrimeNumbers();

	//Calculates prime numbers in a background thread
	UFUNCTION(BlueprintCallable, Category = MultiThreading)
	void CalculatePrimeNumbersAsync();

	UPROPERTY(EditAnywhere, Category = MultiThreading)
	int32 MaxPrime;

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

//Able to access the same function from different code classes
namespace ThreadTest
{
	/*
	-----------------------------------------------------------------------------------------------------------------------------------------------
	--Here was an example to use it for my generate chunks function but that causes the engine to crash so I chose simple tasks to demonstrate it --
											    **Feel free to analyze it and maybe find the issue**
	-----------------------------------------------------------------------------------------------------------------------------------------------
											↓ 	↓											       ↓  ↓  
	*/

	/*static void Generate2(TArray<FVector> Vertices, TArray<int> Triangles, TArray<FVector2D> UV0, int Size, float XOffset, float YOffset, float NoiseScale, float ZMultiplier, float VertexDistance, UProceduralMeshComponent* ProceduralMeshComp, TArray<FVector> Normals, TArray<struct FProcMeshTangent> Tangents, bool EnableCollision, UMaterialInterface* Material)
	{
		//Clear at the Beginning of function, cause performance issue when not clearing
		Vertices.Reset();
		Triangles.Reset();
		UV0.Reset();

		//CreateVertices(Size);
		for(int I = 0; I <= Size; I++){
			for(int J = 0; J <= Size; J++){
				float Z=FMath::PerlinNoise2D(FVector2D((I + 0.1 + XOffset) * NoiseScale,(J + 0.1 + YOffset) * NoiseScale)) * ZMultiplier;
				//float Z= CalculateZ(I, J);
				//GEngine->AddOnScreenDebugMessage(-1, 999.f, FColor::Yellow, FString::Printf(TEXT("Z %f"), Z));

				//Calculate it to set the pivot
				float X = (((Size/-2.f)+I)*(int32)VertexDistance);
				float Y = (((Size/-2.f)+J)*(int32)VertexDistance);
				float UVX = (((Size/-2.f)+I)*(int32)VertexDistance);
				float UVY = (((Size/-2.f)+J)*(int32)VertexDistance);

				//float X = CalculateCordinates(VertexDistance, I);
				//float Y = CalculateCordinates(VertexDistance, J);
				//float UVX = CalculateCordinates(UVScale , I);
				//float UVY = CalculateCordinates(UVScale, J);

				For Debugging:
				UE_LOG(LogTemp, Warning, TEXT("((%i / -2) + %i) * %f = %f"), Size, I, VertexDistance, X);
				UE_LOG(LogTemp, Warning, TEXT("((%i / -2) + %i) * %f = %f"), Size, J, VertexDistance, Y);
				UE_LOG(LogTemp, Warning, TEXT("((%i / -2) + %i) * %f = %f"), Size, I, VertexDistance, UVX);
				UE_LOG(LogTemp, Warning, TEXT("((%i / -2) + %i) * %f = %f"), Size, J, VertexDistance, UVY);
				UE_LOG(LogTemp, Warning, TEXT("  "));
				

				Vertices.Add(FVector(X, Y, Z));
				UV0.Add(FVector2D(UVX, UVY));
			}
		}

		int Vertex = 0;

		for(int I = 0; I < Size; I++){
			for(int J = 0; J < Size; J++){
				Triangles.Add(Vertex);
				Triangles.Add(Vertex + 1);
				Triangles.Add(Vertex + Size + 1);
				Triangles.Add(Vertex + 1);
				Triangles.Add(Vertex + Size + 2);
				Triangles.Add(Vertex + Size + 1);

				Vertex++;
			}
			Vertex++;
		}
		

		UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Vertices, Triangles, UV0, Normals, Tangents );

		ProceduralMeshComp->CreateMeshSection(
			0, //section Index
			Vertices,
			Triangles, 
			Normals,
			UV0,
			TArray<FColor>(),
			Tangents, 
			EnableCollision); //enable mesh collision

		ProceduralMeshComp->SetMaterial(0, Material);
	}*/

	//perform calculation
	static void CalculatePrimeNumbers(int32 MaxLimit)
	{
		//Calculating prime numbers
		//Iterate through numbers from 1 to MaxLimit and check for divisibility
		for (int32 i = 1; i <= MaxLimit; i++)
		{
			bool isPrime = true;

			for (int32 j = 2; j <= i / 2; j++)
			{
				//If what's left after division is equal to 0
				if (FMath::Fmod(i, j) == 0)
				{
					isPrime = false;
					break;
				}
			}

			//If no divisors found, the number is prime
			if (isPrime){
				GLog->Log("Prime number #" + FString::FromInt(i) + ": " + FString::FromInt(i));
			} 
		}
	}
}


class CalculateAsyncTask : public FNonAbandonableTask
{
	//Tested more:
	/*TArray<FVector> Vertices;
	TArray<int> Triangles;
	TArray<FVector2D> UV0;
	int Size;
	float XOffset;
	float YOffset;
	float NoiseScale;
	float ZMultiplier;
	int Distance;
	UProceduralMeshComponent* ProceduralMeshComp;
	TArray<FVector> Normals;
	TArray<struct FProcMeshTangent> Tangents;
	bool EnableCollision;
	UMaterialInterface* Material;*/

	int32 MaxPrime;


public:
	/*GenerateAsyncTask(TArray<FVector> Vertices, TArray<int> Triangles, TArray<FVector2D> UV0, int Size, float XOffset, float YOffset, float NoiseScale, float ZMultiplier, int Distance, UProceduralMeshComponent* ProceduralMeshComp, TArray<FVector> Normals, TArray<struct FProcMeshTangent> Tangents, bool EnableCollision, UMaterialInterface* Material)
	{
		this->Vertices = Vertices;
		this->Triangles = Triangles;
		this->UV0 = UV0;
		this->Size = Size;
		this->XOffset = XOffset;
		this->YOffset = YOffset;
		this->NoiseScale = NoiseScale;
		this->ZMultiplier = ZMultiplier;
		this->Distance = Distance;
		this->ProceduralMeshComp = ProceduralMeshComp;
		this->Normals = Normals;
		this->Tangents = Tangents;
		this->EnableCollision = EnableCollision;
		this->Material = Material;
	}*/

	//Default constructor
	CalculateAsyncTask(int32 MaxPrime){
		this->MaxPrime = MaxPrime;
	}
 
	//Gives API engine the necessary information about the thread that we occupy and the progress of our task
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(CalculateAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
	}
 
	/*This function is executed when we tell our task to execute*/
	void DoWork()
	{
		//ThreadTest::Generate2(Vertices, Triangles, UV0, Size, XOffset, YOffset, NoiseScale, ZMultiplier, Distance, ProceduralMeshComp, Normals, Tangents, EnableCollision, Material);
		
		ThreadTest::CalculatePrimeNumbers(MaxPrime);
 
		GLog->Log("");
		GLog->Log("End of calculating on background thread");
		GLog->Log("");
	}
};