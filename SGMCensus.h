#pragma once
#include "Census.h"
//#include "StereoSGM.h"
//#include "rSGMCmd.h"

class SGMCensus:
	public FeatureDescriptor
{
public:
	SGMCensus(const Params& params);
	~SGMCensus(void);
	float  operator()(const cv::Point2f point, const float disp);
	float  operator()(int row, int col, const float disp);
	virtual void init(MultiViewMatcher& mvm, const cv::Mat& leftImg, const cv::Mat& rightImg);
protected:
	void sgm();
	int getSmoothingCost(int prevCol, int prevRow, int col, int row, int d, int prevMin);
private:
	int height;
	int width;
	int dispCnt; 
	Census census;
	int data[g_maxHeight][g_maxWidth][g_maxDispCnt];
	int L[g_maxHeight][g_maxWidth][g_maxDispCnt];
	int S[g_maxHeight][g_maxWidth][g_maxDispCnt];
	static const int P1 = 1;
	static const int P2 = 4*256;
};

