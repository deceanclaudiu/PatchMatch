#pragma once
#include "planeinit.h"

#include "rSGMCmd.h"

class SGMPlaneInit :
	public PlaneInit
{
public:
	SGMPlaneInit(const Params&);
	~SGMPlaneInit(void);

	void init(const cv::Mat& leftImg, const cv::Mat& rightImg, cv::Mat disp[], cv::Mat normals[]);
};

