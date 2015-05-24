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
	planes(params),
	energyFnct(params),
	frameCnt(0)
{
}

PatchMatch::~PatchMatch(void)
{
}

void PatchMatch::leftRightConfCheck(SpatialPlanes& planes, cv::Mat& dispImg)
{
	dispImg = planes.disp[LEFT];
	for(int row = 0; row < rows; ++row)
		for(int col = 0; col < cols; ++col)
		{
			float d = planes.disp[LEFT].at<float>(row, col);
			if (((col - d + 0.5f) < 0)||(abs(d - planes.disp[RIGHT].at<float>(row, (int)floor(col - d + 0.5f))) > params.leftRightConfCheckTh))
				dispImg.at<float>(row, col) = 0;
		}
}

void PatchMatch::compute(MultiViewMatcher& mvm, const cv::Mat& leftImg, const cv::Mat& rightImg, cv::Mat& dispImg)
{
	++frameCnt;
	rows = leftImg.rows;
	cols = leftImg.cols;
	dispImg.create(leftImg.rows, leftImg.cols, CV_32F);
	init(mvm, leftImg, rightImg);
	if(frameCnt < 2) return;
	cv::imshow("left disparity image", planes.disp[LEFT]/255);
	cv::imshow("right disparity image", planes.disp[RIGHT]/255);
	std::cout<<"initialization done"<<endl;
	cv::waitKey(1);
	for(int it = 0; it < params.iterationNo; ++it)
	{
		spatialPropagation(planes, energyFnct, leftCost, it, LEFT);
		spatialPropagation(planes, energyFnct, rightCost, it, RIGHT);

		cv::imshow("left disparity image", planes.disp[LEFT]/255);
		cv::imshow("right disparity image", planes.disp[RIGHT]/255);
		std::cout<<"spatial propagation done at iteration "<<it<<endl;
		cv::waitKey(1);

		planeRefinement(planes, energyFnct, leftCost, it, LEFT);
		planeRefinement(planes, energyFnct, rightCost, it, RIGHT);

		cv::imshow("left disparity image", planes.disp[LEFT]/255);
		cv::imshow("right disparity image", planes.disp[RIGHT]/255);
		std::cout<<"plane refinement done at iteration "<<it<<endl;
		cv::waitKey(1);

		viewPropagation(planes, energyFnct, leftCost, it, LEFT);
		viewPropagation(planes, energyFnct, rightCost, it, RIGHT);

		cv::imshow("left disparity image", planes.disp[LEFT]/255);
		cv::imshow("right disparity image", planes.disp[RIGHT]/255);
		std::cout<<"view propagation done at iteration "<<it<<endl;
		cv::waitKey(1);
	}
	spatialPropagation(planes, energyFnct, leftCost, params.iterationNo, LEFT);
	spatialPropagation(planes, energyFnct, rightCost, params.iterationNo, RIGHT);

	cv::imshow("left disparity image", planes.disp[LEFT]/255);
	cv::imshow("right disparity image", planes.disp[RIGHT]/255);
	std::cout<<"spatial propagation done at iteration "<<params.iterationNo<<endl;
	cv::waitKey(1);

	leftRightConfCheck(planes, dispImg);
	imshow("disparity image", dispImg/255);
	cout<<"disparity algo done " <<endl;
	waitKey(1);
	imwrite("disp_result.png", dispImg);
	imwrite("left_result.png", planes.disp[LEFT]);
	imwrite("right_result.png", planes.disp[RIGHT]);
	waitKey(0);
}

void PatchMatch::init(MultiViewMatcher& mvm, const cv::Mat& leftImg, const cv::Mat& rightImg)
{
	int rows = leftImg.rows;
	int cols = leftImg.cols;
	planes.init(leftImg, rightImg);
	energyFnct.init(mvm, leftImg, rightImg);
	leftCost.create(rows, cols, CV_32F);
	rightCost.create(rows, cols, CV_32F);
	for(int row = (params.winSize/2+1); row < (rows-params.winSize/2); ++row)
		for(int col = (params.winSize/2+1); col < (cols-params.winSize/2) ; ++col)
		{
			leftCost.at<float>(row, col) = energyFnct.computeEnergy(row, col, planes(row, col, LEFT), LEFT);
			rightCost.at<float>(row, col) = energyFnct.computeEnergy(row, col, planes(row, col, RIGHT), RIGHT);
		}
}

void PatchMatch::planeRefinement(SpatialPlanes& planes, EnergyFunction& energyFnct, cv::Mat& curCost, int it, Direction dir)
{
	Vec3f refinedPlane;
	planes.refine(params.dispEnd/(it*it+1), 1.0f/(it*it+1));
	for(int row = (params.winSize/2+1); row < (rows-params.winSize/2); ++row)
			for(int col = (params.winSize/2+1); col < (cols-params.winSize/2) ; ++col)
			{
				refinedPlane = planes(row, col, dir, SpatialPlanes::REFINED);
				float cost = energyFnct.computeEnergy(row, col, refinedPlane, dir);
				if (cost < curCost.at<float>(row, col))
					{
						planes.replaceWithRefinedPlane(row, col, dir);
						curCost.at<float>(row, col) = cost;
					}
			}
}

void PatchMatch::viewPropagation(SpatialPlanes& planes, EnergyFunction& energyFnct, cv::Mat& curCost, int it, Direction dir)
{
	Vec3f secondPlane;
	int firstCol, firstRow;
	for(int row = (params.winSize/2+1); row < (rows-params.winSize/2); ++row)
	{
		for(int col = (params.winSize/2+1); col < (cols-params.winSize/2) ; ++col)
		{
			secondPlane = planes(row, col, dir);
			for(int i=0; i<2;++i)
			{
				firstRow = row;
				switch(dir)
				{
				case LEFT:
					firstCol = (int)floor(col - secondPlane[2] + 0.5f);
					break;
				case RIGHT:
					firstCol = (int)floor(col + secondPlane[2] + 0.5f);
					break;
				}
				//! if matching pixel is inside image
				if ((firstCol > params.winSize/2)&&(firstCol < (cols - params.winSize/2)))
				{
					float cost = energyFnct.computeEnergy(firstRow, firstCol, secondPlane, dir);
					if (cost < curCost.at<float>(firstRow, firstCol))
					{
						planes.normals[dir].at<Vec3f>(firstRow, firstCol) = planes.normals[dir].at<Vec3f>(row, col);
						planes.disp[dir].at<float>(firstRow, firstCol) = planes.disp[dir].at<float>(row, col);
						curCost.at<float>(row, col) = cost;
					}
				}
			}
		}
	}
}

void PatchMatch::spatialPropagation(SpatialPlanes& planes, EnergyFunction& energyFnct, cv::Mat& curCost, int it, Direction dir)
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
				plane = planes(row, col, dir);
				for(int i=0; i<2;++i)
				{
					neighbRow = row + neighb[i][0];
					neighbCol = col + neighb[i][1];
					float cost = energyFnct.computeEnergy(neighbRow, neighbCol, plane, dir);
					if (cost < curCost.at<float>(neighbRow, neighbCol))
					{
						planes.replace(row, col, neighbRow, neighbCol, dir);
						curCost.at<float>(row, col) = cost;
					}
				}
			}									
	}
	}else{//!if odd iteration start from SE
		for(int row = (rows - params.winSize/2 - 1); row > params.winSize/2; --row)
		{
			for(int col = (cols -  params.winSize/2 - 1); col > params.winSize/2; --col)
			{
				plane = planes(row, col, dir);
				for(int i=2; i<4;++i)
				{
					neighbRow = row + neighb[i][0];
					neighbCol = col + neighb[i][1];
					float cost = energyFnct.computeEnergy(neighbRow, neighbCol, plane, dir);
					if (cost < curCost.at<float>(neighbRow, neighbCol))
					{
						planes.replace(row, col, neighbRow, neighbCol, dir);
						curCost.at<float>(row, col) = cost;
					}
				}
			}
		}
	}
}