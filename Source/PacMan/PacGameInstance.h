// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "ECagent.h"
#include "PacGameInstance.generated.h"

/**
 * 
 */

UCLASS()
class PACMAN_API UPacGameInstance : public UGameInstance
{
	GENERATED_BODY()
		
public:

	UPacGameInstance(const FObjectInitializer& ObjectInitializer);

	/** Increment this value in any map, change map, and notice it persists! */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Learning)
		int32 EpisodesNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Learning)
		int32 Action;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Learning)
		bool bLearningMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Learning)
		int32 TotalGameScore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Learning)
		bool isEpisodeEnded;

	ECagent agent;
	int32 EndedEpisodeScore;

	
};
