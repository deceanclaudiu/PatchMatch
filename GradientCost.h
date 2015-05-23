#pragma once

#include "featuredescriptor.h"
#include <intrin.h> 

class GradientCost :
	public FeatureDescriptor
{
public:
	GradientCost(const Params& params);
	~GradientCost(void);

	float  operator()(const cv::Point point, const float disp);
	float  operator()(int row, int col, const float disp);
private:
	void applyTransform(const cv::Mat& inputImg, cv::Mat& outputImg);
	uint sad(ushort a, ushort b);
};

