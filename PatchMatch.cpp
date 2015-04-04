#include "PatchMatch.h"
#include <iostream>
#include <fstream>

using namespace std;

void writeMatToFileN(cv::Mat& m, const char* filename)
{
    ofstream fout(filename);

    if(!fout)
    {
        cout<<"File Not Opened"<<endl;  return;
    }

    for(int i=0; i<m.rows; i++)
    {
        for(int j=0; j<m.cols; j++)
        {
            fout<<(int)m.at<uchar>(i,j)<<"\t";
        }
        fout<<endl;
    }

    fout.close();
}
PatchMatch::PatchMatch(const Params& params):
	params(params),
	leftPlanes(params),
	rightPlanes(params),
	energyFn(params)
{
}

PatchMatch::~PatchMatch(void)
{
}

void PatchMatch::compute(const Mat& leftImg, const Mat& rightImg, Mat& dispImg)
{
	rows = leftImg.rows;
	cols = leftImg.cols;
	dispImg.create(leftImg.rows, leftImg.cols, CV_32F);
	init(leftImg, rightImg);
	for(int it = 0; it < params.iterationNo; ++it)
	{
		spatialPropagation(it);
		planeRefinement(it);

		imshow("disparity image", leftPlanes.disp/255);		
		waitKey(1);
	}
	imwrite("result.png", leftPlanes.disp);
	waitKey(1);
}

void PatchMatch::init(const Mat& leftImg, const Mat& rightImg)
{
	leftPlanes.initRand(rows, cols);
	rightPlanes.initRand(rows, cols);
	energyFn.init(leftImg, rightImg);
	leftCost.create(rows, cols, CV_32F);
	rightCost.create(rows, cols, CV_32F);
	for(int row = (params.winSize/2+1); row < (rows-params.winSize/2); ++row)
		for(int col = (params.winSize/2+1); col < (cols-params.winSize/2) ; ++col)
		{
			leftCost.at<float>(row, col) = energyFn.computeEnergy(row, col, leftPlanes(row, col));
		}
}

void PatchMatch::planeRefinement(int it)
{
	Vec3f refinedPlane;
	leftPlanes.refine(params.dispEnd/(it*it+1), 1.0/(it*it+1));
	for(int row = (params.winSize/2+1); row < (rows-params.winSize/2); ++row)
			for(int col = (params.winSize/2+1); col < (cols-params.winSize/2) ; ++col)
			{
				refinedPlane = leftPlanes(row, col, SpatialPlanes::REFINED);
				float cost = energyFn.computeEnergy(row, col, refinedPlane);
				if (cost < leftCost.at<float>(row, col))
					{
						leftPlanes.replaceWithRefinedPlane(row, col);
						leftCost.at<float>(row, col) = cost;
					}
			}
}
void PatchMatch::spatialPropagation(int it)
{
	Vec3f plane;
	int neighbRow, neighbCol;
	/*int neighb[8][2] = { {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}};*/
	int neighb[4][2] = { {0, 1}, {1, 0}, {0, -1}, {-1, 0}};
	if(it%2 == 0)
	{//!if even iteration start from NW
		for(int row = (params.winSize/2+1); row < (rows-params.winSize/2); ++row)
		{
			for(int col = (params.winSize/2+1); col < (cols-params.winSize/2) ; ++col)
			{
				plane = leftPlanes(row, col);
				for(int i=0; i<2;++i)
				{
					neighbRow = row + neighb[i][0];
					neighbCol = col + neighb[i][1];
					float cost = energyFn.computeEnergy(neighbRow, neighbCol, plane);
					if (cost < leftCost.at<float>(neighbRow, neighbCol))
					{
						leftPlanes.replace(row, col, neighbRow, neighbCol);
						leftCost.at<float>(row, col) = cost;
					}
				}
			}									
	}
	}else{//!if odd iteration start from SE
		for(int row = (rows - params.winSize/2 - 1); row > params.winSize/2; --row)
		{
			for(int col = (cols -  params.winSize/2 - 1); col > params.winSize/2; --col)
			{
				plane = leftPlanes(row, col);
				for(int i=2; i<4;++i)
				{
					neighbRow = row + neighb[i][0];
					neighbCol = col + neighb[i][1];
					float cost = energyFn.computeEnergy(neighbRow, neighbCol, plane);
					if (cost < leftCost.at<float>(neighbRow, neighbCol))
					{
						leftPlanes.replace(row, col, neighbRow, neighbCol);
						leftCost.at<float>(row, col) = cost;
					}
				}
			}
		}
	}
}