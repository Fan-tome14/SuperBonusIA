#include "ScoreManager.h"
#include "Engine/Engine.h"

AScoreManager::AScoreManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AScoreManager::BeginPlay()
{
	Super::BeginPlay();
}


void AScoreManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AScoreManager::ChangeHUD() {

	resultat = GetSuccessPercentage();
}

