#pragma once

#include "featuredescriptor.h"
#include <intrin.h> 

class Census :
	public FeatureDescriptor
{
public:
	Census(const Params& params);
	~Census(void);

	void init(const cv::Mat& leftImg, const cv::Mat& rightImg);
	float  operator()(const cv::Point point, const float disp);
	float  operator()(int row, int col, const float disp);
private:
	uint popcount(unsigned int x);
	void applyCensusTransform(const cv::Mat& img, cv::Mat& censusImg);
	uint hamming(ushort a, ushort b);
	cv::Mat leftCensusImg;
	cv::Mat rightCensusImg;
};

