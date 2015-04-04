#pragma once
#include "opencv2\core.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\imgproc.hpp"
#include <cmath> 

#include "Params.h"

using namespace cv;
using namespace std;

class FeatureDescriptor
{
public:
	FeatureDescriptor(const Params& params);
	virtual ~FeatureDescriptor(void);
	virtual void init(const Mat& leftImg, const Mat& rightImg){};
	virtual float  operator()(const Point point, const float disp){ return 0;};

protected:
	const Params& params;
};

