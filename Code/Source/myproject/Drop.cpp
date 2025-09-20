#include "Drop.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Components/PrimitiveComponent.h"
#include "Math/UnrealMathUtility.h"
#include "ScoreManager.h"
#include "EngineUtils.h"

ADrop::ADrop()
{
	PrimaryActorTick.bCanEverTick = true;
	Speed = 800.0f; // vitesse
}

void ADrop::BeginPlay()
{
	Super::BeginPlay();

	NewTarget();      // première position Y

	// trouver le ScoreManager présent dans la scène
	for (TActorIterator<AScoreManager> It(GetWorld()); It; ++It)
	{
		ScoreManager = *It;
		break;
	}

}

void ADrop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsWaiting)return;
	FVector TargetPos = GetActorLocation();
	TargetPos.Y = TargetY; // se déplace uniquement sur Y

	FVector NewPos = Seek(TargetPos, DeltaTime);
	SetActorLocation(NewPos);

	// Si proche de la cible → nouvelle position
	if (!IsWaiting&&FMath::Abs(NewPos.Y - TargetY) < 10.0f)
	{
		IsWaiting = true;
		DropCube();
		GetWorld()->GetTimerManager().SetTimer(DropTimer, this, &ADrop::EndWait, 2.0f, false);
	}
}

void ADrop::EndWait() {
	IsWaiting = false;
	NewTarget();
}

// --- Seek ---
FVector ADrop::Seek(const FVector& Target, float DeltaTime)
{
	FVector Pos = GetActorLocation();
	FVector Desired = Target - Pos;
	Desired.Normalize();
	FVector Velocity = Desired * Speed * DeltaTime;
	return Pos + Velocity;
}

// --- Nouvelle cible Y ---
void ADrop::NewTarget()
{
	TargetY = FMath::RandRange(-1050.0f, 1060.0f);

	// Vérifier la direction du déplacement
	float CurrentY = GetActorLocation().Y;
	if (TargetY > CurrentY)
	{
		// Va vers la droite → rotation Z = 0
		SetActorRotation(FRotator(0.0f, 0.0f, 0.0f)); // <<< ajouté
	}
	else
	{
		// Va vers la gauche → rotation Z = 180
		SetActorRotation(FRotator(0.0f, 180.0f, 0.0f)); // <<< ajouté
	}
}

// --- Faire tomber un cube ---
void ADrop::DropCube()
{
	if (CubeToSpawn)
	{
		FVector SpawnLocation = GetActorLocation();
		SpawnLocation.Z += -150.0f;
		FRotator SpawnRotation = FRotator::ZeroRotator;

		AActor* Spawned = GetWorld()->SpawnActor<AActor>(CubeToSpawn, SpawnLocation, SpawnRotation);

		// activer physique si possible
		if (Spawned)
		{
			UPrimitiveComponent* Comp = Cast<UPrimitiveComponent>(Spawned->GetComponentByClass(UPrimitiveComponent::StaticClass()));
			if (Comp)
			{
				Comp->SetSimulatePhysics(true);
			}
		}
	}

	if (ScoreManager)
	{
		
		ScoreManager->AddDropped();
		ScoreManager->ChangeHUD();
		
	}
}
