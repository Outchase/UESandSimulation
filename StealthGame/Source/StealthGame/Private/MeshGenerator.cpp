
#include "MeshGenerator.h"
#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"

AMeshGenerator::AMeshGenerator()
{
	PrimaryActorTick.bCanEverTick = false;

	ProceduralMeshComp = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
	ProceduralMeshComp->SetupAttachment(GetRootComponent());

	//SuperMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Super Mesh Component"));
}

/*void AMeshGenerator::OnConstruction(const FTransform &Transform)
{
	Super::OnConstruction(Transform);

	//Clear at the Beginning of function, cause performance issue when not clearing
	Vertices.Reset();
	Triangles.Reset();
	UV0.Reset();

	CreateVertices();
	CreateTriangles();

	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Vertices, Triangles, UV0, Normals, Tangents);
	//UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Vertices, Triangles, UV0, TArray<FVector>(), TArray<FProcMeshTangent>());

		ProceduralMeshComp->CreateMeshSection(
		0, //section Index
		Vertices,
		Triangles, 
		Normals, //Normals: parameters of default  constructor
		UV0, //Texture Cordinates
		TArray<FColor>(),
		Tangents, 
		EnableCollision); //enable mesh collision

		/*ProceduralMeshComp->CreateMeshSection(
		0, //section Index
		Vertices,
		Triangles, 
		TArray<FVector>(),  //Normals: parameters of default  constructor
		UV0, //Texture Cordinates
		TArray<FColor>(),
		TArray<FProcMeshTangent>(),
		EnableCollision); //enable mesh collision*/

		//ProceduralMeshComp->SetMaterial(0, SandMaterial);
//}


void AMeshGenerator::BeginPlay()
{
	Super::BeginPlay();

	//Clear at the Beginning of function, cause performance issue when not clearing
	Vertices.Reset();
	Triangles.Reset();
	UV0.Reset();

	CreateVertices();
	CreateTriangles();

	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Vertices, Triangles, UV0, Normals /*TArray<FVector>()*/, Tangents /*TArray<FProcMeshTangent>()*/);

	ProceduralMeshComp->CreateMeshSection(
		0, //section Index
		Vertices,
		Triangles, 
		//TArray<FVector>(), 
		Normals, //Normals: parameters of default  constructor
		UV0, //Texture Cordinates
		TArray<FColor>(),
		//TArray<FProcMeshTangent>(),
		Tangents, 
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