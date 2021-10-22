#ifndef _SHAPE_H_
#define _SHAPE_H_

#include <vector>
#include "Ray.h"
#include "defs.h"

using namespace std;

// Base class for any shape object
class Shape
{
public: 
	int id;	        // Id of the shape
	int matIndex;	// Material index of the shape

	virtual ReturnVal intersect(const Ray & ray) const = 0; // Pure virtual method for intersection test. You must implement this for sphere, triangle, and mesh. 

    Shape(void);
    Shape(int id, int matIndex); // Constructor

private:
	// Write any other stuff here
};

// Class for sphere
class Sphere: public Shape
{
public:
	Sphere(void);	// Constructor
	Sphere(int id, int matIndex, int cIndex, float R, vector<Vector3f> *vertices);	// Constructor
	ReturnVal intersect(const Ray & ray) const;	// Will take a ray and return a structure related to the intersection information. You will implement this. 

private:
	int cIndex;
	float R;
	vector<Vector3f> *vertices;
};

// Class for triangle
class Triangle: public Shape
{
public:
	Triangle(void);	// Constructor
	Triangle(int id, int matIndex, int p1Index, int p2Index, int p3Index, vector<Vector3f> *vertices);	// Constructor
	ReturnVal intersect(const Ray & ray) const; // Will take a ray and return a structure related to the intersection information. You will implement this. 

private:
	int p1Index;
	int p2Index;
	int p3Index;
	vector<Vector3f> *vertices;
	float det(float a, float b, float c, float d, float e, float f, float g, float h, float i) const{
		float det = a * ( e*i - h*f ) + b * ( g*f - d*i ) + c * ( d*h - e*g );
		return det;
	}
};

// Class for mesh
class Mesh: public Shape
{
public:
	Mesh(void);	// Constructor
	Mesh(int id, int matIndex, const vector<Triangle>& faces, vector<int> *pIndices, vector<Vector3f> *vertices);	// Constructor
	ReturnVal intersect(const Ray & ray) const; // Will take a ray and return a structure related to the intersection information. You will implement this. 

private:
	vector<Triangle> faces;
	vector<int> *pIndices;
	vector<Vector3f> *vertices;
};

#endif
