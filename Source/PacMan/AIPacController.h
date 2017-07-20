// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "Pac.h"
#include "Pac_GameMode.h"
#include "PacGameInstance.h"
#include "ScreenCapturer.h"
#include <opencv2/core.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include <opencv2/highgui/highgui.hpp>
#include "AIPacController.generated.h"


UCLASS()
class PACMAN_API AAIPacController : public AAIController
{
	GENERATED_BODY()
public:

	static const int kFrameSkip = 4; 
	static const int kTransformedImageDim = 84;
	static const int kSizeOfTheMap = 620;
	static const std::string kSummaryFilename;

	bool bEmbedding; //0 == random projection, 1 == VAE

	// Sets default values for this actor's properties
	AAIPacController();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	APac_GameMode* game_mode;

	//UPROPERTY(EditAnywhere, Category = PawnRef)
	//class APac* pac;

	APac* pac;

	UPacGameInstance* game_inst;

	cv::Mat get_screen(class APac_GameMode*game_mode);

	float sign(int x);

	int step_count; // to skip frames
					//to save observation with number and stop learning phase

	int last_action;

	int current_score; //episode score

	int action; //is changed by game_inst


	int get_score(class APac_GameMode* game_mode);

	FVector get_action_direction(int action);

	cv::Mat transform_image(cv::Mat screen);

	bool bFirstTick;

	//to merge every kFrameSkip frames
	int prev_action;
	cv::Mat accum_obs;
	void cvMat2tarray(cv::Mat mat, TArray<uint8>& a);
	void assign_accum_screen(class APac_GameMode*game_mode, cv::Mat mat);

	cv::Mat tarray2cvmat(TArray<float> a);

	void update_summary();
	
};
