#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <cmath>

#include <set>

#include "Scene.h"
#include "Camera.h"
#include "Color.h"
#include "Model.h"
#include "Rotation.h"
#include "Scaling.h"
#include "Translation.h"
#include "Triangle.h"
#include "Vec3.h"
#include "tinyxml2.h"
#include "Helpers.h"

using namespace tinyxml2;
using namespace std;


void Scene::transformAllVertices(int modelIndex, Matrix4 M){
	std::vector<int> modelVerticeIds = this->getUniqueVerticesOfModel(modelIndex);
	int vertex_id;

	for (int i = 0; i < modelVerticeIds.size(); i++ ){
		vertex_id = modelVerticeIds[i] - 1;
		Vec4 vertex = Vec3toVec4(this->vertices[vertex_id]);
		vertex = multiplyMatrixWithVec4(M, vertex);

		/*
		cout << "matrix : " << endl;
		cout << M << endl;
		cout << "vertex multiplied : " << vertex << endl;
		*/
		this->vertices[vertex_id]->x = vertex.x;
		this->vertices[vertex_id]->y = vertex.y;
		this->vertices[vertex_id]->z = vertex.z;
		/*
		cout << "---------------" << endl;
		cout << "After transform" << endl;
		cout << *this->vertices[vertex_id] << endl;
		*/
	}
}

void Scene::transformModel(int modelIndex) {
	std::vector<int> modelVerticeIds = this->getUniqueVerticesOfModel(modelIndex);

	for (int t = 0; t < this->models[modelIndex]->numberOfTransformations; t++) {

		if (models[modelIndex]->transformationTypes[t] == 'r') {
			Rotation* r = this->rotations[(this->models[modelIndex]->transformationIds[t]) - 1];
			cout << "Rotation : " << *r << endl;
			Matrix4 R = getRotationMatrix(r);
			this->transformAllVertices(modelIndex, R);
			
		}

		else if (models[modelIndex]->transformationTypes[t] == 's') {
			Scaling* s = this->scalings[(this->models[modelIndex]->transformationIds[t]) - 1];
			cout << "Scaling : " << *s << endl;
			Matrix4 S = getScalingMatrix(s);
			this->transformAllVertices(modelIndex, S);
			
		}
		else {
			Translation* tr = this->translations[this->models[modelIndex]->transformationIds[t] - 1];
			cout << "Translation : " << *tr << endl;
			Matrix4 T = getTranslationMatrix(tr);
			this->transformAllVertices(modelIndex, T);
		}
	}
}

std::vector<int> Scene::getUniqueVerticesOfModel(int modelIndex) {

	Model* model = models[modelIndex];
	std::vector<int> nonUnique; 
	for (int tri = 0; tri < model->numberOfTriangles; tri++ ) {
		nonUnique.push_back(model->triangles[tri].getFirstVertexId());
		nonUnique.push_back(model->triangles[tri].getSecondVertexId());
		nonUnique.push_back(model->triangles[tri].getThirdVertexId());
	}

	std::set<int> verticeIDSet;
	unsigned size = nonUnique.size();
	for (unsigned i = 0; i < size; i++) {
		verticeIDSet.insert(nonUnique[i]);
	}
	nonUnique.assign(verticeIDSet.begin(), verticeIDSet.end());
	return nonUnique;
}



Matrix4 Scene::calculateCameraMatrix(Camera *camera){
	double camera_coords[3] = {camera->pos.getElementAt(0), camera->pos.getElementAt(1) ,  camera->pos.getElementAt(2)};

	Matrix4 empty_matrix;
	empty_matrix.val[0][0] = camera->u.getElementAt(0);
	empty_matrix.val[0][1] = camera->u.getElementAt(1);
	empty_matrix.val[0][2] = camera->u.getElementAt(2);
	empty_matrix.val[0][3] = -(empty_matrix.val[0][0]*camera_coords[0]+empty_matrix.val[0][1]*camera_coords[1]+empty_matrix.val[0][2]*camera_coords[2]);
	empty_matrix.val[1][0] = camera->v.getElementAt(0);
	empty_matrix.val[1][1] = camera->v.getElementAt(1);
	empty_matrix.val[1][2] = camera->v.getElementAt(2);
	empty_matrix.val[1][3] = -(empty_matrix.val[1][0]*camera_coords[0]+empty_matrix.val[1][1]*camera_coords[1]+empty_matrix.val[1][2]*camera_coords[2]);
	empty_matrix.val[2][0] = camera->w.getElementAt(0);
	empty_matrix.val[2][1] = camera->w.getElementAt(1);
	empty_matrix.val[2][2] = camera->w.getElementAt(2);
	empty_matrix.val[2][3] = -(empty_matrix.val[2][0]*camera_coords[0]+empty_matrix.val[2][1]*camera_coords[1]+empty_matrix.val[2][2]*camera_coords[2]);
	empty_matrix.val[3][3]=1;
	return empty_matrix;
}

void Scene::saveVertices(){
	for(int i = 0; i < this->vertices.size(); i++){
		this->transformedVertices.push_back( *this->vertices[i]);
	}
}

void Scene::restoreVertices(){
	for(int i = 0; i < this->vertices.size(); i++){
		*this->vertices[i] = this->transformedVertices[i];
	}
}


Matrix4 Scene::ortographicProjection(Camera *camera){
	double m_val[4][4] = {{2/(camera->right - camera->left), 0, 0, -(camera->right + camera->left)/(camera->right - camera->left)},
						{0, 2/(camera->top - camera->bottom), 0, -(camera->top + camera->bottom)/(camera->top - camera->bottom) },
						{0, 0, -2/(camera->far - camera->near), -(camera->far + camera->near)/(camera->far - camera->near)},
						{0, 0, 0, 1}};
	
	return Matrix4(m_val);
}

void Scene::perspectiveProjection(int modelIndex, Camera *camera){
	double m_val[4][4] = {{(2*camera->near)/(camera->right - camera->left), 0, (camera->right + camera->left)/(camera->right - camera->left), 0},
						{0, (2*camera->near)/(camera->top - camera->bottom), (camera->top + camera->bottom)/(camera->top - camera->bottom), 0 },
						{0, 0, -(camera->far + camera->near)/(camera->far - camera->near), -(2*camera->far*camera->near)/(camera->far - camera->near)},
						{0, 0, -1, 0}};

	Matrix4 M_perspective(m_val);
	std::vector<int> modelVerticeIds = this->getUniqueVerticesOfModel(modelIndex);
	
	for (int i = 0; i < modelVerticeIds.size(); i++ ){
		int vertex_id = modelVerticeIds[i] - 1;
		Vec4 vertex(Vec3toVec4(this->vertices[vertex_id]));
		vertex = multiplyMatrixWithVec4(M_perspective, vertex);
		double t = vertex.t;

		this->vertices[vertex_id]->x = (vertex.x)/t;
		this->vertices[vertex_id]->y = (vertex.y)/t;
		this->vertices[vertex_id]->z = (vertex.z)/t;
		cout << "Vertice no: " << vertex_id << " " << *this->vertices[vertex_id] << endl; 
	}
}

Matrix4 Scene::calculateViewportMatrix(Camera* camera){
	double m_val[4][4] = {{(camera->horRes)/2.0, 0, 0, (camera->horRes - 1)/2.0},
						{0, (camera->verRes)/2.0, 0, (camera->verRes - 1)/2.0 },
						{0, 0, 1/2, 1/2},
						{0, 0, 0, 1}};
	return Matrix4(m_val);
}

void Scene::viewportTransformation(int modelIndex, Matrix4 M_viewport ){
	Color c(0,0,0);
	std::vector<int> modelVerticeIds = this->getUniqueVerticesOfModel(modelIndex);
	
	for (int i = 0; i < modelVerticeIds.size(); i++ ){
		int vertex_id = modelVerticeIds[i] - 1;
		Vec4 vert(Vec3toVec4(this->vertices[vertex_id]));

		//cout << "Viewport matrix: " << endl;
		//cout << M_viewport << endl;
		Vec3 coordinate = Vec4toVec3(multiplyMatrixWithVec4(M_viewport, vert));
		
		this->vertices[vertex_id]->x = coordinate.x;
		this->vertices[vertex_id]->y = coordinate.y;
		this->vertices[vertex_id]->z = coordinate.z;

		cout << coordinate << endl;

		//this->image[round(coordinate.x)][round(coordinate.y)].r = c.r;
		//this->image[round(coordinate.x)][round(coordinate.y)].g = c.g;
		//this->image[round(coordinate.x)][round(coordinate.y)].b = c.b;
		
	}
}

void Scene::draw(int x, int y, Color color){
	this->image[x][y].r = color.r;
	this->image[x][y].g = color.g;
	this->image[x][y].b = color.b;
}

void Scene::midpointY(Vec3 v0, Vec3 v1, bool isNegative, Camera* camera){
	int x = v0.x;
	int d = 2*(v0.x - v1.x) + (v1.y - v0.y);
	Color* c0 = this->colorsOfVertices[v0.colorId - 1];
	Color* c1 = this->colorsOfVertices[v1.colorId - 1];
	Color c(c0->r, c0->g, c0->b);
	Color dc; 
    dc.r = (c1->r - c0->r) / (v1.y - v0.y );
    dc.g = (c1->g - c0->g) / (v1.y - v0.y );
    dc.b = (c1->b - c0->b) / (v1.y - v0.y ); 

	for(int y = v0.y; y <= v1.y; y++ ){
		if(x >= 0 && x < camera->horRes && y >= 0 && y < camera->verRes){
			this->draw(x, y, c);
		}
		
		if(!isNegative){
			if(d < 0){ //choose NE
				x++;
				d += 2*((v0.x - v1.x) + (v1.y - v0.y));
			}
			else{ //choose N
				d += 2*(v0.x - v1.x);
			}
		}
		else{ // this is problematic probably
			if( d > 0){ //choose NW
				x--;
				d += 2*((v0.x - v1.x) - (v1.y - v0.y));
			}
			else{ //choose N
				d += 2*(v0.x - v1.x);
			}
		}
		c.r += dc.r;
		c.g += dc.g;
		c.b += dc.b;
	}
}

void Scene::midpointX(Vec3 v0, Vec3 v1, bool isNegative, Camera* camera){
	int y = v0.y;
	int d = 2*(v0.y - v1.y) + (v1.x - v0.x);
	Color* c0 = this->colorsOfVertices[v0.colorId - 1];
	Color* c1 = this->colorsOfVertices[v1.colorId - 1];
	Color c(c0->r, c0->g, c0->b);
	Color dc; 
    dc.r = (c1->r - c0->r) / (v1.x - v0.x );
    dc.g = (c1->g - c0->g) / (v1.x - v0.x );
    dc.b = (c1->b - c0->b) / (v1.x - v0.x ); 


	for(int x = v0.x; x <= v1.x; x++ ){
		if(x >= 0 && x < camera->horRes && y >= 0 && y < camera->verRes){
			this->draw(x, y, c);
		}
		
		if(!isNegative){
			if(d < 0){ //choose NE
				y++;
				d += 2*((v0.y - v1.y) + (v1.x - v0.x));
			}
			else{ //choose E
				d += 2*(v0.y - v1.y);
			}
		}
		else{
			if( d > 0){ //choose SE
				y--;
				d += 2*((v0.y - v1.y) - (v1.x - v0.x));
			}
			else{ //choose E
				d += 2*(v0.y - v1.y);
			}
		}
		c.r += dc.r;
		c.g += dc.g;
		c.b += dc.b;
	}
}

void Scene::rasterizeLine(Vec3 v0, Vec3 v1, Camera* camera){

	double slope = (v1.y - v0.y)/(v1.x - v0.x);
	if(slope <= 1 && slope >= -1){
		if(v1.x < v0.x){
			Vec3 temp(v0);
			v0 = v1;
			v1 = temp;	
		}
	}
	else{
		if(v1.y < v0.y){
			Vec3 temp(v0);
			v0 = v1;
			v1 = temp;
		}
	}

	cout << "comparing vertices: "<< endl;
	cout << "v0 is: " << v0 << endl;
	cout << "v1 is: " << v1 << endl;

	bool isNegative = false;
	if(slope < 1 && slope > -1){ //move on x axis
		if(v1.y < v0.y){
			isNegative = true;		
		}
		this->midpointX(v0, v1, isNegative, camera);		 
	}
	else{ //move on y axis
		if(v1.x < v0.x){
			isNegative = true;
		}
		this->midpointY(v0, v1, isNegative, camera);
	}
}

void Scene::wireframeRasterization(Triangle tri, Camera* camera){
	Vec3 v0 = *this->vertices[tri.getFirstVertexId()-1];
	Vec3 v1 = *this->vertices[tri.getSecondVertexId()-1];
	Vec3 v2 = *this->vertices[tri.getThirdVertexId()-1];

	//todo: write midpointY for slopes greater than 1
	//then call midpointX or midpointY for each vertice pair
	this->rasterizeLine(v0, v1, camera);
	this->rasterizeLine(v1, v2, camera);
	this->rasterizeLine(v0, v2, camera);
}

double Scene::lineEquation(Vec3 v0, Vec3 v1, int x, int y){
	return x*(v0.y - v1.y) + y*(v1.x - v0.x) + (v0.x*v1.y) - (v0.y*v1.x);
}

void Scene::triangleRasterization(Triangle tri, Camera* camera){
	Vec3 v0 = *this->vertices[tri.getFirstVertexId()-1];
	Vec3 v1 = *this->vertices[tri.getSecondVertexId()-1];
	Vec3 v2 = *this->vertices[tri.getThirdVertexId()-1];
	v0.x = round(v0.x);
	v0.y = round(v0.y);
	v0.z = round(v0.z);
	v1.x = round(v1.x);
	v1.y = round(v1.y);
	v1.z = round(v1.z);
	v2.x = round(v2.x);
	v2.y = round(v2.y);
	v2.z = round(v2.z);

	double xMin = min({v0.x, v1.x, v2.x});
	double xMax = max({v0.x, v1.x, v2.x});
	double yMin = min({v0.y,v1.y,v2.y});
	double yMax = max({v0.y,v1.y,v2.y});
	double alpha, beta, gamma;

	for(int y = yMin; y<= yMax; y++){
		for(int x = xMin; x <= xMax; x++){
			alpha = lineEquation(v1, v2, x, y) / lineEquation(v1, v2, v0.x, v0.y);
			beta = lineEquation(v2, v0, x, y) / lineEquation(v2, v0, v1.x, v1.y);
			gamma = lineEquation(v0, v1, x, y) / lineEquation(v0, v1, v2.x, v2.y);

			if(alpha >= 0 && beta >= 0 && gamma >= 0){
				Color* c0 = this->colorsOfVertices[v0.colorId - 1];
				Color* c1 = this->colorsOfVertices[v1.colorId - 1];
				Color* c2 = this->colorsOfVertices[v2.colorId - 1];

				if(x >= 0 && x < camera->horRes  && y >= 0 && y < camera->verRes){
					this->image[x][y].r = (c0->r * alpha) + (c1->r * beta) + (c2->r * gamma);
					this->image[x][y].g = (c0->g * alpha) + (c1->g * beta) + (c2->g * gamma);
					this->image[x][y].b = (c0->b * alpha) + (c1->b * beta) + (c2->b * gamma);
				}				
			}
		}
	}
}

bool Scene::backfaceCulling(Triangle tri, Camera* camera) {
	bool backFacing = false;

	Vec3 v0 = *this->vertices[tri.getFirstVertexId()-1];
	Vec3 v1 = *this->vertices[tri.getSecondVertexId()-1];
	Vec3 v2 = *this->vertices[tri.getThirdVertexId()-1];

	// Find normal of the triangle N(n_x, n_y, n_z):
	Vec3 v, w;
 	v.x = v1.x - v0.x;
	v.y = v1.y - v0.y;
	v.z = v1.z - v0.z;

	w.x = v2.x - v0.x;
	w.y = v2.y - v0.y;
	w.z = v2.z - v0.z;

	Vec3 N = crossProductVec3(v, w);
	Vec3 N_normalized = normalizeVec3(N);

	Vec3 center;
	center.x = (v1.x + v2.x + v0.x) / 3;
	center.y = (v1.y + v2.y + v0.y) / 3;
	center.z = (v1.z + v2.z + v0.z) / 3;

	Vec3 eyeToTri;
	eyeToTri.x = center.x - camera->pos.x;
	eyeToTri.y = center.y - camera->pos.y;
	eyeToTri.z = center.z - camera->pos.z;

	//cout << "center.x - " << center.x << " camera.x " << camera->pos.x << " = " << eyeToTri.x << endl;
	//cout << "center.y - " << center.y << " camera.y " << camera->pos.y << " = " << eyeToTri.y << endl;
	//cout << "center.z - " << center.z << " camera.z " << camera->pos.z << " = " << eyeToTri.z << endl;

	cout << dotProductVec3(eyeToTri, N_normalized) << endl;

	if (dotProductVec3(eyeToTri, N_normalized) >= 0)
		backFacing = true;

	cout << " " << backFacing << endl;
	return backFacing;
}

/*
	Transformations, clipping, culling, rasterization are done here.
	You can define helper functions inside Scene class implementation.
*/
void Scene::forwardRenderingPipeline(Camera *camera)
{
	// TODO: Implement this function.
//	void Scene::transformAllModels() {
//
//		for (int m = 0; m < this->models.size(); m++) {
//			this->transformModel(m);
//		}
//	}
	if(!this->isTransformed){
		cout << "TRANSFORMING THE MODELS" << endl;
//		this->transformAllModels();
		this->isTransformed = true;
		this->saveVertices();
	}
	else{
		this->restoreVertices();
	}

	cout << "------------------------------------------------------" << endl;
	cout << "TRANSFORMING FOR CAMERA " << camera->cameraId << endl;
	
	
	Matrix4 M_camera = calculateCameraMatrix(camera);
	Matrix4 M_viewport = calculateViewportMatrix(camera);

	for (int modelIndex = 0; modelIndex < this->models.size(); modelIndex++){
		this->transformModel(modelIndex);
		//cout << "----Camera Transformation----"<< modelIndex << endl;
		for (int triIndex = 0; triIndex < this->models[modelIndex]->numberOfTriangles; triIndex++) {
			Triangle tri = this->models[modelIndex]->triangles[triIndex];
			cout << endl;
			cout << "Triangle " << triIndex << endl;

			if (this->cullingEnabled) {
				this->models[modelIndex]->triangles[triIndex].backFacing = !this->backfaceCulling(tri, camera);
				cout << "set as " << this->models[modelIndex]->triangles[triIndex].backFacing << endl;
			}
		}
		this->transformAllVertices(modelIndex, M_camera);

		// backface culling here

		

		cout << "----Projection Transformation----" << endl;
		if(this->projectionType == 0){
			Matrix4 M_ortographic = this->ortographicProjection(camera);
			this->transformAllVertices(modelIndex, M_ortographic);
		}
		else{
			this->perspectiveProjection(modelIndex, camera);
		}
		
		

		cout << "----Viewport Transformation----" << endl;
		this->viewportTransformation(modelIndex, M_viewport);

		for(int triIndex=0; triIndex < this->models[modelIndex]->numberOfTriangles; triIndex++){
			Triangle tri = this->models[modelIndex]->triangles[triIndex];
			if (this->models[modelIndex]->triangles[triIndex].backFacing == true){

				if(this->models[modelIndex]->type == 0){
					//cout << "Line rasterization for triangle: " << triIndex << endl;
					this->wireframeRasterization(tri, camera);
				}
				else{
					//TODO: segmentation fault in brazil flag 3 
					//cout << "Triangle rasterization for triangle: " << triIndex << endl;
					this->triangleRasterization(tri, camera);
				}
			}
		

		}
		this->restoreVertices();

	}
}

/*
	Parses XML file
*/
Scene::Scene(const char *xmlPath)
{
	const char *str;
	XMLDocument xmlDoc;
	XMLElement *pElement;

	xmlDoc.LoadFile(xmlPath);

	XMLNode *pRoot = xmlDoc.FirstChild();

	// read background color
	pElement = pRoot->FirstChildElement("BackgroundColor");
	str = pElement->GetText();
	sscanf(str, "%lf %lf %lf", &backgroundColor.r, &backgroundColor.g, &backgroundColor.b);

	// read culling
	pElement = pRoot->FirstChildElement("Culling");
	if (pElement != NULL)
		pElement->QueryBoolText(&cullingEnabled);

	// read projection type
	pElement = pRoot->FirstChildElement("ProjectionType");
	if (pElement != NULL)
		pElement->QueryIntText(&projectionType);

	// read cameras
	pElement = pRoot->FirstChildElement("Cameras");
	XMLElement *pCamera = pElement->FirstChildElement("Camera");
	XMLElement *camElement;
	while (pCamera != NULL)
	{
		Camera *cam = new Camera();

		pCamera->QueryIntAttribute("id", &cam->cameraId);

		camElement = pCamera->FirstChildElement("Position");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf", &cam->pos.x, &cam->pos.y, &cam->pos.z);

		camElement = pCamera->FirstChildElement("Gaze");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf", &cam->gaze.x, &cam->gaze.y, &cam->gaze.z);

		camElement = pCamera->FirstChildElement("Up");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf", &cam->v.x, &cam->v.y, &cam->v.z);

		cam->gaze = normalizeVec3(cam->gaze);
		cam->u = crossProductVec3(cam->gaze, cam->v);
		cam->u = normalizeVec3(cam->u);

		cam->w = inverseVec3(cam->gaze);
		cam->v = crossProductVec3(cam->u, cam->gaze);
		cam->v = normalizeVec3(cam->v);

		camElement = pCamera->FirstChildElement("ImagePlane");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf %lf %lf %lf %d %d",
			   &cam->left, &cam->right, &cam->bottom, &cam->top,
			   &cam->near, &cam->far, &cam->horRes, &cam->verRes);

		camElement = pCamera->FirstChildElement("OutputName");
		str = camElement->GetText();
		cam->outputFileName = string(str);

		cameras.push_back(cam);

		pCamera = pCamera->NextSiblingElement("Camera");
	}

	// read vertices
	pElement = pRoot->FirstChildElement("Vertices");
	XMLElement *pVertex = pElement->FirstChildElement("Vertex");
	int vertexId = 1;

	while (pVertex != NULL)
	{
		Vec3 *vertex = new Vec3();
		Color *color = new Color();

		vertex->colorId = vertexId;

		str = pVertex->Attribute("position");
		sscanf(str, "%lf %lf %lf", &vertex->x, &vertex->y, &vertex->z);

		str = pVertex->Attribute("color");
		sscanf(str, "%lf %lf %lf", &color->r, &color->g, &color->b);

		vertices.push_back(vertex);
		colorsOfVertices.push_back(color);

		pVertex = pVertex->NextSiblingElement("Vertex");

		vertexId++;
	}

	// read translations
	pElement = pRoot->FirstChildElement("Translations");
	XMLElement *pTranslation = pElement->FirstChildElement("Translation");
	while (pTranslation != NULL)
	{
		Translation *translation = new Translation();

		pTranslation->QueryIntAttribute("id", &translation->translationId);

		str = pTranslation->Attribute("value");
		sscanf(str, "%lf %lf %lf", &translation->tx, &translation->ty, &translation->tz);

		translations.push_back(translation);

		pTranslation = pTranslation->NextSiblingElement("Translation");
	}

	// read scalings
	pElement = pRoot->FirstChildElement("Scalings");
	XMLElement *pScaling = pElement->FirstChildElement("Scaling");
	while (pScaling != NULL)
	{
		Scaling *scaling = new Scaling();

		pScaling->QueryIntAttribute("id", &scaling->scalingId);
		str = pScaling->Attribute("value");
		sscanf(str, "%lf %lf %lf", &scaling->sx, &scaling->sy, &scaling->sz);

		scalings.push_back(scaling);

		pScaling = pScaling->NextSiblingElement("Scaling");
	}

	// read rotations
	pElement = pRoot->FirstChildElement("Rotations");
	XMLElement *pRotation = pElement->FirstChildElement("Rotation");
	while (pRotation != NULL)
	{
		Rotation *rotation = new Rotation();

		pRotation->QueryIntAttribute("id", &rotation->rotationId);
		str = pRotation->Attribute("value");
		sscanf(str, "%lf %lf %lf %lf", &rotation->angle, &rotation->ux, &rotation->uy, &rotation->uz);

		rotations.push_back(rotation);

		pRotation = pRotation->NextSiblingElement("Rotation");
	}

	// read models
	pElement = pRoot->FirstChildElement("Models");

	XMLElement *pModel = pElement->FirstChildElement("Model");
	XMLElement *modelElement;
	while (pModel != NULL)
	{
		Model *model = new Model();

		pModel->QueryIntAttribute("id", &model->modelId);
		pModel->QueryIntAttribute("type", &model->type);

		// read model transformations
		XMLElement *pTransformations = pModel->FirstChildElement("Transformations");
		XMLElement *pTransformation = pTransformations->FirstChildElement("Transformation");

		pTransformations->QueryIntAttribute("count", &model->numberOfTransformations);

		while (pTransformation != NULL)
		{
			char transformationType;
			int transformationId;

			str = pTransformation->GetText();
			sscanf(str, "%c %d", &transformationType, &transformationId);

			model->transformationTypes.push_back(transformationType);
			model->transformationIds.push_back(transformationId);

			pTransformation = pTransformation->NextSiblingElement("Transformation");
		}

		// read model triangles
		XMLElement *pTriangles = pModel->FirstChildElement("Triangles");
		XMLElement *pTriangle = pTriangles->FirstChildElement("Triangle");

		pTriangles->QueryIntAttribute("count", &model->numberOfTriangles);

		while (pTriangle != NULL)
		{
			int v1, v2, v3;

			str = pTriangle->GetText();
			sscanf(str, "%d %d %d", &v1, &v2, &v3);

			model->triangles.push_back(Triangle(v1, v2, v3));

			pTriangle = pTriangle->NextSiblingElement("Triangle");
		}

		models.push_back(model);

		pModel = pModel->NextSiblingElement("Model");
	}
}

/*
	Initializes image with background color
*/
void Scene::initializeImage(Camera *camera)
{
	if (this->image.empty())
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			vector<Color> rowOfColors;

			for (int j = 0; j < camera->verRes; j++)
			{
				rowOfColors.push_back(this->backgroundColor);
			}

			this->image.push_back(rowOfColors);
		}
	}
	// if image is filled before, just change color rgb values with the background color
	else
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			for (int j = 0; j < camera->verRes; j++)
			{
				this->image[i][j].r = this->backgroundColor.r;
				this->image[i][j].g = this->backgroundColor.g;
				this->image[i][j].b = this->backgroundColor.b;
			}
		}
	}
}

/*
	If given value is less than 0, converts value to 0.
	If given value is more than 255, converts value to 255.
	Otherwise returns value itself.
*/
int Scene::makeBetweenZeroAnd255(double value)
{
	if (value >= 255.0)
		return 255;
	if (value <= 0.0)
		return 0;
	return (int)(value);
}

/*
	Writes contents of image (Color**) into a PPM file.
*/
void Scene::writeImageToPPMFile(Camera *camera)
{
	ofstream fout;

	fout.open(camera->outputFileName.c_str());

	fout << "P3" << endl;
	fout << "# " << camera->outputFileName << endl;
	fout << camera->horRes << " " << camera->verRes << endl;
	fout << "255" << endl;

	for (int j = camera->verRes - 1; j >= 0; j--)
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			fout << makeBetweenZeroAnd255(this->image[i][j].r) << " "
				 << makeBetweenZeroAnd255(this->image[i][j].g) << " "
				 << makeBetweenZeroAnd255(this->image[i][j].b) << " ";
		}
		fout << endl;
	}
	fout.close();
}

/*
	Converts PPM image in given path to PNG file, by calling ImageMagick's 'convert' command.
	os_type == 1 		-> Ubuntu
	os_type == 2 		-> Windows
	os_type == other	-> No conversion
*/
void Scene::convertPPMToPNG(string ppmFileName, int osType)
{
	string command;

	// call command on Ubuntu
	if (osType == 1)
	{
		command = "convert " + ppmFileName + " " + ppmFileName + ".png";
		system(command.c_str());
	}

	// call command on Windows
	else if (osType == 2)
	{
		command = "wsl convert " + ppmFileName + " " + ppmFileName + ".png";
		system(command.c_str());
	}

	// default action - don't do conversion
	else
	{
	}
}
