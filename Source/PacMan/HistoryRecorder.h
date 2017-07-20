// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "opencv2/core/core.hpp"
#include "opencv2/core/mat.hpp"
#include "opencv2/highgui.hpp"	
#include "opencv2/imgproc.hpp"
#include <vector>


/**
 * 
 not observations but states are now stored!
 */
class PACMAN_API HistoryNode
{
public:
	cv::Mat observation;
	cv::Mat state;
	int action;
	int reward;
	bool bTerminal;
	HistoryNode(cv::Mat obs, int a, int r, bool bT, cv::Mat s = cv::Mat(0, 0, CV_32F)) :
		observation(obs), action(a), reward(r), bTerminal(bT), state(s)
	{};

	~HistoryNode()
	{};
};

class PACMAN_API HistoryRecorder
{
public:
	std::vector<HistoryNode> history;
	HistoryRecorder();
	~HistoryRecorder();
	void add_node(cv::Mat obs, int a, int r, bool bT, cv::Mat s = cv::Mat(0, 0, CV_32F))
	{
		HistoryNode node = HistoryNode(obs, a, r, bT, s);
		history.push_back(node);
	};
};

