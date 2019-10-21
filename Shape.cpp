#include "Shape.h"
#include "Scene.h"
#include <cstdio>
#include <math.h>

Shape::Shape(void)
{
}

Shape::Shape(int id, int matIndex)
    : id(id), matIndex(matIndex)
{
}

Sphere::Sphere(void)
{}

/* Constructor for sphere. You will implement this. */
Sphere::Sphere(int id, int matIndex, int cIndex, float R)
    : Shape(id, matIndex)
{
	this->id = id;
    this->matIndex = matIndex;
    this->cIndex = cIndex;
    this->R = R;
}

/* Sphere-ray intersection routine. You will implement this. 
Note that ReturnVal structure should hold the information related to the intersection point, e.g., coordinate of that point, normal at that point etc. 
You should to declare the variables in ReturnVal structure you think you will need. It is in defs.h file. */
ReturnVal Sphere::intersect(const Ray & ray) const
{
    ReturnVal returnVal;

    Vector o(ray.origin.x, ray.origin.y, ray.origin.z);
    Vector d(ray.direction.x, ray.direction.y, ray.direction.z);

    Vector center(pScene->vertices[this->cIndex-1].x, pScene->vertices[this->cIndex-1].y, pScene->vertices[this->cIndex-1].z);

    Vector v = o - center;

    float a = d.dot(d);
    float b = 2 * v.dot(d);
    float c = v.dot(v) - this->R * this->R;

    float delta = (b*b) - (4*a*c);

    if (delta < pScene->intTestEps){
        returnVal.isIntersect = false;
    }

    else{

        float t1 = (-b - sqrt(delta))/(2*a);
        float t2 = (-b + sqrt(delta))/(2*a);

        float t = (t1 < t2) ? t1 : t2;
        returnVal.intersectCoord = o + d * t;
        //returnVal.normalVec = ??
        returnVal.isIntersect = true;
    }
    return returnVal;
}

Triangle::Triangle(void)
{}

/* Constructor for triangle. You will implement this. */
Triangle::Triangle(int id, int matIndex, int p1Index, int p2Index, int p3Index)
    : Shape(id, matIndex)
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */
}

/* Triangle-ray intersection routine. You will implement this. 
Note that ReturnVal structure should hold the information related to the intersection point, e.g., coordinate of that point, normal at that point etc. 
You should to declare the variables in ReturnVal structure you think you will need. It is in defs.h file. */
ReturnVal Triangle::intersect(const Ray & ray) const
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */
}

Mesh::Mesh()
{}

/* Constructor for mesh. You will implement this. */
Mesh::Mesh(int id, int matIndex, const vector<Triangle>& faces)
    : Shape(id, matIndex)
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */
}

/* Mesh-ray intersection routine. You will implement this. 
Note that ReturnVal structure should hold the information related to the intersection point, e.g., coordinate of that point, normal at that point etc. 
You should to declare the variables in ReturnVal structure you think you will need. It is in defs.h file. */
ReturnVal Mesh::intersect(const Ray & ray) const
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */
}
