#include "SGMPlaneInit.h"

SGMPlaneInit::SGMPlaneInit(const Params& params):
	PlaneInit(params)
{
}


SGMPlaneInit::~SGMPlaneInit(void)
{
}

void SGMPlaneInit::init(const cv::Mat& leftImg, const cv::Mat& rightImg, cv::Mat disp[], cv::Mat normals[])
{
	cv::Vec3f normalLeft(0,0,0);
	disp[LEFT].create(leftImg.rows, leftImg.cols, CV_32F);
	normals[LEFT].create(leftImg.rows, leftImg.cols, CV_32FC3);
	disp[RIGHT].create(leftImg.rows, leftImg.cols, CV_32F);
	normals[RIGHT].create(leftImg.rows, leftImg.cols, CV_32FC3);
	cv::Ptr<cv::StereoSGBM> sgm = cv::StereoSGBM::create(0,256,3, 50, 800, 1, 15, 0, 100, 32, 1); 
	cv::Mat disp_uint16, dervX, dervY;
	sgm->compute(leftImg, rightImg, disp_uint16);
	cv::Scharr(disp_uint16, dervX, CV_16S, 1, 0);
	cv::Scharr(disp_uint16, dervY, CV_16S, 0, 1);
	for(int row=0;row < leftImg.rows; ++row)
		for(int col=0;col < leftImg.cols; ++col)
		{
			float dispValue = disp_uint16.at<ushort>(row, col)/16.0f;
			float a = dervX.at<short>(row, col)/16.0;
			float b = dervY.at<short>(row, col)/16.0;
			float c = dispValue;
			float nz = c;
			float nx = (-1) * a * nz;
			float ny = (-1) * b * nz;
			normalLeft = cv::Vec3f(nx, ny, nz);
			disp[LEFT].at<float>(row, col) = dispValue;
			disp[RIGHT].at<float>(row, col) = dispValue;
			if(c==0)
			{
				normalLeft = cv::Vec3f(0, 0, 1);
			}else
			{
				cv::normalize(normalLeft, normalLeft, 1.0, 0.0, cv::NORM_L2);
			}
			normals[LEFT].at<cv::Vec3f>(row, col) = normalLeft;
			normals[RIGHT].at<cv::Vec3f>(row, col) = normalLeft;
		}
}