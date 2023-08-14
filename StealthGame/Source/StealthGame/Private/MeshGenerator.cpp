
#include "MeshGenerator.h"
#include "ProceduralMeshComponent.h"

AMeshGenerator::AMeshGenerator()
{
	PrimaryActorTick.bCanEverTick = false;

	ProceduralMeshComp = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
	ProceduralMeshComp->SetupAttachment(GetRootComponent());

	//SuperMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Super Mesh Component"));
}

void AMeshGenerator::BeginPlay()
{
	Super::BeginPlay();

	CreateVertices();
	CreateTriangles();

	ProceduralMeshComp->CreateMeshSection(
		0, //section Index
		Vertices,
		Triangles, 
		TArray<FVector>(), //Normals: parameters of default  constructor
		UV0, //Texture Cordinates
		TArray<FColor>(), 
		TArray<FProcMeshTangent>(), 
		EnableCollision); //enable mesh collision

		ProceduralMeshComp->SetMaterial(0, SandMaterial);
}

// Called every frame
void AMeshGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMeshGenerator::CreateVertices(){
	for(int I = 0; I <= XSize; I++){
		for(int J = 0; J <= YSize; J++){
			float Z= FMath::PerlinNoise2D(FVector2D(I * NoiseScale + 0.1 ,J * NoiseScale + 0.1)) * ZMultiplier;
			//GEngine->AddOnScreenDebugMessage(-1, 999.f, FColor::Yellow, FString::Printf(TEXT("Z %f"), Z));
			Vertices.Add(FVector(I * Scale, J * Scale, Z));
			UV0.Add(FVector2D(I * UVScale, J * UVScale));
		}
	}
}

void AMeshGenerator::CreateTriangles(){
	int Vertex = 0;

	for(int I = 0; I < XSize; I++){
		for(int J = 0; J < YSize; J++){
			Triangles.Add(Vertex);
			Triangles.Add(Vertex + 1);
			Triangles.Add(Vertex + YSize + 1);
			Triangles.Add(Vertex + 1);
			Triangles.Add(Vertex + YSize + 2);
			Triangles.Add(Vertex + YSize + 1);

			Vertex++;
		}
		Vertex++;
	}
}