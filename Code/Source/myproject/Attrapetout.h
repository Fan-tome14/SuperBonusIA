// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Attrapetout.generated.h"

UCLASS()
class MYPROJECT_API AAttrapetout : public AActor
{
	GENERATED_BODY()

public:
	AAttrapetout();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

private:
	class AScoreManager* ScoreManager;

	UPROPERTY(EditAnywhere, Category = "AI")
	float Speed = 875.0f; // vitesse de d�placement

	// Classe des cubes � suivre
	UPROPERTY(EditAnywhere, Category = "AI")
	TSubclassOf<AActor> CubeToSpawnClass;

	// Cherche le cube le plus proche
	AActor* FindClosestCube();

	void MoveTowardsTarget(float DeltaTime, AActor* TargetCube);
};

