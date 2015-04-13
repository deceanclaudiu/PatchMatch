#pragma once
#include "opencv2\core.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "Params.h"

class PlaneInit
{
public:
	PlaneInit(const Params&);
	~PlaneInit(void);
	virtual void init(const cv::Mat& leftImg, const cv::Mat& rightImg, cv::Mat disp[], cv::Mat normals[])=0;
protected:
	const Params& params;
};

