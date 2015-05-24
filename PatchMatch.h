#pragma once
#include "opencv2\core.hpp"
#include "opencv2\highgui.hpp"

#include "SpatialPlanes.h"
#include "Params.h"
#include "EnergyFunction.h"

class PatchMatch
{
public:
	PatchMatch(void);
	PatchMatch(const Params&);
	~PatchMatch(void);

	void compute(MultiViewMatcher& mvm, const cv::Mat& leftImg, const cv::Mat& rightImg, cv::Mat& dispImg);

private:
	void spatialPropagation(SpatialPlanes& planes, EnergyFunction& energyFnct, cv::Mat& curCost, int it, Direction dir);
	void viewPropagation(SpatialPlanes& planes, EnergyFunction& energyFnct, cv::Mat& curCost, int it, Direction dir);
	void planeRefinement(SpatialPlanes& planes, EnergyFunction& energyFnct, cv::Mat& curCost, int it, Direction dir);
	void leftRightConfCheck(SpatialPlanes& planes, cv::Mat& dispImg);
	void init(MultiViewMatcher& mvm, const cv::Mat& leftImg, const cv::Mat& rightImg);
	SpatialPlanes planes;
	EnergyFunction energyFnct;
	const Params& params;
	cv::Mat leftCost, rightCost;
	int frameCnt;
	int rows, cols;
};

