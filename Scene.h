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
	
	//void rotateModel(int modelIndex, int transformIndex);
	//void scaleModel(int modelIndex, int transformIndex);
	//void translateModel(int modelIndex, int transformIndex);

	//void translateTriangle(Triangle tri, Translation* tr);
	//void scaleTriangle(Triangle tri, Scaling* s);
	Matrix4 calculateCameraMatrix(Camera *camera);
	Matrix4 calculateProjectionMatrix(Camera *camera);
	//void cameraTransformation(int modelIndex, Matrix4 M_camera);

	std::vector<int> getUniqueVerticesOfModel(int modelIndex);
	//void rotateVertex(int vertex_id, Rotation* r);
	//void scaleVertex(int vertex_id, Scaling* s);
	//void translateVertex(int vertex_id, Translation* tr);
	void restoreVertices();
	void saveVertices();
	Matrix4 ortographicProjection(Camera *camera);
	Matrix4 perspectiveProjection(Camera *camera);
	
	void viewportTransformation(int modelIndex, Matrix4 M_viewport );
	Matrix4 calculateViewportMatrix(Camera* camera);
	
};

#endif