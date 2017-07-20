// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "Pac_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class PACMAN_API APac_GameMode : public AGameMode
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "scoring")
	int Player_Score;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "screenshots")
		class AScreenCapturer* ScreenCapturer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "game")
	bool isVAE;
	
};
