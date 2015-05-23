#include "IntensityCost.h"

IntensityCost::IntensityCost(const Params& params):
	FeatureDescriptor(params)
{
}


IntensityCost::~IntensityCost(void)
{
}


float  IntensityCost::operator()(int row, int col, const float disp)
{
	return (*this)(cv::Point(col, row), disp);
}

float  IntensityCost::operator()(const cv::Point point, const float disp)
{
	double intPart;
	double frac = modf((double)disp, &intPart);
	if((point.x - disp < 0)||(point.x - disp >= (leftImg.cols - 1))||(point.x < 0)||(point.x >= (leftImg.cols - 1))) return 255.0;
	uint left = leftImg.at<uchar>(point);
	uint right = frac * rightImg.at<uchar>(point.y, point.x - disp) + (1 - frac) * rightImg.at<uchar>(point.y, point.x - disp + 1);
	int cost = sad(left, right);
	return cost;
}

uint IntensityCost::sad(ushort a, ushort b)
{
	return abs(a - b);
}
