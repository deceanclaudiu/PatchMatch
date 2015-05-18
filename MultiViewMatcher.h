#pragma once

#include "viso_stereo.h"
#include "opencv2\core.hpp"
#include "opencv2\calib3d.hpp"

class MultiViewMatcher
{
public:
	MultiViewMatcher(VisualOdometryStereo::parameters param);
	MultiViewMatcher(std::string calibFile);
	~MultiViewMatcher(void);

	bool process(cv::Mat& leftImg, cv::Mat& rightImg);

	void getUvdFr1ToFr0(const cv::Point3f& input, cv::Point3f& output);
private:
	VisualOdometryStereo * viso;
	Matrix leftProj, rightProj;
	Matrix P, Pinv, M, Minv;
	float b;//baseline in meters
	float f;//focal length in pixels
	float cu, cv;
};

