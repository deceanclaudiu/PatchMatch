#pragma once
#include "opencv2\core.hpp"
#include "opencv2\highgui.hpp"

#include "SpatialPlanes.h"
#include "Params.h"
#include "EnergyFunction.h"

using namespace cv;
using namespace std;

class PatchMatch
{
public:
	PatchMatch(void);
	PatchMatch(const Params&);
	~PatchMatch(void);

	void compute(const Mat& leftImg, const Mat& rightImg, Mat& dispImg);

private:
	void spatialPropagation(int it);
	void planeRefinement(int it);
	void init(const Mat& leftImg, const Mat& rightImg);
	SpatialPlanes leftPlanes, rightPlanes;
	EnergyFunction energyFn;
	const Params& params;
	Mat leftCost, rightCost;
	int rows, cols;
};

