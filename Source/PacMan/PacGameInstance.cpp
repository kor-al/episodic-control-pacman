// Fill out your copyright notice in the Description page of Project Settings.

#include "PacMan.h"
#include "PacGameInstance.h"

UPacGameInstance::UPacGameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	EndedEpisodeScore = 0;
}

