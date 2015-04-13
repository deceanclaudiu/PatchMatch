#pragma once
#include "opencv2\core.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\imgproc.hpp"

#include "Params.h"

class PlaneRefinementProposal
{
public:
	PlaneRefinementProposal(const Params& params);
	~PlaneRefinementProposal(void);
	void generate(const cv::Mat disp[], const cv::Mat normals[], float deltaDisp, cv::Vec3f deltaNormal, cv::Mat dispProp[], cv::Mat normalsProp[]);
protected:
	void generate(const cv::Mat& disp, const cv::Mat& normals, float deltaDisp, cv::Vec3f deltaNormal, cv::Mat& dispProp, cv::Mat& normalsProp);
private:
	const Params& params;
};

