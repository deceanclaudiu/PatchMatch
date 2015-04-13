#include "opencv2\core.hpp"
#include "opencv2\highgui.hpp"
#include <opencv2/imgproc/imgproc.hpp>

#include "Census.h"
#include "PatchMatch.h"

Params params;
PatchMatch pm(params);

int main()
{
	std::string sourceDir = "d:/Work/Kitti/dataset/training\\";
	cv::Mat leftImg, rightImg, dispImg, leftFilteredImg, rightFilteredImg;

	leftImg = imread(sourceDir + "image_0/000028_10.png");
	rightImg = imread(sourceDir + "image_1/000028_10.png");
	cvtColor(leftImg, leftImg, COLOR_RGB2GRAY);
	cvtColor(rightImg, rightImg, COLOR_RGB2GRAY);
	//leftImg = imread("D:/Work/DisparityEstimationMethods/PatchMatch/left.png");
	//rightImg = imread("D:/Work/DisparityEstimationMethods/PatchMatch/right.png");
	//bilateralFilter(leftImg, leftFilteredImg, 15, 10, 10);
	//bilateralFilter(rightImg, rightFilteredImg, 15, 10, 10);
	cv::namedWindow("Left Image");
	cv::imshow("Left Image", leftImg);
	cv::waitKey(1);

	pm.compute(leftImg, rightImg, dispImg);
	return 0;
}