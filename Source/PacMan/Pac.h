// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "Pac.generated.h"

UCLASS()
class PACMAN_API APac : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APac();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pac")
		class APac_GameMode* pGameMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pac")
		FVector MovementDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pac")
		int Action;

	UPROPERTY(EditAnywhere, Category = Score)
		int Score;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	
};
