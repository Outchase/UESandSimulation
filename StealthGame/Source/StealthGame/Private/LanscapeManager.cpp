#include "LanscapeManager.h"
#include "ChunkGenerator.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALanscapeManager::ALanscapeManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ALanscapeManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ALanscapeManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//calculate Chunk Position
	CurrentChunkPosition = FVector2D(
		UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation().X/Scale,
		UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation().Y/Scale
	);
	
	SetVisible();

	//UE_LOG(LogTemp, Warning, TEXT("Visible: %s"), *Visible.ToString());
	/*
	UE_LOG(LogTemp, Warning, TEXT("Player pos: %s"), *CurrentChunkPosition.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Player X: %f"), CurrentChunkPosition.X);
	UE_LOG(LogTemp, Warning, TEXT("Player Y: %f"), CurrentChunkPosition.Y);
	*/

	if(!Generated.Contains(Visible)){
		ActorChunk = GetWorld()->SpawnActor<AChunkGenerator>(ChunkClass, FVector((int32)Visible.X*Scale, (int32)Visible.Y*Scale, 0.0f), FRotator(0.0f, 0.0f, 0.0f));
		SetChunkParameters(ActorChunk);
		ActorChunk->Generate();
		Generated.Add(Visible);
	}

}


void ALanscapeManager::SetChunkParameters(AChunkGenerator* Chunk)
{

	Chunk->Jitter = Jitter;

	Chunk->Seed = Seed; 
	Chunk->Frequency = Frequency;
	Chunk->Octaves = Octaves;
	Chunk->Lacunarity = Lacunarity;
	Chunk->Gain = Gain;

	Chunk->Size = Size;
	Chunk->VertexDistance = Scale/Size;
	Chunk->UVScale = UVScale;
	Chunk->ZMultiplier = ZMultiplier;
	Chunk->Material = Material;
	Chunk->NoiseScale = NoiseScale;
	Chunk->EnableCollision = EnableCollision;
	Chunk->XOffset = Size*(int32)Visible.X;
	Chunk->YOffset = Size*(int32)Visible.Y;


	//UE_LOG(LogTemp, Warning, TEXT("Actor %s"), *Chunk->GetName());
}

void ALanscapeManager::SetVisible()
{
	for(int I = Distance*-1; I < Distance; I++){
		Visible.X= (int32)CurrentChunkPosition.X+I;
		Visible.Y= (int32)CurrentChunkPosition.Y+I;
	}
}


