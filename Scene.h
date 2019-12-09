#ifndef _SCENE_H_
#define _SCENE_H_

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include "Camera.h"
#include "Color.h"
#include "Model.h"
#include "Rotation.h"
#include "Scaling.h"
#include "Translation.h"
#include "Triangle.h"
#include "Vec3.h"
#include "Vec4.h"
#include "Matrix4.h"

using namespace std;

class Scene
{
public:
	Color backgroundColor;
	bool cullingEnabled;
	int projectionType;

	bool isTransformed = false;
	vector< Vec3 > transformedVertices;

	vector< vector<Color> > image;
	vector< Camera* > cameras;
	vector< Vec3* > vertices;
	vector< Color* > colorsOfVertices;
	vector< Scaling* > scalings;
	vector< Rotation* > rotations;
	vector< Translation* > translations;
	vector< Model* > models;

	Scene(const char *xmlPath);

	void initializeImage(Camera* camera);
	void forwardRenderingPipeline(Camera* camera);
	int makeBetweenZeroAnd255(double value);
	void writeImageToPPMFile(Camera* camera);
	void convertPPMToPNG(string ppmFileName, int osType);

	void transformAllModels();
	void transformModel(int m);
	void transformAllVertices(int modelIndex, Matrix4 M);
	
	std::vector<int> getUniqueVerticesOfModel(int modelIndex);
	void restoreVertices();
	void saveVertices();

	Matrix4 calculateCameraMatrix(Camera *camera);
	Matrix4 calculateViewportMatrix(Camera* camera);
	Matrix4 ortographicProjection(Camera *camera);
	void perspectiveProjection(int modelIndex, Camera *camera);
	void viewportTransformation(int modelIndex, Matrix4 M_viewport );

	void wireframeRasterization(Triangle tri);
	void draw(int x, int y, Color color);
	void midpointX(Vec3 v0, Vec3 v1, bool isNegative);
	void midpointY(Vec3 v0, Vec3 v1, bool isNegative);
	void rasterizeLine(Vec3 v0, Vec3 v1);
	
};

#endif