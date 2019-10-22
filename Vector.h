#ifndef _VECTOR_H_
#define _VECTOR_H_
#include <iostream> 

class Vector {
    public:
        float x,y,z;
        Vector();
        Vector(float x, float y, float z);
        Vector(const Vector &v1);
        ~Vector();

        Vector operator+ (Vector const &v1);
        Vector operator- (Vector const &v1);
        Vector operator* (float scalar);
        Vector cross (Vector const &v1) ;
        float dot(Vector const &v1);

    
};

#endif