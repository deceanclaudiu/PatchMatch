#pragma once
#include "opencv2\core.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\imgproc.hpp"
#include <cmath>
#include <cassert>

#include "Params.h"

using namespace cv;

class AdaptiveWindow
{
public:
	AdaptiveWindow(const Params&);
	~AdaptiveWindow(void);

	void init(const Mat& img);
	float  operator()(int row, int col, int neighbRow, int neighbCol);
private:
	Mat img;
	static const float SIGMAI;
	const Params& params;
	int cols, rows;
	float WLUT[256][256];
};

