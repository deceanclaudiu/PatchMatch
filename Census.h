#pragma once

#include "featuredescriptor.h"
#include <intrin.h> 

class Census :
	public FeatureDescriptor
{
public:
	Census(const Params& params);
	~Census(void);
	float compute(const cv::Mat& img1, const cv::Mat& img2, const cv::Point2f point1, const cv::Point2f point2);
	float  operator()(const cv::Point2f point, const float disp);
	float  operator()(int row, int col, const float disp);
private:
	uint popcount(unsigned int x);
	void applyTransform(const cv::Mat& inputImg, cv::Mat& outputImg);
	uint hamming(ushort a, ushort b);
};

