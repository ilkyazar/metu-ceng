#include "Scene.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "Shape.h"
#include "tinyxml2.h"
#include <cstring>
#include <limits>

using namespace tinyxml2;

Ray Scene::computeShadowRay(Vector3f p, Ray viewingRay, Vector3f lightPosition) {
	Vector3f shadowRayOrigin, shadowRayDirection;
	shadowRayDirection.x = lightPosition.x - p.x;
	shadowRayDirection.y = lightPosition.y - p.y;
	shadowRayDirection.z = lightPosition.z - p.z;
	shadowRayOrigin.x = p.x + this->shadowRayEps * shadowRayDirection.x;
	shadowRayOrigin.y = p.y + this->shadowRayEps * shadowRayDirection.y;
	shadowRayOrigin.z = p.z + this->shadowRayEps * shadowRayDirection.z;

	Ray shadowRay(shadowRayOrigin, shadowRayDirection);
	return shadowRay;
}

Color Scene::traverseLights(Vector3f p, ReturnVal returnVal, int objIndex, Ray viewingRay, int recDepth) {
	
	Color color;

	if(recDepth < 0){
		color = {0,0,0};
		return color;
	}
	
	Vector3f diffuse, ambient, specular;
	Material* material;
	material = this->materials[this->objects[objIndex]->matIndex-1];

	ambient.r = this->ambientLight.x * material->ambientRef.r;
	ambient.g = this->ambientLight.y * material->ambientRef.g;
	ambient.b = this->ambientLight.z * material->ambientRef.b;

	color.channel[0] = ambient.r;
	color.channel[1] = ambient.g;
	color.channel[2] = ambient.b;

	for (int l = 0; l < this->lights.size(); l++) {
		bool inShadow = false;
		Vector3f lightPosition = this->lights[l]->position;
		Ray shadowRay = this->computeShadowRay(p, viewingRay, lightPosition);

		for (int o = 0; o < this->objects.size(); o++) {
			ReturnVal shadowRayReturnVal = this->objects[o]->intersect(shadowRay);
			Vector3f shadowRayP = {shadowRayReturnVal.intersectCoord.x, 
									shadowRayReturnVal.intersectCoord.y,
									shadowRayReturnVal.intersectCoord.z};
			if (shadowRayReturnVal.isIntersect &&
				shadowRay.gett(shadowRayP) < shadowRay.gett(lightPosition)
				) {
				inShadow = true;
				continue;
			}
		}
		
		diffuse = {0,0,0};
		specular = {0,0,0};

		if(!inShadow){
			Vector w_i(lightPosition.x - p.x, lightPosition.y - p.y, lightPosition.z - p.z);
			w_i = w_i.normalize(w_i);

			Vector3f irradiance = this->lights[l]->computeLightContribution(p);
			
			Vector3f diffuseRefCoeff = material->diffuseRef;

			Vector N = returnVal.normalVec;
			N = N.normalize(N);
			float cos_diffuse = max(0.f, w_i.dot(N));

			diffuse.r = diffuseRefCoeff.x * cos_diffuse * irradiance.x;
			diffuse.g = diffuseRefCoeff.y * cos_diffuse * irradiance.y;
			diffuse.b = diffuseRefCoeff.z * cos_diffuse * irradiance.z;
			
			Vector h;
			Vector w_o(-viewingRay.direction.x, -viewingRay.direction.y, -viewingRay.direction.z);
			w_o = w_o.normalize(w_o);
			h = (w_i + w_o) / (w_i + w_o).getMagnitude();

			float cos_specular = pow(max(0.f, N.dot(h)), material->phongExp);

			Vector3f specularRefCoeff = material->specularRef;

			specular.r = specularRefCoeff.r * cos_specular * irradiance.x;
			specular.g = specularRefCoeff.g * cos_specular * irradiance.y;
			specular.b = specularRefCoeff.b * cos_specular * irradiance.z;
			
		}

		color.channel[0] = (ambient.r + diffuse.r + specular.r) > 255 ? 255 : (ambient.r + diffuse.r + specular.r);
		color.channel[1] = (ambient.g + diffuse.g + specular.g) > 255 ? 255 : (ambient.g + diffuse.g + specular.g);
		color.channel[2] = (ambient.b + diffuse.b + specular.b) > 255 ? 255 : (ambient.b + diffuse.b + specular.b);
		//std::cout << "color = " << static_cast<unsigned>(color.red) << " " << static_cast<unsigned>(color.grn) << " " << static_cast<unsigned>(color.blu) << std::endl;
		
	}
	//calculate reflection
	if(material->mirrorRef.r != 0 || material->mirrorRef.g != 0 || material->mirrorRef.b != 0  ){
		Vector w_o(-viewingRay.direction.x, -viewingRay.direction.y, -viewingRay.direction.z);
		w_o = w_o.normalize(w_o);
		Vector n = returnVal.normalVec;
		n = n.normalize(n);

		Vector w_r = w_o*(-1) + (n*2)*(n.dot(w_o));
		Vector3f mirrorRayOrigin, mirrorRayDirection;

		mirrorRayDirection = {w_r.x, w_r.y, w_r.z };
		mirrorRayOrigin = {p.x + this->shadowRayEps * mirrorRayDirection.x,
						   p.y + this->shadowRayEps * mirrorRayDirection.y,
						   p.z + this->shadowRayEps * mirrorRayDirection.z};
		Ray mirrorRay(mirrorRayOrigin, mirrorRayDirection); 
		
		float t_min = std::numeric_limits<float>::max();
		for(int obj = 0; obj < this->objects.size(); obj++ ){
			ReturnVal returnValMirror = this->objects[obj]->intersect(mirrorRay);
			if(returnValMirror.isIntersect){
				Vector3f p_temp = {returnValMirror.intersectCoord.x, returnValMirror.intersectCoord.y, returnValMirror.intersectCoord.z };

				if (mirrorRay.gett(p_temp) < t_min){
					t_min = mirrorRay.gett(p_temp);
					p = p_temp;
					Color reflection = this->traverseLights(p, returnValMirror, obj, mirrorRay, recDepth-1);
					color.red = color.red + material->mirrorRef.r * reflection.red > 255 ? 255 : color.red + material->mirrorRef.r * reflection.red;
					color.grn = color.grn + material->mirrorRef.r * reflection.grn > 255 ? 255 : color.grn + material->mirrorRef.r * reflection.grn;
					color.blu = color.blu + material->mirrorRef.r * reflection.blu > 255 ? 255 : color.blu + material->mirrorRef.r * reflection.blu;	 
				}
			}
		}


	}
	return color;
}

Color Scene::traverseObjects(int i, int j, int cameraIndex) {

	float t_min = std::numeric_limits<int>::max();
	Ray primaryRay = cameras[cameraIndex]->getPrimaryRay(i, j);
	bool rayIntersectedWithObj = false;
	Color color;
	Vector3f p;
	
	for (int obj = 0; obj < this->objects.size(); obj++) {
		ReturnVal returnVal = this->objects[obj]->intersect(primaryRay);
		if (returnVal.isIntersect) {
			Vector3f p_temp;
			p_temp.x = returnVal.intersectCoord.x;
			p_temp.y = returnVal.intersectCoord.y;
			p_temp.z = returnVal.intersectCoord.z;	

			if (primaryRay.gett(p_temp) < t_min) {
				t_min = primaryRay.gett(p_temp);
				rayIntersectedWithObj = true;
				p = p_temp;
				color = this->traverseLights(p, returnVal, obj, primaryRay, this->maxRecursionDepth);
			}
		}
	}

	if (rayIntersectedWithObj == false) {
		color.red = this->backgroundColor.x;
		color.grn = this->backgroundColor.y;
		color.blu = this->backgroundColor.z;
	}

	return color;
}
/* 
 * Must render the scene from each camera's viewpoint and create an image.
 * You can use the methods of the Image class to save the image as a PPM file. 
 */
void Scene::renderScene(void)
{
	Color color;

	for (int cameraIndex = 0; cameraIndex < cameras.size(); cameraIndex++) {
		const char* outputImgName = this->cameras[cameraIndex]->imageName;
		int nx = this->cameras[cameraIndex]->imgPlane.nx;
		int ny = this->cameras[cameraIndex]->imgPlane.ny;

		Image* outputImage = new Image(nx, ny);

		for (int i = 0; i < nx; i++) {
			for (int j = 0; j < ny; j++) {		
				color = this->traverseObjects(i, j, cameraIndex);
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

