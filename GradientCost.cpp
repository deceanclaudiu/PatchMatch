#include "GradientCost.h"

GradientCost::GradientCost(const Params& params):
	FeatureDescriptor(params)
{
}


GradientCost::~GradientCost(void)
{
}

void GradientCost::init(const cv::Mat& leftImg, const cv::Mat& rightImg)
{
	applyTransform(leftImg, leftDscrImg);
	applyTransform(rightImg, rightDscrImg);

	//namedWindow("Left Grad");
	//imshow("Left Grad", leftDscrImg/500.0);
	//namedWindow("Right Grad");
	//imshow("Right Grad", rightDscrImg/500.0);
	//waitKey(0);
}

void GradientCost::applyTransform(const cv::Mat& img, cv::Mat& resultImg)
{
	cv::Mat xDerv, yDerv;
	cv::Scharr(img, xDerv, CV_32F, 1, 0);
	cv::Scharr(img, yDerv, CV_32F, 0, 1);
	cv::pow(xDerv, 2, xDerv);
	cv::pow(yDerv, 2, yDerv);
	resultImg = xDerv + yDerv;
	cv::sqrt(resultImg, resultImg);
}

float  GradientCost::operator()(int row, int col, const float disp)
{
	return (*this)(cv::Point(col, row), disp);
}

float  GradientCost::operator()(const cv::Point point, const float disp)
{
	double intPart;
	double frac = modf((double)disp, &intPart);
	if((point.x - disp < 0)||(point.x - disp >= (leftDscrImg.cols - 1))) return 0xFF;
	uint left = leftDscrImg.at<float>(point);
	uint right = frac * rightDscrImg.at<float>(point.y, point.x - disp) + (1 - frac) * rightDscrImg.at<float>(point.y, point.x - disp + 1);
	int cost = sad(left, right);
	return cost;
}

uint GradientCost::sad(ushort a, ushort b)
{
	return abs(a - b);
}
