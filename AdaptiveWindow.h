#pragma once
#include "opencv2\core.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\imgproc.hpp"
#include <cmath>
#include <cassert>

#include "Params.h"

class AdaptiveWindow
{
public:
	AdaptiveWindow(const Params&);
	~AdaptiveWindow(void);

	void init(const cv::Mat& leftImg, const cv::Mat& rightImg);
	float  operator()(int row, int col, int neighbRow, int neighbCol, Direction dir);
private:
	cv::Mat leftImg, rightImg;
	static const float SIGMAI;
	const Params& params;
	int cols, rows;
	float WLUT[256][256];
};

