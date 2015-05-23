#pragma once

#include "featuredescriptor.h"
#include <intrin.h> 

class IntensityCost :
	public FeatureDescriptor
{
public:
	IntensityCost(const Params& params);
	~IntensityCost(void);

	float  operator()(const cv::Point point, const float disp);
	float  operator()(int row, int col, const float disp);
private:
	uint sad(ushort a, ushort b);
};

