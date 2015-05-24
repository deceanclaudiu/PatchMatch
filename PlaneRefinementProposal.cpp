#include "PlaneRefinementProposal.h"


PlaneRefinementProposal::PlaneRefinementProposal(const Params& params):
	params(params)
{
}


PlaneRefinementProposal::~PlaneRefinementProposal(void)
{
}


void PlaneRefinementProposal::generate(const cv::Mat disp[], const cv::Mat normals[], float deltaDisp, cv::Vec3f deltaNormal, cv::Mat dispProp[], cv::Mat normalsProp[])
{
	generate(disp[LEFT], normals[LEFT], deltaDisp, deltaNormal, dispProp[LEFT], normalsProp[LEFT]);
	generate(disp[RIGHT], normals[RIGHT], deltaDisp, deltaNormal, dispProp[RIGHT], normalsProp[RIGHT]);
}


void PlaneRefinementProposal::generate(const cv::Mat& disp, const cv::Mat& normals, float deltaDisp, cv::Vec3f deltaNormal, cv::Mat& dispProp, cv::Mat& normalsProp)
{
	cv::Mat deltaDispMap(disp.rows, disp.cols, CV_32F);
	cv::Mat deltaNormalsMap(disp.rows, disp.cols, CV_32FC3);

	cv::randu(deltaDispMap, -deltaDisp, deltaDisp);
	cv::randu(deltaNormalsMap, -deltaNormal, deltaNormal);

	dispProp = abs(disp + deltaDispMap);
	normalsProp = normals + deltaNormalsMap;

	for(int row = 0; row < disp.rows; ++row)
		for(int col = 0; col < disp.cols; ++col)
		{

			cv::Vec3f& normal = normalsProp.at<cv::Vec3f>(row, col);
			float d = dispProp.at<float>(row, col);
			//normal[2] = (normal[2] == 0)? 0.01:normal[2];
			dispProp.at<float>(row, col) = (d < 0)? 0.01:d;
			dispProp.at<float>(row, col) = (d > params.dispEnd)? params.dispEnd:d;

			cv::normalize(normal, normal, 1.0, 0.0, cv::NORM_L2);
		}
}