#pragma once

#include "opencv2\core.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\imgproc.hpp"
#include <cmath>

#include "Params.h"
#include "AdaptiveWindow.h"
#include "CombineCost.h"

using namespace cv;

typedef Census CostFunction; 

class EnergyFunction
{
public:
	EnergyFunction(const Params& params);
	~EnergyFunction(void);

	void init(const Mat& leftImg, const Mat& rightImg);
	bool betterPlane(int row, int col, Vec3f planeEquation, Vec3f planeEquationProposal);
	float computeEnergy(int row, int col, Vec3f planeEquation);
private:
	AdaptiveWindow adapWin; 
	CostFunction costFnct;
	const Params& params;
};

