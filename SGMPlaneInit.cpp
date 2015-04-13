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
	disp[LEFT].create(leftImg.rows, leftImg.cols, CV_32F);
	normals[LEFT].create(leftImg.rows, leftImg.cols, CV_32FC3);
	disp[RIGHT].create(leftImg.rows, leftImg.cols, CV_32F);
	normals[RIGHT].create(leftImg.rows, leftImg.cols, CV_32FC3);
	cv::Mat leftImgPadded = leftImg;
	cv::Mat rightImgPadded = rightImg;
	uint8* leftImgPtr = leftImgPadded.data;
	uint8* rightImgPtr = rightImgPadded.data;
	float32 * dispImg, * dispImgRight;
	if(leftImg.cols % 4 != 0)
	{
		int pad = leftImg.cols % 4;
		cv::copyMakeBorder(leftImg, leftImgPadded, 0, 0, 0, 4 - pad, cv::BORDER_CONSTANT);
		cv::copyMakeBorder(rightImg, rightImgPadded, 0, 0, 0, 4 - pad, cv::BORDER_CONSTANT);
	}
	dispImg = (float32*)_mm_malloc(leftImgPadded.cols*leftImgPadded.rows*sizeof(float32), 16);
	dispImgRight = (float32*)_mm_malloc(leftImgPadded.cols*leftImgPadded.rows*sizeof(float32), 16);
	processCensus9x7SGM(leftImgPtr, rightImgPtr, dispImg, dispImgRight, leftImgPadded.cols, leftImgPadded.rows, 1, 8, 4, 4, 256);
}