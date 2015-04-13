#pragma once

#include "opencv2\core.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\imgproc.hpp"
#include <cmath>

#include "Params.h"
#include "AdaptiveWindow.h"
#include "CombineCost.h"

typedef Census CostFunction; 

class EnergyFunction
{
public:
	EnergyFunction(const Params& params);
	~EnergyFunction(void);

	void init(const cv::Mat& leftImg, const cv::Mat& rightImg);
	bool betterPlane(int row, int col, cv::Vec3f planeEquation, cv::Vec3f planeEquationProposal, Direction dir);
	float computeEnergy(int row, int col, cv::Vec3f planeEquation, Direction dir);
private:
	AdaptiveWindow adapWin; 
	CostFunction costFnct;
	const Params& params;
};

