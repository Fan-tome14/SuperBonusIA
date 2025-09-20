#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Drop.generated.h"


UCLASS()
class MYPROJECT_API ADrop : public AActor
{
	GENERATED_BODY()

public:
	ADrop();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:

	class AScoreManager* ScoreManager; // référence au manager

	// --- Mouvement ---
	float TargetY;
	float Speed;
	bool IsWaiting=false;

	void NewTarget();
	FVector Seek(const FVector& Target, float DeltaTime);

	// --- Drop cubes ---
	UPROPERTY(EditAnywhere, Category = "Drop")
	TSubclassOf<AActor> CubeToSpawn; // cube � spawn

	FTimerHandle DropTimer;
	void DropCube();
	void EndWait();
};
