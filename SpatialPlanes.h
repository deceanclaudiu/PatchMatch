#pragma once

#include<stdio.h>
#include<iostream>

#include "RandomPlaneInit.h"
#include "SGMPlaneInit.h"
#include "PlaneRefinementProposal.h"

using namespace cv;

typedef RandomPlaneInit PlaneInitMethod; 

class SpatialPlanes
{
public:
	typedef enum {
		CURRENT,
		REFINED
	} RETURN_TYPE;
	SpatialPlanes(const Params&);
	~SpatialPlanes(void);

	Vec3f operator()(int row, int col, Direction dir, RETURN_TYPE returnType= CURRENT);
	void replace(int rowFrom, int colFrom, int rowTo, int colTo, Direction dir);
	void init(const Mat& leftImg, const Mat& rightImg);
	void refine(float deltaDisp, Vec3f deltaNormal);
	void replaceWithRefinedPlane(int row, int col, Direction dir);
//private:
	PlaneInitMethod planeInit;
	PlaneRefinementProposal planeRefinementProposal;
	Mat disp[2], normals[2];
	Mat refinedDisp[2], refinedNormals[2];
	const Params& params;
};

