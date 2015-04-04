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

void EnergyFunction::init(const Mat& leftImg, const Mat& rightImg)
{
	adapWin.init(leftImg);
	costFnct.init(leftImg, rightImg);
}

bool EnergyFunction::betterPlane(int row, int col, Vec3f planeEquation, Vec3f planeEquationProposal)
{
	float currentCost = computeEnergy(row, col, planeEquation);
	float proposalCost = computeEnergy(row, col, planeEquationProposal);
	return (currentCost > proposalCost)?true:false;
}

float EnergyFunction::computeEnergy(int row, int col, Vec3f planeEquation)
{
	float energy(0.0);
	float sumWeights(0.0);
	for(int neighbRow = (-params.winSize/2 + row); neighbRow < (params.winSize/2 + row); ++neighbRow)
		for(int neighbCol = (-params.winSize/2 + col); neighbCol < (params.winSize/2 + col); ++neighbCol)
		{
			float disp = planeEquation[0]*(neighbCol - col) + planeEquation[1]*(neighbRow - row) + planeEquation[2];
			float weight = adapWin(row, col, neighbRow, neighbCol);
			sumWeights += weight;
			energy += costFnct(neighbRow, neighbCol, disp) * weight;
		}
	energy /= sumWeights;

	return energy;
}