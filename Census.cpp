#include "Census.h"

Census::Census(const Params& params):
	FeatureDescriptor(params)
{
}


Census::~Census(void)
{
}

float Census::compute(const cv::Mat& img1, const cv::Mat& img2, const cv::Point2f point1, const cv::Point2f point2)
{
	cv::Rect rect1(cv::Point2f(), img1.size());
	cv::Rect rect2(cv::Point2f(), img2.size());
	if (rect1.contains(point1)&&rect2.contains(point2))
	{
		uint leftCensus = img1.at<ushort>(floor(point1.y + 0.5), floor(point1.x + 0.5));
		uint rightCensus = img2.at<ushort>(floor(point2.y + 0.5), floor(point2.x + 0.5));
		int cost = hamming(leftCensus, rightCensus);
		return cost;
	}else
	{
		return 16.0;
	}
}

float  Census::operator()(int row, int col, const float disp)
{
	return (*this)(cv::Point2f(col, row), disp);
}


float  Census::operator()(const cv::Point2f point, const float disp)
{
	if((point.x - disp < 0)||(point.x - disp >= (leftImg.cols - 1))||(point.x < 0)||(point.x >= (leftImg.cols - 1))||(disp < 0)) return 255.0;
	int cost(0);
	//if((frameCnt % 2)==0)
	{
		cv::Point3f point1(point.x, point.y, disp);
		cv::Point3f point0;
		mvm->getUvdFr1ToFr0(point1, point0);
		cv::Point2f point0L(point0.x, point0.y);
		cv::Point2f point0R(point0.x - point0.z, point0.y);
		cost += compute(leftImg, prevLeftImg, point, point0L);
		cost += compute(leftImg, prevRightImg, point, point0R);
	}
	uint leftCensus = leftImg.at<ushort>(point.y, point.x);
	uint rightCensus = rightImg.at<ushort>(point.y, floor(point.x - disp + 0.5));
	cost += hamming(leftCensus, rightCensus);
	return cost;
}

uint Census::hamming(ushort a, ushort b)
{
	return popcount(a ^ b);
}

uint Census::popcount(unsigned int x)
{
	int c = 0;
	for (; x > 0; x &= x -1) c++;
	return c;
}

void Census::applyTransform(const cv::Mat& inputImg, cv::Mat& outputImg)
{
	int neighb[16][2] = {
		{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, 
		{0, 1}, {1, -1}, {1, 0}, {1, 1},
		{-2, -2}, {-2, 0}, {-2, 2}, {0, -2},
		{0, 2}, {2, -2}, {2, 0}, {2, -2}};
	const int MARGIN = 2;
	for(int row = MARGIN; row < inputImg.rows - MARGIN; ++row)
		for(int col = MARGIN; col < inputImg.cols - MARGIN; ++col)
		{
			ushort census = 0;
			for(int index = 0; index < 16; ++index)
			{
				if (inputImg.at<uchar>(row, col) < inputImg.at<uchar>(row + neighb[index][0], col + neighb[index][1]))
					census = census + 1;
				census = census<<1;
			}
			outputImg.at<ushort>(row, col) = census;
		}
}
