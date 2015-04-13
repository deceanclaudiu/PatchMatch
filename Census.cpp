#include "Census.h"

Census::Census(const Params& params):
	FeatureDescriptor(params)
{
}


Census::~Census(void)
{
}

void Census::init(const cv::Mat& leftImg, const cv::Mat& rightImg)
{
	leftCensusImg.create(leftImg.rows, leftImg.cols, CV_16U);
	rightCensusImg.create(rightImg.rows, rightImg.cols, CV_16U);
	applyCensusTransform(leftImg, leftCensusImg);
	applyCensusTransform(rightImg, rightCensusImg);

		//namedWindow("Left Census");
		//imshow("Left Census", leftCensusImg);
		//namedWindow("Right Census");
		//imshow("Right Census", rightCensusImg);
		//waitKey(0);
}

float  Census::operator()(int row, int col, const float disp)
{
	return (*this)(cv::Point(col, row), disp);
}
float  Census::operator()(const cv::Point point, const float disp)
{
	if((point.x - disp < 0)||(point.x - disp >= (leftCensusImg.cols - 1))||(point.x < 0)||(point.x >= (leftCensusImg.cols - 1))) return 255.0;
	uint leftCensus = leftCensusImg.at<ushort>(point.y, point.x);
	uint rightCensus = rightCensusImg.at<ushort>(point.y, floor(point.x - disp + 0.5));
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

void Census::applyCensusTransform(const cv::Mat& img, cv::Mat& censusImg)
{
	int neighb[16][2] = {
		{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, 
		{0, 1}, {1, -1}, {1, 0}, {1, 1},
		{-2, -2}, {-2, 0}, {-2, 2}, {0, -2},
		{0, 2}, {2, -2}, {2, 0}, {2, -2}};
	const int MARGIN = 2;
	for(int row = MARGIN; row < img.rows - MARGIN; ++row)
		for(int col = MARGIN; col < img.cols - MARGIN; ++col)
		{
			ushort census = 0;
			for(int index = 0; index < 16; ++index)
			{
				if (img.at<uchar>(row, col) < img.at<uchar>(row + neighb[index][0], col + neighb[index][1]))
					census = census + 1;
				census = census<<1;
			}
			censusImg.at<ushort>(row, col) = census;
		}
}