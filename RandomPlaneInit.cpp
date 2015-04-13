#include "RandomPlaneInit.h"


RandomPlaneInit::RandomPlaneInit(const Params& params):
	PlaneInit(params)
{
}


RandomPlaneInit::~RandomPlaneInit(void)
{
}


void RandomPlaneInit::init(const cv::Mat& leftImg, const cv::Mat& rightImg, cv::Mat disp[], cv::Mat normals[])
{
	init(leftImg, rightImg, disp[LEFT], normals[LEFT]);
	init(leftImg, rightImg, disp[RIGHT], normals[RIGHT]);
}

void RandomPlaneInit::init(const cv::Mat& leftImg, const cv::Mat& rightImg, cv::Mat& disp, cv::Mat& normals)
{
	int rows = leftImg.rows;
	int cols = leftImg.cols;
	disp.create(rows, cols, CV_32F);
	normals.create(rows, cols, CV_32FC3);


	randu(disp, params.dispStart, params.dispEnd);
	randu(normals, -2.0, 2.0);
	//Normalize normals to unit vector
	for(int row = 0; row < rows; ++row)
		for(int col = 0; col < cols; ++col)
		{
			cv::Vec3f& vec = normals.at<cv::Vec3f>(row, col);
			cv::normalize(vec, vec, 1.0, 0.0, cv::NORM_L2);
		}
}