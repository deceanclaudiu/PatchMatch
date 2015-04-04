#pragma once

#include "opencv2\core.hpp"
#include "opencv2\highgui.hpp"
#include<stdio.h>
#include<iostream>

#include "Params.h"

using namespace cv;
using namespace std;

class SpatialPlanes
{
public:
	typedef enum {
		CURRENT,
		REFINED
	} RETURN_TYPE;
	SpatialPlanes(const Params&);
	~SpatialPlanes(void);

	Vec3f operator()(int row, int col, RETURN_TYPE returnType= CURRENT);
	void replace(int rowFrom, int colFrom, int rowTo, int colTo);
	void initRand(int rows, int cols);
	void refine(float deltaDisp, Vec3f deltaNormal);
	void replaceWithRefinedPlane(int row, int col);
//private:
	Mat disp, normals;
	Mat refinedDisp, refinedNormals;
	const Params& params;
};

