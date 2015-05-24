#include "opencv2\core.hpp"
#include "opencv2\highgui.hpp"
#include <opencv2/imgproc/imgproc.hpp>

#include "PatchMatch.h"
#include "MultiViewMatcher.h"

Params params;
PatchMatch pm(params);

int main()
{
	std::string sourceDir = "d:/Work/Kitti/dataset/training\\";
	cv::Mat leftImg0, rightImg0, leftImg1, rightImg1, dispImg, leftFilteredImg, rightFilteredImg;

	leftImg0 = imread(sourceDir + "image_0/000028_10.png");
	rightImg0 = imread(sourceDir + "image_1/000028_10.png");
	leftImg1 = imread(sourceDir + "image_0/000028_11.png");
	rightImg1 = imread(sourceDir + "image_1/000028_11.png");
	cvtColor(leftImg0, leftImg0, COLOR_RGB2GRAY);
	cvtColor(rightImg0, rightImg0, COLOR_RGB2GRAY);
	cvtColor(leftImg1, leftImg1, COLOR_RGB2GRAY);
	cvtColor(rightImg1, rightImg1, COLOR_RGB2GRAY);
	//leftImg = imread("D:/Work/DisparityEstimationMethods/PatchMatch/left.png");
	//rightImg = imread("D:/Work/DisparityEstimationMethods/PatchMatch/right.png");
	//bilateralFilter(leftImg, leftFilteredImg, 15, 10, 10);
	//bilateralFilter(rightImg, rightFilteredImg, 15, 10, 10);
	cv::namedWindow("Left Image");
	cv::imshow("Left Image", leftImg1);
	cv::waitKey(1);

	MultiViewMatcher mvm(sourceDir + "calib/000028.txt");

	// compute visual odometry
	mvm.process(leftImg0, rightImg0);
	// compute first frame
	pm.compute(mvm, leftImg0, rightImg0, dispImg);

	// compute visual odometry
	if (mvm.process(leftImg1, rightImg1)) 
	{
		// compute 2nd frame
		pm.compute(mvm, leftImg1, rightImg1, dispImg);
		//for(int i=1 ; i< 255; ++i)
		//{
		//	std::cout<<i<<std::endl;
		//	cv::Point3f input(300,200, i);
		//	cv::Point3f output;
		//	mvm.getUvdFr1ToFr0(input, output);
		//	cv::Point2f pct1(input.x, input.y);
		//	cv::Point2f pct0(output.x, output.y);
		//	cv::Point2f pct0R(output.x - output.z, output.y);
		//	cv::circle(leftImg1, pct1, 10, cv::Scalar(0), 3);
		//	Mat leftImg0t = leftImg0.clone();
		//	cv::circle(leftImg0t, pct0, 10, cv::Scalar(0), 3);
		//	Mat rightImg0t = rightImg0.clone();
		//	cv::circle(rightImg0t, pct0R, 10, cv::Scalar(0), 3);
		//	cv::namedWindow("Left Image 0");
		//	cv::imshow("Left Image", leftImg1);
		//	cv::imshow("Left Image 0", leftImg0t);
		//	cv::imshow("Right Image 0", rightImg0t);
		//	cv::waitKey(0);
		//}
	}
	return 0;
}