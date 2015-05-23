#include "FeatureDescriptor.h"


FeatureDescriptor::FeatureDescriptor(const Params& params):
	params(params)
{
}


FeatureDescriptor::~FeatureDescriptor(void)
{
}


void FeatureDescriptor::init(const cv::Mat& leftImg, const cv::Mat& rightImg)
{
	this->leftImg.create(leftImg.rows, leftImg.cols, CV_16U);
	this->rightImg.create(rightImg.rows, rightImg.cols, CV_16U);
	applyTransform(leftImg, this->leftImg);
	applyTransform(rightImg, this->rightImg);
}

void FeatureDescriptor::applyTransform(const cv::Mat& inputImg, cv::Mat& outputImg)
{
	outputImg = inputImg.clone();
}