#include "Census.h"

Census::Census(const Params& params):
	FeatureDescriptor(params)
{
}


Census::~Census(void)
{
}


float  Census::operator()(int row, int col, const float disp)
{
	return (*this)(cv::Point(col, row), disp);
}
float  Census::operator()(const cv::Point point, const float disp)
{
	if((point.x - disp < 0)||(point.x - disp >= (leftImg.cols - 1))||(point.x < 0)||(point.x >= (leftImg.cols - 1))) return 255.0;
	uint leftCensus = leftImg.at<ushort>(point.y, point.x);
	uint rightCensus = rightImg.at<ushort>(point.y, floor(point.x - disp + 0.5));
	int cost = hamming(leftCensus, rightCensus);
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