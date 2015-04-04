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

void AdaptiveWindow::init(const Mat& img)
{
	(*this).img = img;
}

float  AdaptiveWindow::operator()(int row, int col, int neighbRow, int neighbCol)
{
	assert((row >= 0) && (col >= 0) && (neighbRow >= 0) && (neighbCol >= 0) && (row < img.rows) && (col < img.cols) && (neighbRow < img.rows) && (neighbCol < img.cols));
	return WLUT[img.at<uchar>(row, col)][img.at<uchar>(neighbRow, neighbCol)];
}