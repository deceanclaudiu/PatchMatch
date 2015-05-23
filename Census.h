#pragma once

#include "featuredescriptor.h"
#include <intrin.h> 

class Census :
	public FeatureDescriptor
{
public:
	Census(const Params& params);
	~Census(void);

	float  operator()(const cv::Point point, const float disp);
	float  operator()(int row, int col, const float disp);
private:
	uint popcount(unsigned int x);
	void applyTransform(const cv::Mat& inputImg, cv::Mat& outputImg);
	uint hamming(ushort a, ushort b);
};

