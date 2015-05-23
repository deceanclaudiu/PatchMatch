#pragma once

#include "featuredescriptor.h"
#include "Census.h"
#include "IntensityCost.h"
#include "GradientCost.h"

#include <intrin.h> 

class CombineCost :
	public FeatureDescriptor
{
public:
	CombineCost(const Params& params);
	~CombineCost(void);

	void init(const cv::Mat& leftImg, const cv::Mat& rightImg);
	float  operator()(const cv::Point point, const float disp);
	float  operator()(int row, int col, const float disp);
private:
	Census censusCost;
	IntensityCost intensityCost;
	GradientCost gradientCost;

	static const float alfa, beta;
};

