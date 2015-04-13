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
	virtual void init(const cv::Mat& leftImg, const cv::Mat& rightImg)=0;
	virtual float  operator()(const cv::Point point, const float disp)=0;

protected:
	const Params& params;
};

