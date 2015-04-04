#pragma once

#include "featuredescriptor.h"
#include <intrin.h> 

class Census :
	public FeatureDescriptor
{
public:
	Census(const Params& params);
	~Census(void);

	void init(const Mat& leftImg, const Mat& rightImg);
	float  operator()(const Point point, const float disp);
	float  operator()(int row, int col, const float disp);
private:
	uint popcount(unsigned int x);
	void applyCensusTransform(const Mat& img, Mat& censusImg);
	uint hamming(ushort a, ushort b);
	Mat leftCensusImg;
	Mat rightCensusImg;
};

