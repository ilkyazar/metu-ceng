#include "Ray.h"
Ray::Ray()
{
}

Ray::Ray(const Vector3f& origin, const Vector3f& direction)
    : origin(origin), direction(direction)
{
}

/* Takes a parameter t and returns the point accoring to t. t is the parametric variable in the ray equation o+t*d.*/
Vector3f Ray::getPoint(float t) const 
{
    Vector3f point;
    point.x = this->origin.x + t*(this->direction.x);
    point.y = this->origin.y + t*(this->direction.y);
    point.z = this->origin.z + t*(this->direction.z);
    return point;

}

/* Takes a point p and returns the parameter t according to p such that p = o+t*d. */
float Ray::gett(const Vector3f & p) const
{
    float t = ( p.x - this->origin.x )/this->direction.x;
    return t;
}
/*
int main(){
    Vector3f origin, direction;
    const char *str;
    str = "3 4 5";
    sscanf(str, "%f %f %f", &origin.x, &origin.y, &origin.z);
    str = "10 4 5";
    sscanf(str, "%f %f %f", &direction.x, &direction.y, &direction.z);

    Ray* myRay= new Ray(origin, direction);
    myRay->getPoint(3);
}
*/
