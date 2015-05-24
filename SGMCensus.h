#pragma once
#include "FeatureDescriptor.h"
#include "StereoSGM.h"
#include "rSGMCmd.h"

class SGMCensus:
	public FeatureDescriptor
{
public:
	SGMCensus(const Params& params);
	~SGMCensus(void);
	float  operator()(const cv::Point2f point, const float disp);
	float  operator()(int row, int col, const float disp);
	virtual void init(MultiViewMatcher& mvm, const cv::Mat& leftImg, const cv::Mat& rightImg);
private:
	int * data;
};

