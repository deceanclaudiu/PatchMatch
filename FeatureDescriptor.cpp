#include "FeatureDescriptor.h"


FeatureDescriptor::FeatureDescriptor(const Params& params):
	params(params),
	frameCnt(0)
{
}


FeatureDescriptor::~FeatureDescriptor(void)
{
}


void FeatureDescriptor::init(MultiViewMatcher& mvm, const cv::Mat& leftImg, const cv::Mat& rightImg)
{
	++frameCnt;
	this->mvm = &mvm;
	prevLeftImg = this->leftImg.clone();
	prevRightImg = this->rightImg.clone();
	this->leftImg.create(leftImg.rows, leftImg.cols, CV_16U);
	this->rightImg.create(rightImg.rows, rightImg.cols, CV_16U);
	applyTransform(leftImg, this->leftImg);
	applyTransform(rightImg, this->rightImg);
}

void FeatureDescriptor::applyTransform(const cv::Mat& inputImg, cv::Mat& outputImg)
{
	outputImg = inputImg.clone();
}