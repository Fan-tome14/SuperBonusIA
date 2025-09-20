#include "Attrapetout.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "ScoreManager.h"
#include "EngineUtils.h"


AAttrapetout::AAttrapetout()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAttrapetout::BeginPlay()
{
	Super::BeginPlay();

	// trouver le ScoreManager présent dans la scène
	for (TActorIterator<AScoreManager> It(GetWorld()); It; ++It)
	{
		ScoreManager = *It;
		break;
	}

	// activer détection overlap
	OnActorBeginOverlap.AddDynamic(this, &AAttrapetout::OnOverlapBegin);

}

void AAttrapetout::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Cherche le cube le plus proche
	AActor* TargetCube = FindClosestCube();

	// Se d�placer vers ce cube
	if (TargetCube)
	{
		MoveTowardsTarget(DeltaTime, TargetCube);
	}
}

// --- Cherche le cube BP_Cube le plus proche ---
AActor* AAttrapetout::FindClosestCube()
{
	TArray<AActor*> FoundCubes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), CubeToSpawnClass, FoundCubes);

	if (FoundCubes.Num() == 0) return nullptr;

	AActor* Closest = nullptr;
	float MinDist = FLT_MAX;
	FVector MyPos = GetActorLocation();

	for (AActor* Cube : FoundCubes)
	{
		float Dist = FMath::Abs(Cube->GetActorLocation().Y - MyPos.Y); // distance sur Y uniquement
		if (Dist < MinDist)
		{
			MinDist = Dist;
			Closest = Cube;
		}
	}

	return Closest;
}

// --- D�placer l'actor uniquement sur Y vers le cube cible ---
void AAttrapetout::MoveTowardsTarget(float DeltaTime, AActor* TargetCube)
{
	FVector MyPos = GetActorLocation();
	FVector TargetPos = TargetCube->GetActorLocation();

	float DirY = TargetPos.Y - MyPos.Y;
	float MoveY = FMath::Clamp(DirY, -Speed * DeltaTime, Speed * DeltaTime);

	MyPos.Y += MoveY;
	SetActorLocation(MyPos);
}

void AAttrapetout::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor && OtherActor->IsA(CubeToSpawnClass))
	{
		if (ScoreManager)
		{
			ScoreManager->AddCaught();
			ScoreManager->ChangeHUD();
			
		}

		// Détruire le cube attrapé
		OtherActor->Destroy();
	}
}
