#include "Shape.h"
#include "Scene.h"
#include <cstdio>
#include <math.h>
#include <limits>

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
Sphere::Sphere(int id, int matIndex, int cIndex, float R, vector<Vector3f> *pVertices)
    : Shape(id, matIndex)
{
	this->id = id;
    this->matIndex = matIndex;
    this->cIndex = cIndex;
    this->R = R;
    this->vertices = pVertices;
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
Triangle::Triangle(int id, int matIndex, int p1Index, int p2Index, int p3Index, vector<Vector3f> *pVertices)
    : Shape(id, matIndex)
{
	this->id = id;
    this->matIndex = matIndex;
    this->p1Index = p1Index;
    this->p2Index = p2Index;
    this->p3Index = p3Index;
    this->vertices = pVertices;
}

/* Triangle-ray intersection routine. You will implement this. 
Note that ReturnVal structure should hold the information related to the intersection point, e.g., coordinate of that point, normal at that point etc. 
You should to declare the variables in ReturnVal structure you think you will need. It is in defs.h file. */
ReturnVal Triangle::intersect(const Ray & ray) const
{
	ReturnVal returnVal;
    Vector n;
    Vector v0(pScene->vertices[this->p1Index - 1].x, pScene->vertices[this->p1Index - 1].y, pScene->vertices[this->p1Index - 1].z);
    Vector v1(pScene->vertices[this->p2Index - 1].x, pScene->vertices[this->p2Index - 1].y, pScene->vertices[this->p2Index - 1].z);
    Vector v2(pScene->vertices[this->p3Index - 1].x, pScene->vertices[this->p3Index - 1].y, pScene->vertices[this->p3Index - 1].z);
    n = (v1-v0).cross(v2-v0);
    //float d = n.dot(v0);
    Vector origin(ray.origin.x, ray.origin.y, ray.origin.z);
    Vector direction(ray.direction.x, ray.direction.y, ray.direction.z);

    //float t = (n.dot(origin) + d) / n.dot(direction); 
    float t = n.dot(v0 - origin) / direction.dot(n);
    
    Vector3f point = ray.getPoint(t);

    Vector p(point.x, point.y, point.z);

    if (/*fabs(direction.dot(n)) < pScene->intTestEps*/ direction.dot(n) == 0) { // ray and the triangle are parallel
        returnVal.isIntersect = false;
        return returnVal;
    }
    if (t < 0) { // the triangle is behind 
        returnVal.isIntersect = false;
        return returnVal;
    }

    Vector c;
    Vector edge0 = v1 - v0;
    Vector vp0 = p - v0;
    c = edge0.cross(vp0);
    if (n.dot(c) <= 0) {
        returnVal.isIntersect = false;
        return returnVal;
    }

    Vector edge1 = v2 - v1;
    Vector vp1 = p - v1;
    c = edge1.cross(vp1);
    if (n.dot(c) <= 0) {
        returnVal.isIntersect = false;
        return returnVal;
    }

    Vector edge2 = v0 - v2;
    Vector vp2 = p - v2;
    c = edge2.cross(vp2);
    if (n.dot(c) <= 0) {
        returnVal.isIntersect = false;
        return returnVal;
    }

    returnVal.isIntersect = true;
    returnVal.intersectCoord = p;
    return returnVal;
}

Mesh::Mesh()
{}

/* Constructor for mesh. You will implement this. */
Mesh::Mesh(int id, int matIndex, const vector<Triangle>& faces, vector<int> *pIndices, vector<Vector3f> *pVertices)
    : Shape(id, matIndex)
{
	this->id = id;
    this->matIndex = matIndex;
    this->faces = faces;
    this->pIndices = pIndices;
    this->vertices = pVertices; 
}

/* Mesh-ray intersection routine. You will implement this. 
Note that ReturnVal structure should hold the information related to the intersection point, e.g., coordinate of that point, normal at that point etc. 
You should to declare the variables in ReturnVal structure you think you will need. It is in defs.h file. */
ReturnVal Mesh::intersect(const Ray & ray) const
{
	ReturnVal returnVal, returnValTri;
    returnVal.isIntersect = false; 
    
    for(int triIndex=0; triIndex < this->faces.size(); triIndex++){
        int v0_index = pIndices[0][3*triIndex];
        int v1_index = pIndices[0][3*triIndex + 1];
        int v2_index = pIndices[0][3*triIndex + 2];
        
        /*
        Vector v0( vertices[0][v0_index].x, vertices[0][v0_index].y, vertices[0][v0_index].z );
        Vector v1( vertices[0][v1_index].x, vertices[0][v1_index].y, vertices[0][v1_index].z );
        Vector v2( vertices[0][v2_index].x, vertices[0][v2_index].y, vertices[0][v2_index].z );
        */
        float t_min = std::numeric_limits<int>::max();
        Triangle tri(-1, -1, v0_index, v1_index, v2_index, nullptr);
        returnValTri = tri.intersect(ray);
        Vector3f triCoord;
        triCoord.x = returnValTri.intersectCoord.x;
        triCoord.y = returnValTri.intersectCoord.y;
        triCoord.z = returnValTri.intersectCoord.z; 

        if(returnValTri.isIntersect && (ray.gett(triCoord) < t_min) ){
            returnVal.isIntersect = true;
            t_min = ray.gett(triCoord);
            returnVal.intersectCoord = returnValTri.intersectCoord;

        }
    }
    return returnVal;

}
