#include "ChunkGenerator.h"
#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"

// Sets default values
AChunkGenerator::AChunkGenerator()
{
	PrimaryActorTick.bCanEverTick = false;

	//set components
	ProceduralMeshComp = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
	ProceduralMeshComp->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void AChunkGenerator::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AChunkGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AChunkGenerator::CreateVertices(int XYSize){
	for(int I = 0; I <= XYSize; I++){
		for(int J = 0; J <= XYSize; J++){
			float Z= CalculateZ(I, J);
			//GEngine->AddOnScreenDebugMessage(-1, 999.f, FColor::Yellow, FString::Printf(TEXT("Z %f"), Z));

			//Calculate it to set the pivot of chunk
			float X = CalculateCordinates(VertexDistance, I);
			float Y = CalculateCordinates(VertexDistance, J);
			float UVX = CalculateCordinates(UVScale , I);
			float UVY = CalculateCordinates(UVScale, J);

			/*//For Debugging:
			UE_LOG(LogTemp, Warning, TEXT("((%i / -2) + %i) * %f = %f"), Size, I, VertexDistance, X);
			UE_LOG(LogTemp, Warning, TEXT("((%i / -2) + %i) * %f = %f"), Size, J, VertexDistance, Y);
			UE_LOG(LogTemp, Warning, TEXT("((%i / -2) + %i) * %f = %f"), Size, I, VertexDistance, UVX);
			UE_LOG(LogTemp, Warning, TEXT("((%i / -2) + %i) * %f = %f"), Size, J, VertexDistance, UVY);
			UE_LOG(LogTemp, Warning, TEXT("  "));
			*/

			Vertices.Add(FVector(X, Y, Z));
			UV0.Add(FVector2D(UVX, UVY));
		}
	}
}


void AChunkGenerator::CreateTriangles(int XYSize){
	int Vertex = 0;

	for(int I = 0; I < XYSize; I++){
		for(int J = 0; J < XYSize; J++){
			Triangles.Add(Vertex);
			Triangles.Add(Vertex + 1);
			Triangles.Add(Vertex + XYSize + 1);
			Triangles.Add(Vertex + 1);
			Triangles.Add(Vertex + XYSize + 2);
			Triangles.Add(Vertex + XYSize + 1);

			Vertex++;
		}
		Vertex++;
	}
}

void AChunkGenerator::Generate()
{
	//Clear at the Beginning of function, cause performance issue when not clearing
	Vertices.Reset();
	Triangles.Reset();
	UV0.Reset();

	CreateVertices(Size);
	CreateTriangles(Size);

	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Vertices, Triangles, UV0, Normals /*TArray<FVector>()*/, Tangents /*TArray<FProcMeshTangent>()*/);

	ProceduralMeshComp->CreateMeshSection(
		0, //section Index
		Vertices,
		Triangles, 
		//TArray<FVector>(), 
		Normals, //Normals: parameters of default constructor
		UV0, //Texture Cordinates
		TArray<FColor>(),
		//TArray<FProcMeshTangent>(),
		Tangents, 
		EnableCollision); //enable mesh collision

		ProceduralMeshComp->SetMaterial(0, Material);
}

float AChunkGenerator::CalculateZ(int X, int Y)
{
	return FMath::PerlinNoise2D(FVector2D((X + 0.1 + XOffset) * NoiseScale,(Y + 0.1 + YOffset) * NoiseScale)) * ZMultiplier;
}

float AChunkGenerator::CalculateCordinates(float Distance, int XYIndex)
{
	//Example:
	//X = ((10 / -2) + 0) * 1.000000 = -5.000000
	//Y = ((10 / -2) + 1) * 1.000000 = -4.000000
	//UVX = ((10 / -2) + 0) * 1.000000 = -5.000000
	//UVY = ((10 / -2) + 1) * 1.000000 = -4.000000
	 
	return ((Size/-2.f)+XYIndex)*(int32)Distance;
}

void AChunkGenerator::CalculatePrimeNumbers()
{
	//ThreadTest::Generate2(Vertices, Triangles, UV0, Size, XOffset, YOffset, NoiseScale, ZMultiplier, VertexDistance, ProceduralMeshComp, Normals, Tangents, EnableCollision, Material);
	ThreadTest::CalculatePrimeNumbers(MaxPrime);

	GLog->Log("");
	GLog->Log("End of Calculating on game thread");
	GLog->Log("");
}

void AChunkGenerator::CalculatePrimeNumbersAsync()
{
	//(new FAutoDeleteAsyncTask<GenerateAsyncTask>(Vertices, Triangles, UV0, Size, XOffset, YOffset, NoiseScale, ZMultiplier, VertexDistance, ProceduralMeshComp, Normals, Tangents, EnableCollision, Material))->StartBackgroundTask();
	(new FAutoDeleteAsyncTask<CalculateAsyncTask>(MaxPrime))->StartBackgroundTask();

}