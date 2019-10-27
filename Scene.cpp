#include "Scene.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "Shape.h"
#include "tinyxml2.h"
#include "Image.h"
#include <cstring>
#include <limits>

using namespace tinyxml2;

/* 
 * Must render the scene from each camera's viewpoint and create an image.
 * You can use the methods of the Image class to save the image as a PPM file. 
 */
void Scene::renderScene(void)
{
	Color color;

	for (int cameraIndex = 0; cameraIndex < cameras.size(); cameraIndex++) {
		int id = cameras[cameraIndex]->id;
		const char* outputImgName = this->cameras[cameraIndex]->imageName;
		int nx = this->cameras[cameraIndex]->imgPlane.nx;
		int ny = this->cameras[cameraIndex]->imgPlane.ny;

		Image* outputImage = new Image(nx, ny);
		Ray primaryRay;
		Vector3f p;
		ReturnVal returnVal;

		for (int i = 0; i < nx; i++) {
			for (int j = 0; j < ny; j++) {
				primaryRay = cameras[cameraIndex]->getPrimaryRay(i, j);
				float t_min = std::numeric_limits<int>::max();
				bool obj = false;
				int obj_i;
				for (int objIndex = 0; objIndex < this->objects.size(); objIndex++) {
					returnVal = this->objects[objIndex]->intersect(primaryRay);
					if (returnVal.isIntersect == true) {
						Vector3f p_temp;
						p_temp.x = returnVal.intersectCoord.x;
						p_temp.y = returnVal.intersectCoord.y;
						p_temp.z = returnVal.intersectCoord.z;
						if (primaryRay.gett(p_temp) < t_min) {
							p = p_temp;
							t_min = primaryRay.gett(p);
							obj = true;
							obj_i = objIndex;
						}
					}
				}

				if (obj) {
					Vector3f ambient, diffuse, specular;
					float cos_diffuse, cos_specular;
					ambient.r = this->ambientLight.x * this->materials[this->objects[obj_i]->matIndex-1]->ambientRef.r;
					ambient.g = this->ambientLight.y * this->materials[this->objects[obj_i]->matIndex-1]->ambientRef.g;
					ambient.b = this->ambientLight.z * this->materials[this->objects[obj_i]->matIndex-1]->ambientRef.b;					

					color.red = ambient.r;
					color.grn = ambient.g;
					color.blu = ambient.b;

					for (int l = 0; l < this->lights.size(); l++) {
						// compute the shadow ray s from intersectCoord to l
						Vector3f shadowRayOrigin, shadowRayDirection;
						shadowRayOrigin.x = p.x + this->shadowRayEps * primaryRay.direction.x;
						shadowRayOrigin.y = p.y + this->shadowRayEps * primaryRay.direction.y;
						shadowRayOrigin.z = p.z + this->shadowRayEps * primaryRay.direction.z;
						shadowRayDirection.x = this->lights[l]->position.x - p.x;
						shadowRayDirection.y = this->lights[l]->position.y - p.y;
						shadowRayDirection.z = this->lights[l]->position.z - p.z;
						Ray shadowRay(shadowRayOrigin, shadowRayDirection);
						
						for (int o = 0; o < this->objects.size(); o++) {
							// if s intersects the o before the light source, continue loop, because point is in shadow
							ReturnVal returnVal_ray = this->objects[o]->intersect(shadowRay);
							if (returnVal_ray.isIntersect &&
								shadowRay.gett(p) < primaryRay.gett(p) ) {
								continue;
							}
							// color += diffuse + specular
							Vector3f contribution = this->lights[l]->computeLightContribution(p);
							Vector contr(contribution.x, contribution.y, contribution.z);

							Vector w_i(this->lights[l]->position.x - p.x, this->lights[l]->position.y - p.y, this->lights[l]->position.z - p.z);
							//w_i = w_i.normalize(w_i);
							Vector normal(returnVal.normalVec.x, returnVal.normalVec.y, returnVal.normalVec.z);
							//normal = normal.normalize(normal);
							//cos_diffuse = w_i.dot(normal);
							cos_diffuse = (-w_i.dot(normal)) / (w_i.getMagnitude() * normal.getMagnitude());
							//cos_diffuse = std::max(0.f, std::min(cos_diffuse, 1.f));

							diffuse.r = (contr.x * this->materials[this->objects[obj_i]->matIndex-1]->diffuseRef.r * cos_diffuse);
							diffuse.g = (contr.y * this->materials[this->objects[obj_i]->matIndex-1]->diffuseRef.g * cos_diffuse);
							diffuse.b = (contr.z * this->materials[this->objects[obj_i]->matIndex-1]->diffuseRef.b * cos_diffuse);

							color.red = (diffuse.r + ambient.r) > 255 ? 255 : (diffuse.r + ambient.r);
							color.grn = (diffuse.r + ambient.g) > 255 ? 255 : (diffuse.g + ambient.g);
							color.blu = (diffuse.r + ambient.b) > 255 ? 255 : (diffuse.b + ambient.b);

							//std::cout << "color = " << static_cast<unsigned>(color.red) << " " << static_cast<unsigned>(color.grn) << " " << static_cast<unsigned>(color.blu) << std::endl;
						}
					}
					
				}
				else {
					color.red = this->backgroundColor.x;
					color.grn = this->backgroundColor.y;
					color.blu = this->backgroundColor.z;				
				}				
				outputImage->setPixelValue(j, i, color);
			}
		}
		outputImage->saveImage(outputImgName);
	}


}

// Parses XML file. 
Scene::Scene(const char *xmlPath)
{
	const char *str;
	XMLDocument xmlDoc;
	XMLError eResult;
	XMLElement *pElement;

	maxRecursionDepth = 1;
	shadowRayEps = 0.001;

	eResult = xmlDoc.LoadFile(xmlPath);

	XMLNode *pRoot = xmlDoc.FirstChild();

	pElement = pRoot->FirstChildElement("MaxRecursionDepth");
	if(pElement != nullptr)
		pElement->QueryIntText(&maxRecursionDepth);

	pElement = pRoot->FirstChildElement("BackgroundColor");
	str = pElement->GetText();
	sscanf(str, "%f %f %f", &backgroundColor.r, &backgroundColor.g, &backgroundColor.b);

	pElement = pRoot->FirstChildElement("ShadowRayEpsilon");
	if(pElement != nullptr)
		pElement->QueryFloatText(&shadowRayEps);

	pElement = pRoot->FirstChildElement("IntersectionTestEpsilon");
	if(pElement != nullptr)
		eResult = pElement->QueryFloatText(&intTestEps);

	// Parse cameras
	pElement = pRoot->FirstChildElement("Cameras");
	XMLElement *pCamera = pElement->FirstChildElement("Camera");
	XMLElement *camElement;
	while(pCamera != nullptr)
	{
        int id;
        char imageName[64];
        Vector3f pos, gaze, up;
        ImagePlane imgPlane;

		eResult = pCamera->QueryIntAttribute("id", &id);
		camElement = pCamera->FirstChildElement("Position");
		str = camElement->GetText();
		sscanf(str, "%f %f %f", &pos.x, &pos.y, &pos.z);
		camElement = pCamera->FirstChildElement("Gaze");
		str = camElement->GetText();
		sscanf(str, "%f %f %f", &gaze.x, &gaze.y, &gaze.z);
		camElement = pCamera->FirstChildElement("Up");
		str = camElement->GetText();
		sscanf(str, "%f %f %f", &up.x, &up.y, &up.z);
		camElement = pCamera->FirstChildElement("NearPlane");
		str = camElement->GetText();
		sscanf(str, "%f %f %f %f", &imgPlane.left, &imgPlane.right, &imgPlane.bottom, &imgPlane.top);
		camElement = pCamera->FirstChildElement("NearDistance");
		eResult = camElement->QueryFloatText(&imgPlane.distance);
		camElement = pCamera->FirstChildElement("ImageResolution");	
		str = camElement->GetText();
		sscanf(str, "%d %d", &imgPlane.nx, &imgPlane.ny);
		camElement = pCamera->FirstChildElement("ImageName");
		str = camElement->GetText();
		strcpy(imageName, str);

		cameras.push_back(new Camera(id, imageName, pos, gaze, up, imgPlane));

		pCamera = pCamera->NextSiblingElement("Camera");
	}

	// Parse materals
	pElement = pRoot->FirstChildElement("Materials");
	XMLElement *pMaterial = pElement->FirstChildElement("Material");
	XMLElement *materialElement;
	while(pMaterial != nullptr)
	{
		materials.push_back(new Material());

		int curr = materials.size() - 1;
	
		eResult = pMaterial->QueryIntAttribute("id", &materials[curr]->id);
		materialElement = pMaterial->FirstChildElement("AmbientReflectance");
		str = materialElement->GetText();
		sscanf(str, "%f %f %f", &materials[curr]->ambientRef.r, &materials[curr]->ambientRef.g, &materials[curr]->ambientRef.b);
		materialElement = pMaterial->FirstChildElement("DiffuseReflectance");
		str = materialElement->GetText();
		sscanf(str, "%f %f %f", &materials[curr]->diffuseRef.r, &materials[curr]->diffuseRef.g, &materials[curr]->diffuseRef.b);
		materialElement = pMaterial->FirstChildElement("SpecularReflectance");
		str = materialElement->GetText();
		sscanf(str, "%f %f %f", &materials[curr]->specularRef.r, &materials[curr]->specularRef.g, &materials[curr]->specularRef.b);
		materialElement = pMaterial->FirstChildElement("MirrorReflectance");
		if(materialElement != nullptr)
		{
			str = materialElement->GetText();
			sscanf(str, "%f %f %f", &materials[curr]->mirrorRef.r, &materials[curr]->mirrorRef.g, &materials[curr]->mirrorRef.b);
		}
				else
		{
			materials[curr]->mirrorRef.r = 0.0;
			materials[curr]->mirrorRef.g = 0.0;
			materials[curr]->mirrorRef.b = 0.0;
		}
		materialElement = pMaterial->FirstChildElement("PhongExponent");
		if(materialElement != nullptr)
			materialElement->QueryIntText(&materials[curr]->phongExp);

		pMaterial = pMaterial->NextSiblingElement("Material");
	}

	// Parse vertex data
	pElement = pRoot->FirstChildElement("VertexData");
	int cursor = 0;
	Vector3f tmpPoint;
	str = pElement->GetText();
	while(str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
		cursor++;
	while(str[cursor] != '\0')
	{
		for(int cnt = 0 ; cnt < 3 ; cnt++)
		{
			if(cnt == 0)
				tmpPoint.x = atof(str + cursor);
			else if(cnt == 1)
				tmpPoint.y = atof(str + cursor);
			else
				tmpPoint.z = atof(str + cursor);
			while(str[cursor] != ' ' && str[cursor] != '\t' && str[cursor] != '\n')
				cursor++; 
			while(str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
				cursor++;
		}
		vertices.push_back(tmpPoint);
	}

	// Parse objects
	pElement = pRoot->FirstChildElement("Objects");
	
	// Parse spheres
	XMLElement *pObject = pElement->FirstChildElement("Sphere");
	XMLElement *objElement;
	while(pObject != nullptr)
	{
		int id;
		int matIndex;
		int cIndex;
		float R;

		eResult = pObject->QueryIntAttribute("id", &id);
		objElement = pObject->FirstChildElement("Material");
		eResult = objElement->QueryIntText(&matIndex);
		objElement = pObject->FirstChildElement("Center");
		eResult = objElement->QueryIntText(&cIndex);
		objElement = pObject->FirstChildElement("Radius");
		eResult = objElement->QueryFloatText(&R);

		objects.push_back(new Sphere(id, matIndex, cIndex, R, &vertices));

		pObject = pObject->NextSiblingElement("Sphere");
	}

	// Parse triangles
	pObject = pElement->FirstChildElement("Triangle");
	while(pObject != nullptr)
	{
		int id;
		int matIndex;
		int p1Index;
		int p2Index;
		int p3Index;

		eResult = pObject->QueryIntAttribute("id", &id);
		objElement = pObject->FirstChildElement("Material");
		eResult = objElement->QueryIntText(&matIndex);
		objElement = pObject->FirstChildElement("Indices");
		str = objElement->GetText();
		sscanf(str, "%d %d %d", &p1Index, &p2Index, &p3Index);

		objects.push_back(new Triangle(id, matIndex, p1Index, p2Index, p3Index, &vertices));

		pObject = pObject->NextSiblingElement("Triangle");
	}

	// Parse meshes
	pObject = pElement->FirstChildElement("Mesh");
	while(pObject != nullptr)
	{
		int id;
		int matIndex;
		int p1Index;
		int p2Index;
		int p3Index;
		int cursor = 0;
		int vertexOffset = 0;
		vector<Triangle> faces;
		vector<int> *meshIndices = new vector<int>;

		eResult = pObject->QueryIntAttribute("id", &id);
		objElement = pObject->FirstChildElement("Material");
		eResult = objElement->QueryIntText(&matIndex);
		objElement = pObject->FirstChildElement("Faces");
		objElement->QueryIntAttribute("vertexOffset", &vertexOffset);
		str = objElement->GetText();
		while(str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
			cursor++;
		while(str[cursor] != '\0')
		{
			for(int cnt = 0 ; cnt < 3 ; cnt++)
			{
				if(cnt == 0)
					p1Index = atoi(str + cursor) + vertexOffset;
				else if(cnt == 1)
					p2Index = atoi(str + cursor) + vertexOffset;
				else
					p3Index = atoi(str + cursor) + vertexOffset;
				while(str[cursor] != ' ' && str[cursor] != '\t' && str[cursor] != '\n')
					cursor++; 
				while(str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
					cursor++;
			}
			faces.push_back(*(new Triangle(-1, matIndex, p1Index, p2Index, p3Index, &vertices)));
			meshIndices->push_back(p1Index);
			meshIndices->push_back(p2Index);
			meshIndices->push_back(p3Index);
		}

		objects.push_back(new Mesh(id, matIndex, faces, meshIndices, &vertices));

		pObject = pObject->NextSiblingElement("Mesh");
	}

	// Parse lights
	int id;
	Vector3f position;
	Vector3f intensity;
	pElement = pRoot->FirstChildElement("Lights");

	XMLElement *pLight = pElement->FirstChildElement("AmbientLight");
	XMLElement *lightElement;
	str = pLight->GetText();
	sscanf(str, "%f %f %f", &ambientLight.r, &ambientLight.g, &ambientLight.b);

	pLight = pElement->FirstChildElement("PointLight");
	while(pLight != nullptr)
	{
		eResult = pLight->QueryIntAttribute("id", &id);
		lightElement = pLight->FirstChildElement("Position");
		str = lightElement->GetText();
		sscanf(str, "%f %f %f", &position.x, &position.y, &position.z);
		lightElement = pLight->FirstChildElement("Intensity");
		str = lightElement->GetText();
		sscanf(str, "%f %f %f", &intensity.r, &intensity.g, &intensity.b);

		lights.push_back(new PointLight(position, intensity));

		pLight = pLight->NextSiblingElement("PointLight");
	}
}

