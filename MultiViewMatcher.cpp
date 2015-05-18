#include "MultiViewMatcher.h"
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream


MultiViewMatcher::MultiViewMatcher(VisualOdometryStereo::parameters param)/*:
	viso(param)*/
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
	viso = new VisualOdometryStereo(param);

	P = leftProj.getMat(0, 0, 2, 2);
	Pinv = Matrix::inv(P);

	//std::cout << P <<std::endl<<std::endl;
	//std::cout << Pinv <<std::endl<<std::endl;
}

MultiViewMatcher::~MultiViewMatcher(void)
{
}

void MultiViewMatcher::getUvdFr1ToFr0(const cv::Point3f& input, cv::Point3f& output)
{
	float u = input.x;
	float v = input.y;
	float d = input.z;
	Matrix xyz1_h(4,1);
	xyz1_h.val[0][0] = b*(u-cu)/d;
	xyz1_h.val[1][0] = b*(v-cv)/d;
	xyz1_h.val[2][0] = b*f/d;
	xyz1_h.val[3][0] = 1;
	std::cout<<xyz1_h<<std::endl<<std::endl;

	Matrix xyz0_h = Minv * xyz1_h;
	std::cout<<xyz0_h<<std::endl<<std::endl;

	Matrix xyz0 = (xyz0_h/xyz0_h.val[3][0]).getMat(0, 0, 2, 0);
	std::cout<<xyz0<<std::endl<<std::endl;

	Matrix uv0_h = P * xyz0;
	Matrix uv0 = (uv0_h/uv0_h.val[2][0]).getMat(0, 0, 1, 0);
	std::cout<<uv0<<std::endl<<std::endl;

	output.x = uv0.val[0][0]  ;
	output.y = uv0.val[1][0];
	return;
}

bool MultiViewMatcher::process(cv::Mat& leftImg, cv::Mat& rightImg)
{
	// compute visual odometry
	int32_t dims[] = {leftImg.cols, leftImg.rows, leftImg.cols};
	bool result = viso->process(leftImg.data, rightImg.data,dims);

	// current pose (this matrix transforms a point from the current
	// frame's camera coordinates to the first frame's camera coordinates)nv
	M = viso->getMotion();
	Minv = Matrix::inv(M);

	std::cout<<M<<std::endl<<std::endl;
	std::cout<<Minv<<std::endl<<std::endl;
	return result;
}