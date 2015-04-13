#pragma once
#include "PlaneInit.h"

class RandomPlaneInit :
	public PlaneInit
{
public:
	RandomPlaneInit(const Params&);
	~RandomPlaneInit(void);

	void init(const cv::Mat& leftImg, const cv::Mat& rightImg, cv::Mat disp[], cv::Mat normals[]);
protected:
	void init(const cv::Mat& leftImg, const cv::Mat& rightImg, cv::Mat& disp, cv::Mat& normals);
};

