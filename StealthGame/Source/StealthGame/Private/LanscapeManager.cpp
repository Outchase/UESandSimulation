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

void ALanscapeManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//calculate Chunk Position
	CurrentChunkPosition = FVector2D(
		UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation().X/Scale,
		UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation().Y/Scale
	);
	
	SetVisible();

	if(!Generated.Contains(Visible)){
		ActorChunk = GetWorld()->SpawnActor<AChunkGenerator>(ChunkClass, FVector((int32)Visible.X*Scale, (int32)Visible.Y*Scale, 0.0f), FRotator(0.0f, 0.0f, 0.0f));
		SetChunkParameters(ActorChunk);
		ActorChunk->Generate();
		Generated.Add(Visible);
		if(bTestMultiThreding){
			if(bIsAsync){
				ActorChunk->CalculatePrimeNumbersAsync();
			} else {
				ActorChunk->CalculatePrimeNumbers();
			}
		}
	}

}

void ALanscapeManager::SetChunkParameters(AChunkGenerator* Chunk)
{
	Chunk->Size = Size;
	Chunk->VertexDistance = Scale/Size;
	Chunk->UVScale = UVScale;
	Chunk->ZMultiplier = ZMultiplier;
	Chunk->Material = Material;
	Chunk->NoiseScale = NoiseScale;
	Chunk->EnableCollision = EnableCollision;
	Chunk->XOffset = Size*(int32)Visible.X;
	Chunk->YOffset = Size*(int32)Visible.Y;
	Chunk->MaxPrime = MaxPrime;
}

void ALanscapeManager::SetVisible()
{
	for(int I = Distance*-1; I < Distance; I++){
		Visible.X= (int32)CurrentChunkPosition.X+I;
		Visible.Y= (int32)CurrentChunkPosition.Y+I; 
	}
}
