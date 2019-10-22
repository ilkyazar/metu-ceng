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
	this->id = id;
    this->matIndex = matIndex;
    this->p1Index = p1Index;
    this->p2Index = p2Index;
    this->p3Index = p3Index;

}

/* Triangle-ray intersection routine. You will implement this. 
Note that ReturnVal structure should hold the information related to the intersection point, e.g., coordinate of that point, normal at that point etc. 
You should to declare the variables in ReturnVal structure you think you will need. It is in defs.h file. */
ReturnVal Triangle::intersect(const Ray & ray) const
{
    ReturnVal returnVal;
    
    Vector n;
    Vector a(pScene->vertices[this->p1Index - 1].x, pScene->vertices[this->p1Index - 1].y, pScene->vertices[this->p1Index - 1].z);
    Vector b(pScene->vertices[this->p2Index - 1].x, pScene->vertices[this->p2Index - 1].y, pScene->vertices[this->p2Index - 1].z);
    Vector c(pScene->vertices[this->p3Index - 1].x, pScene->vertices[this->p3Index - 1].y, pScene->vertices[this->p3Index - 1].z);

    n = (a - b).cross(c - b);

    Vector d(ray.direction.x, ray.direction.y, ray.direction.z);
    if (d.dot(n) < pScene->intTestEps) { // Triangle object and ray is parallel to each other.
        //std::cout << "parallel " << d.dot(n) << std::endl;
        returnVal.isIntersect = false;
    }

    Vector o(ray.origin.x, ray.origin.y, ray.origin.z);

    float t = (a - o).dot(n) / d.dot(n);
    Vector3f point = ray.getPoint(t);
    Vector p(point.x, point.y, point.z);

    if (((c-a).cross(p-a)).dot(n) > pScene->intTestEps &&
        ((a-b).cross(p-b)).dot(n) > pScene->intTestEps&&
        ((b-c).cross(p-c)).dot(n) > pScene->intTestEps) {
            returnVal.isIntersect = true;
            returnVal.intersectCoord = p;
        }
    else {
        returnVal.isIntersect = false;
    }
    return returnVal;
}

Mesh::Mesh()
{}

/* Constructor for mesh. You will implement this. */
Mesh::Mesh(int id, int matIndex, const vector<Triangle>& faces)
    : Shape(id, matIndex)
{
	this->id = id;
    this->matIndex = matIndex;
    this->faces = faces;

}

/* Mesh-ray intersection routine. You will implement this. 
Note that ReturnVal structure should hold the information related to the intersection point, e.g., coordinate of that point, normal at that point etc. 
You should to declare the variables in ReturnVal structure you think you will need. It is in defs.h file. */
ReturnVal Mesh::intersect(const Ray & ray) const
{
    ReturnVal returnValTri, returnValMesh;
    returnValMesh.isIntersect = false;

    for (int triIndex = 0; triIndex < this->faces.size(); triIndex++) {
        returnValTri = this->faces[triIndex].intersect(ray);
        if (returnValTri.isIntersect){
            returnValMesh.isIntersect = true;
        } 
    }
    return returnValMesh;
}
