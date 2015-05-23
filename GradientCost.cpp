#include "GradientCost.h"

GradientCost::GradientCost(const Params& params):
	FeatureDescriptor(params)
{
}


GradientCost::~GradientCost(void)
{
}


void GradientCost::applyTransform(const cv::Mat& inputImg, cv::Mat& outputImg)
{
	cv::Mat xDerv, yDerv;
	cv::Scharr(inputImg, xDerv, CV_32F, 1, 0);
	cv::Scharr(inputImg, yDerv, CV_32F, 0, 1);
	cv::pow(xDerv, 2, xDerv);
	cv::pow(yDerv, 2, yDerv);
	outputImg = xDerv + yDerv;
	cv::sqrt(outputImg, outputImg);
}

float  GradientCost::operator()(int row, int col, const float disp)
{
	return (*this)(cv::Point(col, row), disp);
}

float  GradientCost::operator()(const cv::Point point, const float disp)
{
	double intPart;
	double frac = modf((double)disp, &intPart);
	if((point.x - disp < 0)||(point.x - disp >= (leftImg.cols - 1))||(point.x < 0)||(point.x >= (leftImg.cols - 1))) return 255.0;
	uint left = leftImg.at<float>(point);
	uint right = frac * rightImg.at<float>(point.y, point.x - disp) + (1 - frac) * rightImg.at<float>(point.y, point.x - disp + 1);
	int cost = sad(left, right);
	return cost;
}

uint GradientCost::sad(ushort a, ushort b)
{
	return abs(a - b);
}
