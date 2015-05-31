#pragma once

#include "featuredescriptor.h"
#include "Census.h"
#include "IntensityCost.h"
#include "GradientCost.h"
#include "SGMCensus.h"
#include <intrin.h> 

class CombineCost :
	public FeatureDescriptor
{
public:
	CombineCost(const Params& params);
	~CombineCost(void);

	void init(MultiViewMatcher& mvm, const cv::Mat& leftImg, const cv::Mat& rightImg);
	float  operator()(const cv::Point2f point, const float disp);
	float  operator()(int row, int col, const float disp);
private:
	Census censusCost;
	IntensityCost intensityCost;
	GradientCost gradientCost;
	SGMCensus sgmCensusCost;
	static const float alfa, beta;
};

