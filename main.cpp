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
	std::string dstDir = "d:/Work/Kitti/dataset/results\\";
	cv::Mat leftImg0, rightImg0, dispImg;
	for(int no = 1; no<= 193; ++no)
	{
		std::string number;
		if(no < 10)
			number = "00000" + std::to_string(no);
		else if(no < 100)
			number = "0000" + std::to_string(no);
		else 
			number = "000" + std::to_string(no);
		std::string file = number+"_10.png";
		leftImg0 = imread(sourceDir + "image_0/" + file);
		rightImg0 = imread(sourceDir + "image_1/" + file);

		cvtColor(leftImg0, leftImg0, COLOR_RGB2GRAY);
		cvtColor(rightImg0, rightImg0, COLOR_RGB2GRAY);
		cv::namedWindow("Left Image");
		cv::imshow("Left Image", leftImg0);
		cv::waitKey(1);

		MultiViewMatcher mvm(sourceDir + "calib/"+number+".txt");

		// compute visual odometry
		mvm.process(leftImg0, rightImg0);
		// compute first frame
		pm.compute(mvm, leftImg0, rightImg0, dispImg);
		cv::imwrite(dstDir+file, dispImg);

		file = number+"_11.png";
		leftImg0 = imread(sourceDir + "image_0/" + file);
		rightImg0 = imread(sourceDir + "image_1/" + file);
		cvtColor(leftImg0, leftImg0, COLOR_RGB2GRAY);
		cvtColor(rightImg0, rightImg0, COLOR_RGB2GRAY);
		cv::imshow("Left Image", leftImg0);
		cv::waitKey(1);
		// compute visual odometry
		if (mvm.process(leftImg0, rightImg0)) 
		{
			// compute 2nd frame
			pm.compute(mvm, leftImg0, rightImg0, dispImg);
			cv::imwrite(dstDir+file, dispImg);
		}
	}
	return 0;
}