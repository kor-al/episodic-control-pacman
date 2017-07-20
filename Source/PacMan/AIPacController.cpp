// Fill out your copyright notice in the Description page of Project Settings.

#include "PacMan.h"
#include "AIPacController.h"

const std::string AAIPacController::kSummaryFilename = "pac_summary.txt";

AAIPacController::AAIPacController()
{
	PrimaryActorTick.bCanEverTick = true;
	UE_LOG(LogTemp, Warning, TEXT("AI Pac controller Constructor"));
	bFirstTick = true;
}

void  AAIPacController::BeginPlay()
{

	Super::BeginPlay();
	
	game_mode = (APac_GameMode*)GetWorld()->GetAuthGameMode();
	game_inst = (UPacGameInstance*)GetGameInstance();
	bEmbedding = game_mode->isVAE;

	cv::setBreakOnError(true);
	
	step_count = 0;
	UE_LOG(LogTemp, Warning, TEXT("AI Pac controller BeginPlay"));

	//agent.loadQECtable();
}

void AAIPacController::Tick(float DeltaSeconds)
{
	//
	// reward is 0, 1 or 200 if pacman dies ---> game_inst->isEpisodeEnded == true
	//

	Super::Tick(DeltaSeconds);

	pac = (APac*)GetControlledPawn();
	if (!pac)
	{
		return;
	}

	int sc = get_score(game_mode);
	int reward = sc - current_score;
	cv::Mat screen = get_screen(game_mode);
	//cv::imwrite("D:/Alice/Documents/HSE/masters/pac_observations/" + std::to_string(step_count) + ".png", screen);


	bool *bLearningMode = &game_inst->bLearningMode;

	//skip empty frames
	if (screen.cols == 0)
		return;
	//--------------------------------------------
	
	step_count++;
	cv::Mat screen_t = transform_image(screen);
	//cv::imwrite("D:/Alice/Documents/HSE/masters/pac_observations/t" + std::to_string(step_count) + ".png", screen_t);
	UE_LOG(LogTemp, Warning, TEXT("____reward = %i"), reward);
	
	game_inst->EndedEpisodeScore += reward;

	if (!*bLearningMode && game_inst->isEpisodeEnded)
	{

		(game_inst->agent).end_episode(reward);
		UE_LOG(LogTemp, Warning, TEXT("END of EPISODE # %i"), game_inst->EpisodesNum-1);
		bFirstTick = true;
		step_count = 0;
		game_inst->isEpisodeEnded = false;
		//print progress : the episode number and gained player score 
		update_summary();
		//as episode score is printed -> reset counter
		game_inst->EndedEpisodeScore = 0;
		return;
	}

	if (step_count%kFrameSkip) //add skipped frames into one frame to process (while the last action is repeated)
	{
		if (step_count == 1)
		{
			game_mode->ScreenCapturer->MergedScreenshot.Empty();
			screen_t.copyTo(accum_obs);
		}
		else
		{
			cv::addWeighted(screen_t, 1, accum_obs, 1, 0, accum_obs);
		}
		action = prev_action;
	}
	else
	{
		step_count = 0;

	//	//cv::imwrite("D:/Alice/Documents/HSE/masters/observations/" + std::to_string(total_frames_count) + ".png", accum_obs);
		assign_accum_screen(game_mode, accum_obs);


		if (*bLearningMode)
		{
			action = (game_inst->agent).random_action();
		}
		else
		{
			cv::Mat state = cv::Mat(0, 0, CV_32F);

			if (bEmbedding)
			{
				state = tarray2cvmat(game_mode->ScreenCapturer->State);
			}

			if (bFirstTick)
			{
				UE_LOG(LogTemp, Warning, TEXT("START of EPISODE # %i"), game_inst->EpisodesNum);
				action = (game_inst->agent).start_episode(accum_obs, state);
				bFirstTick = false;
			}
			else
			{

				action = (game_inst->agent).step(accum_obs, reward, state);
			}
		}

	}

	action = (game_inst->agent).random_action();
	UE_LOG(LogTemp, Warning, TEXT("____action = %i"), action);

	pac->Action = action;
	
	current_score = sc;
	prev_action = action;

}

cv::Mat AAIPacController::get_screen(class APac_GameMode* game_mode)
{
	int H = game_mode->ScreenCapturer->Height;
	int W = game_mode->ScreenCapturer->Width;
	TArray<uint8> data = game_mode->ScreenCapturer->Screenshot;
	uint8* values = data.GetData();
	cv::Mat chan[3] = {
		cv::Mat(W,H,CV_8U, values),
		cv::Mat(W,H,CV_8U, values + H*W),
		cv::Mat(W,H,CV_8U, values + 2 * H*W)
	};
	cv::Mat merged;
	cv::merge(chan, 3, merged);
	return merged;
}




float AAIPacController::sign(int x)
{
	if (x > 0) return 1.0;
	if (x < 0) return -1.0;
	return 0.0;
}


cv::Mat AAIPacController::tarray2cvmat(TArray<float> a)
{
	float* values = a.GetData();
	return cv::Mat(1, a.Num(), CV_32FC1, values).clone();
}

void AAIPacController::assign_accum_screen(class APac_GameMode*game_mode, cv::Mat mat)
{
	cvMat2tarray(mat, game_mode->ScreenCapturer->MergedScreenshot);
}

int AAIPacController::get_score(class APac_GameMode*game_mode)
{
	if (!game_mode)	return -1;
	else return game_mode->Player_Score;
}

FVector AAIPacController::get_action_direction(int action)
{
	FVector direction;
	direction.Z = 0.f;
	if (action == 1)
	{
		direction.X = 1.f;
		direction.Y = 0.f;
	}
	else 
		if (action == 2)
		{
			direction.X = 0.f;
			direction.Y = -1.f;
		}
		else
			if (action == 3)
			{
				direction.X = -1.f;
				direction.Y = 0.f;
			}
			else
				if (action == 4)
				{
					direction.X = 0.f;
					direction.Y = 1.f;
				}
				else
				{
					direction.X = 0.f;
					direction.Y = 0.f;
				}

	return direction;
}

cv::Mat AAIPacController::transform_image(cv::Mat screen)
{

	cvtColor(screen, screen, CV_BGR2GRAY);
	//crop the screenshot
	cv::Rect myROI(screen.cols / 2 - kSizeOfTheMap / 2, screen.rows / 2 - kSizeOfTheMap / 2, kSizeOfTheMap, kSizeOfTheMap);
	cv::Mat croppedRef(screen, myROI);
	// Copy the data
	croppedRef.copyTo(screen);

	cv::threshold(screen, screen, 1, 255, cv::THRESH_BINARY);

	cv::Mat kernel;   //the default structuring element (kernel) for max filter (dilate)
	cv::dilate(screen, screen, kernel);
	cv::threshold(screen, screen, 1, 255, cv::THRESH_BINARY);
	////cv::Mat dst(newH, newW, CV_8U);
	cv::resize(screen, screen, cv::Size(kTransformedImageDim, kTransformedImageDim), cv::INTER_AREA);
	//cv::threshold(screen, screen, 1, 255, cv::THRESH_BINARY);
	cv::normalize(screen, screen, 1, 0, cv::NORM_MINMAX);// [0,1] in matrix , if commented [0,255]
	screen.convertTo(screen, CV_32FC1);
	return screen;
}

void AAIPacController::cvMat2tarray(cv::Mat mat, TArray<uint8>& a)
{
	a.Empty();
	mat.convertTo(mat, CV_8UC1);
	for (int i = 0; i < mat.rows; ++i) 
	{
		a.Append(mat.ptr<uint8>(i), mat.cols);
	}

}

void AAIPacController::update_summary()
{

	std::ofstream file;
	
	file.open(kSummaryFilename, std::ios_base::app | std::ios_base::out);
	if (file.fail())
		throw std::ios_base::failure(std::strerror(errno));

	file.exceptions(file.exceptions() | std::ios::failbit | std::ifstream::badbit);

	file << game_inst->EpisodesNum-1 << " "<<game_inst->EndedEpisodeScore << std::endl;

}
