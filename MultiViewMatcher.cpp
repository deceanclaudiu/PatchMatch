#include "MultiViewMatcher.h"
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream


MultiViewMatcher::MultiViewMatcher(VisualOdometryStereo::parameters param)
{
}

MultiViewMatcher::MultiViewMatcher(std::string calibFile)
{
	std::ifstream ifs (calibFile, std::ifstream::in);
	Matrix tmp(3, 4);
	float val; // = ifs.get();
	std::string s;
	for (int i = 0; i < 2; ++i)
	{
		ifs >> s;
		for(int row = 0; row < 3; ++row)
			for(int col = 0; col < 4; ++col)
			{
				ifs >> val;
				tmp.val[row][col] = val;
			}
		if(i==0)
		{
			leftProj = tmp;
		}else
		{
			rightProj = tmp;
		}
	}
	ifs.close();

	VisualOdometryStereo::parameters param;
	f = leftProj.val[0][0];
	b = (-1) * rightProj.val[0][3] / f;
	cu = leftProj.val[0][2];
	cv = leftProj.val[1][2];
	param.calib.f  = f; // focal length in pixels
	param.calib.cu = leftProj.val[0][2]; // principal point (u-coordinate) in pixels
	param.calib.cv = leftProj.val[1][2]; // principal point (v-coordinate) in pixels
	param.base     = b; // baseline in meters
	param.ransac_iters = 500;
	param.inlier_threshold = 2.0;
 	viso = new VisualOdometryStereo(param);
}

MultiViewMatcher::~MultiViewMatcher(void)
{
	delete viso;
}

void MultiViewMatcher::getUvdFr1ToFr0(const cv::Point3f& input, cv::Point3f& output)
{
	float u = input.x;
	float v = input.y;
	float d = input.z + 0.0001;

	float x = b*(u-cu)/d;
	float y = b*(v-cv)/d;
	float z = b*f/d;
	float dd;
	float xx(0),yy(0),zz(0);

	xx = s1[0]*x + s1[1]*y + s1[2]* z + s1[3]; 
	yy = s2[0]*x + s2[1]*y + s2[2]* z + s2[3]; 
	zz = s3[0]*x + s3[1]*y + s3[2]* z + s3[3]; 
	dd = rt3[0]*x + rt3[1]*y + rt3[2]* z + rt3[3]; 
	dd = b*f/dd;
	output.x = xx/zz;
	output.y = yy/zz;
	output.z = dd;
	return;
}

bool MultiViewMatcher::process(cv::Mat& leftImg, cv::Mat& rightImg)
{
	// compute visual odometry
	int32_t dims[] = {leftImg.cols, leftImg.rows, leftImg.cols};
	bool result = viso->process(leftImg.data, rightImg.data,dims);
	// output some statistics
    double num_matches = viso->getNumberOfMatches();
    double num_inliers = viso->getNumberOfInliers();
    std::cout << "Matches: " << num_matches<< std::endl;
    std::cout << "Inliers: " << 100.0*num_inliers/num_matches << " %" << std::endl;

	// current pose (this matrix transforms a point from the current
	// frame's camera coordinates to the first frame's camera coordinates)nv
	M = viso->getMotion();
	std::cout<<"M:"<<std::endl<<M<<std::endl<<std::endl;
	Minv = Matrix::inv(M);
	std::cout<<"Minv:"<<std::endl<<Minv<<std::endl<<std::endl;
	std::cout<<"Proj:"<<std::endl<<leftProj<<std::endl<<std::endl;
	S = leftProj*Minv;
	std::cout<<"S:"<<std::endl<<S<<std::endl<<std::endl;
	for(int i=0; i<4; ++i)
	{
		s1[i] = S.val[0][i];
		s2[i] = S.val[1][i];
		s3[i] = S.val[2][i];
		rt3[i] = Minv.val[2][i];
	}
	return result;
}