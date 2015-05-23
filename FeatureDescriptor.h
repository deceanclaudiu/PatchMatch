#pragma once
#include "opencv2\core.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\imgproc.hpp"
#include <cmath> 

#include "Params.h"

class FeatureDescriptor
{
public:
	FeatureDescriptor(const Params& params);
	virtual ~FeatureDescriptor(void);
	virtual void init(const cv::Mat& leftImg, const cv::Mat& rightImg);
	virtual void applyTransform(const cv::Mat& inputImg, cv::Mat& outputImg);
	virtual float  operator()(const cv::Point point, const float disp)=0;

protected:
	const Params& params;
	cv::Mat leftImg;
	cv::Mat rightImg;
};

