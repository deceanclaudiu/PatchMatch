#include "opencv2\core.hpp"
#include "opencv2\highgui.hpp"
#include <opencv2/imgproc/imgproc.hpp>

#include "Census.h"
#include "PatchMatch.h"

using namespace std;
using namespace cv;

Params params;
PatchMatch pm(params);

int main()
{
	string sourceDir = "d:/Work/Kitti/dataset/training\\";
	Mat leftImg, rightImg, dispImg, leftFilteredImg, rightFilteredImg;

	leftImg = imread(sourceDir + "image_0/000028_10.png");
	rightImg = imread(sourceDir + "image_1/000028_10.png");
	cvtColor(leftImg, leftImg, COLOR_RGB2GRAY);
	cvtColor(rightImg, rightImg, COLOR_RGB2GRAY);
	//leftImg = imread("D:/Work/DisparityEstimationMethods/PatchMatch/left.png");
	//rightImg = imread("D:/Work/DisparityEstimationMethods/PatchMatch/right.png");
	//bilateralFilter(leftImg, leftFilteredImg, 15, 10, 10);
	//bilateralFilter(rightImg, rightFilteredImg, 15, 10, 10);
	namedWindow("Left Image");
	imshow("Left Image", leftImg);
	waitKey(1);

	pm.compute(leftImg, rightImg, dispImg);
	return 0;
}