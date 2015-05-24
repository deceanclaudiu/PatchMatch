#pragma once
#include "opencv2\core.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\imgproc.hpp"
#include <cmath> 

#include "Params.h"
#include "MultiViewMatcher.h"

class FeatureDescriptor
{
public:
	FeatureDescriptor(const Params& params);
	virtual ~FeatureDescriptor(void);
	virtual void init(MultiViewMatcher& mvm, const cv::Mat& leftImg, const cv::Mat& rightImg);
	virtual void applyTransform(const cv::Mat& inputImg, cv::Mat& outputImg);
	float  operator()(const cv::Point2f point, const float disp){return 0;};
	float compute(const cv::Mat& img1, const cv::Mat& img2, const cv::Point2f point1, const cv::Point2f point2){return 0;};
protected:
	MultiViewMatcher * mvm;
	int frameCnt;
	const Params& params;
	cv::Mat leftImg, rightImg;
	cv::Mat prevLeftImg, prevRightImg;
};

