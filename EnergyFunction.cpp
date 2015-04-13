#include "EnergyFunction.h"


EnergyFunction::EnergyFunction(const Params& params):
	params(params),
	adapWin(params),
	costFnct(params)
{
}


EnergyFunction::~EnergyFunction(void)
{
}

void EnergyFunction::init(const cv::Mat& leftImg, const cv::Mat& rightImg)
{
	adapWin.init(leftImg, rightImg);
	costFnct.init(leftImg, rightImg);
}

bool EnergyFunction::betterPlane(int row, int col, cv::Vec3f planeEquation, cv::Vec3f planeEquationProposal, Direction dir)
{
	float currentCost = computeEnergy(row, col, planeEquation, dir);
	float proposalCost = computeEnergy(row, col, planeEquationProposal, dir);
	return (currentCost > proposalCost)?true:false;
}

float EnergyFunction::computeEnergy(int row, int col, cv::Vec3f planeEquation, Direction dir)
{
	float energy(0.0);
	float sumWeights(0.0);
	for(int neighbRow = (-params.winSize/2 + row); neighbRow < (params.winSize/2 + row); ++neighbRow)
		for(int neighbCol = (-params.winSize/2 + col); neighbCol < (params.winSize/2 + col); ++neighbCol)
		{
			float disp = planeEquation[0]*(neighbCol - col) + planeEquation[1]*(neighbRow - row) + planeEquation[2];
			float weight = adapWin(row, col, neighbRow, neighbCol, dir);
			sumWeights += weight;
			switch(dir)
			{
			case LEFT:
				energy += costFnct(neighbRow, neighbCol, disp) * weight;
				break;
			case RIGHT:
				energy += costFnct(neighbRow, neighbCol + disp, disp) * weight;
				break;
			}
		}
	energy /= sumWeights;

	return energy;
}