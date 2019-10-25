#include "Light.h"

/* Constructor. Implemented for you. */
PointLight::PointLight(const Vector3f & position, const Vector3f & intensity)
    : position(position), intensity(intensity)
{
}

// Compute the contribution of light at point p using the
// inverse square law formula
Vector3f PointLight::computeLightContribution(const Vector3f& p)
{
    std::cout << this->intensity.x << " " << this->intensity.y << " " << this->intensity.z << std::endl;
    Vector3f vec;
    vec.x = 0;
    vec.y = 0;
    vec.z = 0;
    return vec;
}
