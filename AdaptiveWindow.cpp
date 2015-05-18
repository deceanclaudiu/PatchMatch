#include "AdaptiveWindow.h"

const float AdaptiveWindow::SIGMAI = 10.0;

AdaptiveWindow::AdaptiveWindow(const Params& params):
	params(params)
{
	for(int i = 0; i < 256; ++i)
		for(int j = 0; j < 256; ++j)
			WLUT[i][j] = exp(-abs(i-j)/SIGMAI);
}

AdaptiveWindow::~AdaptiveWindow(void)
{
}

void AdaptiveWindow::init(const cv::Mat& leftImg, const cv::Mat& rightImg)
{
	(*this).leftImg = leftImg;
	(*this).rightImg = rightImg;
}

float  AdaptiveWindow::operator()(int row, int col, int neighbRow, int neighbCol, Direction dir)
{
	assert((row >= 0) && (col >= 0) && (neighbRow >= 0) && (neighbCol >= 0) && (row < leftImg.rows) && (col < leftImg.cols) && (neighbRow < leftImg.rows) && (neighbCol < leftImg.cols));
	switch(dir)
	{
		case LEFT:
			return WLUT[leftImg.at<uchar>(row, col)][leftImg.at<uchar>(neighbRow, neighbCol)];
		case RIGHT:
			return WLUT[rightImg.at<uchar>(row, col)][rightImg.at<uchar>(neighbRow, neighbCol)];
		default:
			return 0;
	}
}