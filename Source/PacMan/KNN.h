#pragma once


#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>

#include <opencv2/flann/flann.hpp>
#include <ctime>
#include <vector>
#include <algorithm>
#include <memory>
#include <vector>
#include <string>

#pragma warning( disable : 4103)
#include <boost/regex.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include "cvMat_serialization.h"

#include <iostream> 
#include <fstream> 


/**
*
*/
class  PACMAN_API KNN
{
private:



	int capacity;
	cv::Mat states;
	std::vector<float> q_values;
	std::vector<float> LRUs;
	int current_capacity;
	float time;


	cv::flann::Index kdtree;

	//binary achive
	friend class boost::serialization::access;
	template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{
		ar & capacity;
		ar & current_capacity;
		ar & q_values;
		ar & LRUs;
		ar & time;
		ar & states;
	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		ar & capacity;
		ar & current_capacity;
		ar & q_values;
		ar & LRUs;
		ar & time;
		ar & states;
	}
	BOOST_SERIALIZATION_SPLIT_MEMBER()


public:

	float knn_value(cv::Mat key, int knn);
	void add(cv::Mat key, float value);
	bool peek(cv::Mat key, float value, bool bModify, float* result_qval);

	void saveIndex(std::string filename) const;
	void loadIndex(std::string filename);

	KNN();
	KNN(int capacity, int dim);
	~KNN();
};

