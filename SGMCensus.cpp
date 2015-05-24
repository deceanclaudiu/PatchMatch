#include "SGMCensus.h"


SGMCensus::SGMCensus(const Params& params):
	FeatureDescriptor(params)
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
	//int dispRange = params.dispEnd - params.dispStart;
	//int b =  data[(int)floor(point.y * leftImg.cols*dispRange + point.x * dispRange + disp - params.dispStart + 0.5)];
	return 2.0;
}

void SGMCensus::init(MultiViewMatcher& mvm, const cv::Mat& leftImg, const cv::Mat& rightImg)
{
	int width = leftImg.rows;
	int height = leftImg.cols;
	int dispRange = params.dispEnd - params.dispStart;
	StereoSGMParams_t paramsStereo;
    paramsStereo.lrCheck = true;
    paramsStereo.MedianFilter = true;
    paramsStereo.Paths = 8;
    paramsStereo.subPixelRefine = 0;
    paramsStereo.NoPasses = 2;
    paramsStereo.rlCheck = false;
	// get memory and init sgm params
    float32* dispImg = (float32*)_mm_malloc(width*height*sizeof(float32), 16);
    float32* dispImgRight = (float32*)_mm_malloc(width*height*sizeof(float32), 16);
	uint16* dsi = (uint16*)_mm_malloc(width*height*dispRange*sizeof(uint16), 32);
    uint64* leftImgCensus = (uint64*)_mm_malloc(width*height*sizeof(uint64), 16);
    uint64* rightImgCensus = (uint64*)_mm_malloc(width*height*sizeof(uint64), 16);
	StereoSGM<uint8> m_sgm16(width, height, params.dispEnd , paramsStereo);
	census9x7_mode8(leftImg.data, leftImgCensus, width, height);
    census9x7_mode8(rightImg.data, rightImgCensus, width, height);

    costMeasureCensusCompressed9x7_xyd(leftImgCensus, rightImgCensus, height, width, dispRange, 1, dsi);

    m_sgm16.processParallel(dsi, leftImg.data, dispImg, dispImgRight, 1);

/*	Census::init(mvm, leftImg, rightImg);
	int rows = leftImg.rows;
	int cols = leftImg.cols;
	int dispRange = params.dispEnd - params.dispStart;
	delete data;
	data =  new int[rows*cols*dispRange];
	int * tmp = data;
	for(int row = 0; row < rows; ++row)
		for(int col = 0; col < cols; ++col)
			for(int disp = 0; disp < dispRange; ++disp, tmp++)
			{
				int cost = Census::operator()(row, col, disp + params.dispStart);
				*tmp = cost;
			}*/
}