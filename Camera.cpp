#include "Camera.h"
#include <cstring>

Camera::Camera(int id,                      // Id of the camera
               const char* imageName,       // Name of the output PPM file 
               const Vector3f& pos,         // Camera position
               const Vector3f& gaze,        // Camera gaze direction
               const Vector3f& up,          // Camera up direction
               const ImagePlane& imgPlane)  // Image plane parameters
{
     this->id = id;
     std::strcpy(this->imageName, imageName);      
     this->imgPlane = imgPlane;
     this->pos = pos;
     this->gaze = gaze;
     this->up = up;

}

/* Takes coordinate of an image pixel as row and col, and
 * returns the ray going through that pixel. 
 */
Ray Camera::getPrimaryRay(int col, int row) const
{
     Vector pos_vec(this->pos.x, this->pos.y, this->pos.z);
     Vector gaze_vec(this->gaze.x, this->gaze.y, this->gaze.z);
     Vector up_vec(this->up.x, this->up.y, this->up.z);
     Vector zero_vec(0, 0, 0);
     Vector q, m, u, v, d;

     u = up_vec.cross(gaze_vec);
     m = pos_vec + gaze_vec * this->imgPlane.distance;
     q = m +  (u * this->imgPlane.left) + (up_vec * this->imgPlane.top); 

     float s_u = (row + 0.5) * (this->imgPlane.right - this->imgPlane.left) / this->imgPlane.nx;
     float s_v = (col + 0.5) * (this->imgPlane.top - this->imgPlane.bottom) / this->imgPlane.ny;


     d = q + (u * s_u) - (up_vec * s_v) - pos_vec;

     Ray malifalitiko;
     malifalitiko.origin = pos;
     malifalitiko.direction.x = d.x;
     malifalitiko.direction.y = d.y;
     malifalitiko.direction.z = d.z;

     //std::cout << malifalitiko.direction.x << " " << malifalitiko.direction.y << " " << malifalitiko.direction.z << std::endl;
     return malifalitiko;
}

