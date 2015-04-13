#pragma once

#include "featuredescriptor.h"
#include <intrin.h> 

class GradientCost :
	public FeatureDescriptor
{
public:
	GradientCost(const Params& params);
	~GradientCost(void);

	void init(const cv::Mat& leftImg, const cv::Mat& rightImg);
	float  operator()(const cv::Point point, const float disp);
	float  operator()(int row, int col, const float disp);
private:
	void applyTransform(const cv::Mat& img, cv::Mat& resultImg);
	uint sad(ushort a, ushort b);
	cv::Mat leftDscrImg;
	cv::Mat rightDscrImg;
};

