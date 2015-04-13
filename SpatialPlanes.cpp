#include "SpatialPlanes.h"

SpatialPlanes::SpatialPlanes(const Params& params):
	params(params),
	planeInit(params),
	planeRefinementProposal(params)
{
}

SpatialPlanes::~SpatialPlanes(void)
{
}

void SpatialPlanes::init(const Mat& leftImg, const Mat& rightImg)
{
	planeInit.init(leftImg, rightImg, disp, normals);
}

void SpatialPlanes::replace(int rowFrom, int colFrom, int rowTo, int colTo, Direction dir)
{
	Vec3f normal = normals[dir].at<Vec3f>(rowFrom, colFrom);
	float d = disp[dir].at<float>(rowFrom, colFrom);
	normals[dir].at<Vec3f>(rowTo, colTo) = normal;
	disp[dir].at<float>(rowTo, colTo) = d;
}

void SpatialPlanes::replaceWithRefinedPlane(int row, int col, Direction dir)
{
	Vec3f normal = refinedNormals[dir].at<Vec3f>(row, col);
	float d = refinedDisp[dir].at<float>(row, col);
	normals[dir].at<Vec3f>(row, col) = normal;
	disp[dir].at<float>(row, col) = d;
}

void SpatialPlanes::refine(float deltaDisp, Vec3f deltaNormal)
{
	planeRefinementProposal.generate(disp, normals, deltaDisp, deltaNormal, refinedDisp, refinedNormals); 
}

Vec3f SpatialPlanes::operator()(int row, int col, Direction dir, RETURN_TYPE returnType)
{
	Vec3f plane(0,0,0);
	float d(0);
	Vec3f normal(0,0,0);
	switch(returnType)
	{
	case CURRENT:
		normal = normals[dir].at<Vec3f>(row, col);
		d = disp[dir].at<float>(row, col);
		break;
	case REFINED:
		normal = refinedNormals[dir].at<Vec3f>(row, col);
		d = refinedDisp[dir].at<float>(row, col);
		break;
	} 
	plane[0] = -normal[0]/normal[2];
	plane[1] = -normal[1]/normal[2];
	plane[2] = d;

	return plane;
}