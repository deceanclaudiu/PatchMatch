#pragma once

#include "featuredescriptor.h"
#include <intrin.h> 

class GradientCost :
	public FeatureDescriptor
{
public:
	GradientCost(const Params& params);
	~GradientCost(void);

	void init(const Mat& leftImg, const Mat& rightImg);
	float  operator()(const Point point, const float disp);
	float  operator()(int row, int col, const float disp);
private:
	void applyTransform(const Mat& img, Mat& resultImg);
	uint sad(ushort a, ushort b);
	Mat leftDscrImg;
	Mat rightDscrImg;
};

