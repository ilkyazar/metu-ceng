#include "Vector.h"

Vector::Vector(void)
{
}

Vector::~Vector(void)
{
}

Vector::Vector(float x, float y, float z){
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector::Vector(const Vector &v1){
    this->x = v1.x;
    this->y = v1.y;
    this->z = v1.z; 
}

Vector Vector::operator+ (Vector const &v1){
    Vector v2;
    v2.x = v1.x + this->x;
    v2.y = v1.y + this->y;
    v2.z = v1.z + this->z;
    return v2;
}

Vector Vector::operator- (Vector const &v1){
    Vector v3;
    v3.x = this->x - v1.x;
    v3.y = this->y - v1.y;
    v3.z = this->z - v1.z;
    return v3;
}

Vector Vector::operator* (float scalar){
    Vector v2;
    v2.x = scalar * this->x;
    v2.y = scalar * this->y;
    v2.z = scalar * this->z;
    return v2;
}

Vector Vector::cross(Vector const &v1){
    Vector v3;
    v3.x = (this->y * v1.z) - (this->z * v1.y);
    v3.y = (this->x * v1.z) - (this->z * v1.x);
    v3.z = (this->x * v1.y) - (this->y * v1.x);
    //std::cout << "v1.x" << v1.x << " " << this->x << std::endl;
    //std::cout << "v1.y" << v1.y << " " << this->y << std::endl;
    //std::cout << "v1.z" << v1.z << " " << this->z << std::endl;

    return v3;
}

float Vector::dot(Vector const &v1){
    float result = v1.x * this->x + v1.y * this->y + v1.z * this->z;
    return result;
}
