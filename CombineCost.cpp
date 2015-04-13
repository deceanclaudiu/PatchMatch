#include "CombineCost.h"

const float CombineCost::alfa = 0.5;
const float CombineCost::beta = 0.5;

CombineCost::CombineCost(const Params& params):
	FeatureDescriptor(params),
	censusCost(params),
	intensityCost(params),
	gradientCost(params)
{
}


CombineCost::~CombineCost(void)
{
}

void CombineCost::init(const cv::Mat& leftImg, const cv::Mat& rightImg)
{
	leftDscrImg = leftImg;
	rightDscrImg = rightImg;
	censusCost.init(leftImg,rightImg);
	intensityCost.init(leftImg,rightImg);
	gradientCost.init(leftImg,rightImg);
}

float  CombineCost::operator()(int row, int col, const float disp)
{
	return (*this)(cv::Point(col, row), disp);
}

float  CombineCost::operator()(const cv::Point point, const float disp)
{
	double censusCst, gradientCst, intensityCst, cost;
	censusCst = censusCost(point, disp);
	intensityCst = intensityCost(point, disp);
	gradientCst = gradientCost(point, disp);
	
	cost = alfa * intensityCst + beta * gradientCst + (1.0 - alfa - beta) * censusCst;
	return cost;
}

