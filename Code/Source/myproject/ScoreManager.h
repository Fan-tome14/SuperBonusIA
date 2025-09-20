#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScoreManager.generated.h"

UCLASS()
class MYPROJECT_API AScoreManager : public AActor
{
	GENERATED_BODY()

public:
	AScoreManager();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// --- Score ---
	int32 TotalDropped = 0;
	int32 TotalCaught = 0;

	UPROPERTY(BlueprintReadOnly,VisibleAnywhere, Category = "resultat")float resultat = 0.0f;

	// Ajouter un cube dropp�
	void AddDropped() { TotalDropped++; }

	// Ajouter un cube attrap�
	void AddCaught() { TotalCaught++; }

	// Calculer le pourcentage de r�ussite
	float GetSuccessPercentage() const
	{
		if (TotalDropped == 0) return 0.0f;
		return (float)TotalCaught / (float)TotalDropped * 100.0f;
	}

	void ChangeHUD();
};
