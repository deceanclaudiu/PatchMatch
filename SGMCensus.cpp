#include "SGMCensus.h"


SGMCensus::SGMCensus(const Params& params):
	FeatureDescriptor(params),
	census(params)
{
}


SGMCensus::~SGMCensus(void)
{
	delete data;
}


float  SGMCensus::operator()(int row, int col, const float disp)
{
	return (*this)(cv::Point2f(col, row), disp);
}


float  SGMCensus::operator()(const cv::Point2f point, const float disp)
{
	if((point.x - disp < 0)||(point.x - disp >= (leftImg.cols - 1))||(point.x < 0)||(point.x >= (leftImg.cols - 1))) return 255.0;

	//return census(point.y, point.x, disp);
	int row = point.y;
	int col = (int)point.x;
	int d = (int)floor(disp+0.5);
	return S[row][col][d];//census(row, col, disp);
}

void SGMCensus::init(MultiViewMatcher& mvm, const cv::Mat& leftImg, const cv::Mat& rightImg)
{
	height = leftImg.rows;
	width = leftImg.cols;
	dispCnt = params.dispEnd - params.dispStart + 1;    
	FeatureDescriptor::init(mvm, leftImg, rightImg);
	census.init(mvm, leftImg, rightImg);
	memset(data, 0xF, g_maxWidth*g_maxHeight*g_maxDispCnt*sizeof(int));
	for(int row = 0; row < height; ++row)
		for(int col = 0; col < width; ++col)
			for(int disp = 0; disp < dispCnt; ++disp)
			{
				int cost = census(row, col, disp);
				data[row][col][disp]= cost;
			}
	sgm();
}

void SGMCensus::sgm()
{
	memcpy(L, data, g_maxWidth*g_maxHeight*g_maxDispCnt*sizeof(int));
	memcpy(S, data, g_maxWidth*g_maxHeight*g_maxDispCnt*sizeof(int));
	/*for(int row = 0; row < height; ++row)
		for(int col = 0; col < width; ++col)
		{
			if(col > 0)
			{
				int prevMin = 0xFFFF;
				for(int dd = 0;dd < dispCnt;++dd)
				{
					if(prevMin > L[row][col - 1][dd]) prevMin = L[row][col-1][dd];
				}
				for(int disp = 0; disp < dispCnt; ++disp)
				{
						L[row][col][disp] += getSmoothingCost(col-1, row, col, row, disp, prevMin) - prevMin;
						S[row][col][disp] += L[row][col][disp];
				}
			}

		}
	memcpy(L, data, g_maxWidth*g_maxHeight*g_maxDispCnt*sizeof(int));
	for(int row = 0; row < height; ++row)
		for(int col = 0; col < width; ++col)
		{
			if(row > 0)
			{
				int prevMin = 0xFFFF;
				for(int dd = 0;dd < dispCnt;++dd)
				{
					if(prevMin > L[row-1][col][dd]) prevMin = L[row-1][col][dd];
				}
				for(int disp = 0; disp < dispCnt; ++disp)
				{
						L[row][col][disp] += getSmoothingCost(col, row-1, col, row, disp, prevMin) - prevMin;
						S[row][col][disp] += L[row][col][disp];
				}
			}
		}*/
	memcpy(L, data, g_maxWidth*g_maxHeight*g_maxDispCnt*sizeof(int));
	for(int row = 0; row < height; ++row)
		for(int col = 0; col < width; ++col)
		{
			if((col > 0)&&(row > 0))
			{
				int prevMin = 0xFFFF;
				for(int dd = 0;dd < dispCnt;++dd)
				{
					if(prevMin > L[row - 1][col - 1][dd]) prevMin = L[row-1][col-1][dd];
				}
				for(int disp = 0; disp < dispCnt; ++disp)
				{
						L[row][col][disp] += getSmoothingCost(col-1, row-1, col, row, disp, prevMin) - prevMin;
						S[row][col][disp] += L[row][col][disp];
				}
			}

		}/*
	memcpy(L, data, g_maxWidth*g_maxHeight*g_maxDispCnt*sizeof(int));
	for(int row = height - 1; row > 0 ; --row)
		for(int col = width - 1; col > 0; --col)
		{
			if(col < height - 1)
			{
				int prevMin = 0xFFFF;
				for(int dd = 0;dd < dispCnt;++dd)
				{
					if(prevMin > L[row][col + 1][dd]) prevMin = L[row][col + 1][dd];
				}
				for(int disp = 0; disp < dispCnt; ++disp)
				{
						L[row][col][disp] += getSmoothingCost(col + 1, row, col, row, disp, prevMin) - prevMin;
						S[row][col][disp] += L[row][col][disp];
				}
			}
		}
	memcpy(L, data, g_maxWidth*g_maxHeight*g_maxDispCnt*sizeof(int));
	for(int row = height - 1; row > 0 ; --row)
		for(int col = width - 1; col > 0; --col)
		{
			if(row < width - 1)
			{
				int prevMin = 0xFFFF;
				for(int dd = 0;dd < dispCnt;++dd)
				{
					if(prevMin > L[row+1][col][dd]) prevMin = L[row+1][col][dd];
				}
				for(int disp = 0; disp < dispCnt; ++disp)
				{
						L[row][col][disp] += getSmoothingCost(col, row+1, col, row, disp, prevMin) - prevMin;
						S[row][col][disp] += L[row][col][disp];
				}
			}
		}
	memcpy(L, data, g_maxWidth*g_maxHeight*g_maxDispCnt*sizeof(int));
	for(int row = height - 1; row > 0 ; --row)
		for(int col = width - 1; col > 0; --col)
		{
			if((row < width - 1)&&(col < height - 1))
			{
				int prevMin = 0xFFFF;
				for(int dd = 0;dd < dispCnt;++dd)
				{
					if(prevMin > L[row+1][col+1][dd]) prevMin = L[row+1][col+1][dd];
				}
				for(int disp = 0; disp < dispCnt; ++disp)
				{
						L[row][col][disp] += getSmoothingCost(col+1, row+1, col, row, disp, prevMin) - prevMin;
						S[row][col][disp] += L[row][col][disp];
				}
			}
		}*/
}


int SGMCensus::getSmoothingCost(int prevCol, int prevRow, int col, int row, int d, int prevMin)
{
	int cost = L[prevRow][prevCol][d];

	if(d > params.dispStart)
	{
		int cost2 = L[prevRow][prevCol][d - 1] + P1;
		if(cost > cost2) cost = cost2;
	}
	if(d < params.dispEnd)
	{
		int cost3 = L[prevRow][prevCol][d + 1] + P1;
		if (cost > cost3) cost = cost3;
	}
	float simDiff = abs(leftImg.at<uchar>(row,col)-leftImg.at<uchar>(prevRow,prevCol)) + 1.0;
	prevMin += P2/simDiff;
	if (cost > prevMin) cost = prevMin;

	return cost;
}