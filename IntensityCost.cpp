#include "IntensityCost.h"

IntensityCost::IntensityCost(const Params& params):
	FeatureDescriptor(params)
{
}


IntensityCost::~IntensityCost(void)
{
}

void IntensityCost::init(const cv::Mat& leftImg, const cv::Mat& rightImg)
{
	leftDscrImg = leftImg;
	rightDscrImg = rightImg;
}

float  IntensityCost::operator()(int row, int col, const float disp)
{
	return (*this)(cv::Point(col, row), disp);
}

float  IntensityCost::operator()(const cv::Point point, const float disp)
{
	double intPart;
	double frac = modf((double)disp, &intPart);
	if((point.x - disp < 0)||(point.x - disp >= (leftDscrImg.cols - 1))) return 0xFFF;
	uint left = leftDscrImg.at<uchar>(point);
	uint right = frac * rightDscrImg.at<uchar>(point.y, point.x - disp) + (1 - frac) * rightDscrImg.at<uchar>(point.y, point.x - disp + 1);
	int cost = sad(left, right);
	return cost;
}

uint IntensityCost::sad(ushort a, ushort b)
{
	return abs(a - b);
}
