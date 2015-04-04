#include "SpatialPlanes.h"

#include <iostream>
#include <fstream>

using namespace std;

void writeMatToFile(cv::Mat& m, const char* filename)
{
    ofstream fout(filename);

    if(!fout)
    {
        cout<<"File Not Opened"<<endl;  return;
    }

    for(int i=0; i<m.rows; i++)
    {
        for(int j=0; j<m.cols; j++)
        {
            fout<<m.at<float>(i,j)<<"\t";
        }
        fout<<endl;
    }

    fout.close();
}

SpatialPlanes::SpatialPlanes(const Params& params):
	params(params)
{
}

SpatialPlanes::~SpatialPlanes(void)
{
}

void SpatialPlanes::initRand(int rows, int cols)
{
	disp.create(rows, cols, CV_32F);
	normals.create(rows, cols, CV_32FC3);

	refinedDisp.create(rows, cols, CV_32F);
	refinedNormals.create(rows, cols, CV_32FC3);

	randu(disp, params.dispStart, params.dispEnd);
	randu(normals, -2.0, 2.0);
	//Normalize normals to unit vector
	for(int row = 0; row < rows; ++row)
		for(int col = 0; col < cols; ++col)
		{
			Vec3f& vec = normals.at<Vec3f>(row, col);
			normalize(vec, vec, 1.0, 0.0, NORM_L2);
		}
	//writeMatToFile(disp,"disp.txt");
}

void SpatialPlanes::replace(int rowFrom, int colFrom, int rowTo, int colTo)
{
	Vec3f normal = normals.at<Vec3f>(rowFrom, colFrom);
	float d = disp.at<float>(rowFrom, colFrom);
	normals.at<Vec3f>(rowTo, colTo) = normal;
	disp.at<float>(rowTo, colTo) = d;
}

void SpatialPlanes::replaceWithRefinedPlane(int row, int col)
{
	Vec3f normal = refinedNormals.at<Vec3f>(row, col);
	float d = refinedDisp.at<float>(row, col);
	normals.at<Vec3f>(row, col) = normal;
	disp.at<float>(row, col) = d;
}

void SpatialPlanes::refine(float deltaDisp, Vec3f deltaNormal)
{
	Mat deltaDispMap(disp.rows, disp.cols, CV_32F);
	Mat deltaNormalsMap(disp.rows, disp.cols, CV_32FC3);

	randu(deltaDispMap, -deltaDisp, deltaDisp);
	randu(deltaNormalsMap, -deltaNormal, deltaNormal);

	refinedDisp = disp + deltaDispMap;
	refinedNormals = normals + deltaNormalsMap;

	for(int row = 0; row < disp.rows; ++row)
		for(int col = 0; col < disp.cols; ++col)
		{

			Vec3f& normal = refinedNormals.at<Vec3f>(row, col);
			float d = refinedDisp.at<float>(row, col);
			//normal[2] = (normal[2] == 0)? 0.01:normal[2];
			refinedDisp.at<float>(row, col) = (d < 0)? 0.01:d;
			refinedDisp.at<float>(row, col) = (d > params.dispEnd)? params.dispEnd:d;

			normalize(normal, normal, 1.0, 0.0, NORM_L2);
		}
}

Vec3f SpatialPlanes::operator()(int row, int col, RETURN_TYPE returnType)
{
	Vec3f plane(0,0,0);
	float d(0);
	Vec3f normal(0,0,0);
	switch(returnType)
	{
	case CURRENT:
		normal = normals.at<Vec3f>(row, col);
		d = disp.at<float>(row, col);
		break;
	case REFINED:
		normal = refinedNormals.at<Vec3f>(row, col);
		d = refinedDisp.at<float>(row, col);
		break;
	} 
	plane[0] = -normal[0]/normal[2];
	plane[1] = -normal[1]/normal[2];
	plane[2] = d;

	return plane;
}