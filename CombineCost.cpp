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

void CombineCost::init(MultiViewMatcher& mvm, const cv::Mat& leftImg, const cv::Mat& rightImg)
{
	FeatureDescriptor::init(mvm, leftImg, rightImg);
	censusCost.init(mvm, leftImg,rightImg);
	intensityCost.init(mvm, leftImg,rightImg);
	gradientCost.init(mvm, leftImg,rightImg);
}

float  CombineCost::operator()(int row, int col, const float disp)
{
	return (*this)(cv::Point2f(col, row), disp);
}

float  CombineCost::operator()(const cv::Point2f point, const float disp)
{
	double censusCst, gradientCst, intensityCst, cost;
	censusCst = censusCost(point, disp);
	intensityCst = intensityCost(point, disp);
	gradientCst = gradientCost(point, disp);
	
	cost = alfa * intensityCst + beta * gradientCst + (1.0 - alfa - beta) * censusCst;
	return cost;
}

