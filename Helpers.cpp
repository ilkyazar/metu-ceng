#include <iostream>
#include <cmath>
#include <algorithm>
#include "Helpers.h"
#include "Matrix4.h"
#include "Vec3.h"
#include "Vec4.h"
#include "Rotation.h"
#include "Translation.h"
#include "Scaling.h"
using namespace std;

/*
 * Calculate cross product of vec3 a, vec3 b and return resulting vec3.
 */
Vec3 crossProductVec3(Vec3 a, Vec3 b)
{
    Vec3 result;

    result.x = a.y * b.z - b.y * a.z;
    result.y = b.x * a.z - a.x * b.z;
    result.z = a.x * b.y - b.x * a.y;

    return result;
}

/*
 * Calculate dot product of vec3 a, vec3 b and return resulting value.
 */
double dotProductVec3(Vec3 a, Vec3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

/*
 * Find length (|v|) of vec3 v.
 */
double magnitudeOfVec3(Vec3 v)
{
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

/*
 * Normalize the vec3 to make it unit vec3.
 */
Vec3 normalizeVec3(Vec3 v)
{
    Vec3 result;
    double d;

    d = magnitudeOfVec3(v);
    result.x = v.x / d;
    result.y = v.y / d;
    result.z = v.z / d;

    return result;
}

/*
 * Return -v (inverse of vec3 v)
 */
Vec3 inverseVec3(Vec3 v)
{
    Vec3 result;
    result.x = -v.x;
    result.y = -v.y;
    result.z = -v.z;

    return result;
}

/*
 * Add vec3 a to vec3 b and return resulting vec3 (a+b).
 */
Vec3 addVec3(Vec3 a, Vec3 b)
{
    Vec3 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;

    return result;
}

/*
 * Subtract vec3 b from vec3 a and return resulting vec3 (a-b).
 */
Vec3 subtractVec3(Vec3 a, Vec3 b)
{
    Vec3 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;

    return result;
}

/*
 * Multiply each element of vec3 with scalar.
 */
Vec3 multiplyVec3WithScalar(Vec3 v, double c)
{
    Vec3 result;
    result.x = v.x * c;
    result.y = v.y * c;
    result.z = v.z * c;

    return result;
}

/*
 * Prints elements in a vec3. Can be used for debugging purposes.
 */
void printVec3(Vec3 v)
{
    cout << "(" << v.x << "," << v.y << "," << v.z << ")" << endl;
}

/*
 * Check whether vec3 a and vec3 b are equal.
 * In case of equality, returns 1.
 * Otherwise, returns 0.
 */
int areEqualVec3(Vec3 a, Vec3 b)
{

    /* if x difference, y difference and z difference is smaller than threshold, then they are equal */
    if ((ABS((a.x - b.x)) < EPSILON) && (ABS((a.y - b.y)) < EPSILON) && (ABS((a.z - b.z)) < EPSILON))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*
 * Returns an identity matrix (values on the diagonal are 1, others are 0).
*/
Matrix4 getIdentityMatrix()
{
    Matrix4 result;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (i == j)
            {
                result.val[i][j] = 1.0;
            }
            else
            {
                result.val[i][j] = 0.0;
            }
        }
    }

    return result;
}

/*
 * Multiply matrices m1 (Matrix4) and m2 (Matrix4) and return the result matrix r (Matrix4).
 */
Matrix4 multiplyMatrixWithMatrix(Matrix4 m1, Matrix4 m2)
{
    Matrix4 result;
    double total;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            total = 0;
            for (int k = 0; k < 4; k++)
            {
                total += m1.val[i][k] * m2.val[k][j];
            }

            result.val[i][j] = total;
        }
    }

    return result;
}

/*
 * Multiply matrix m (Matrix4) with vector v (vec4) and store the result in vector r (vec4).
 */
Vec4 multiplyMatrixWithVec4(Matrix4 m, Vec4 v)
{
    double values[4];
    double total;

    for (int i = 0; i < 4; i++)
    {
        total = 0;
        for (int j = 0; j < 4; j++)
        {
            total += m.val[i][j] * v.getElementAt(j);
        }
        values[i] = total;
    }

    return Vec4(values[0], values[1], values[2], values[3], v.colorId);
}

//transformation functions
Matrix4 getTranslationMatrix(Matrix4 matrix, Translation* t) {
    matrix.val[0][3] = t->tx;
    matrix.val[1][3] = t->ty;
    matrix.val[2][3] = t->tz;
    return matrix;
}

Matrix4 getScalingMatrix(Matrix4 matrix, Scaling s){
    matrix.val[0][0] = matrix.val[0][0] * s.sx;
    matrix.val[1][1] = matrix.val[1][1] * s.sy;
    matrix.val[2][2] = matrix.val[2][2] * s.sz;
    return matrix;
}

Vec3 getV(double x, double y, double z){
    double minimum = min({x,y,z});
    if(minimum == x){
        return Vec3(0,-z,y,-1);
    }
    else if(minimum == y){
        return Vec3(-z,0,x,-1);
    }
    else{
        return Vec3(-y,x,0,-1);
    }
}

Matrix4 getRotationMatrix(Matrix4 matrix, Rotation r){
    Vec3 u(r.ux, r.uy, r.uz, -1);
    Vec3 unit_u(normalizeVec3(u));
    
    Vec3 v(getV(unit_u.x, unit_u.y, unit_u.z));
    Vec3 unit_v(normalizeVec3(v));
    
    Vec3 w(crossProductVec3(u,v));
    Vec3 unit_w(normalizeVec3(w));

    double m_val[4][4] = {{unit_u.x, unit_u.y, unit_u.z, 0},
                        {unit_v.x, unit_v.y, unit_v.z, 0},
                        {unit_w.x, unit_w.y, unit_w.z, 0},
                        {0,0,0,1}};

    double inverse_m_val[4][4] = {{unit_u.x, unit_v.x, unit_w.x, 0},
                                {unit_u.y, unit_v.y, unit_w.y, 0},
                                {unit_u.z, unit_v.z, unit_w.z, 0},
                                {0,0,0,1}};

    double rotation_x[4][4] = {{1,0,0,0},{0,cos(r.angle), -sin(r.angle),0},
                            {0, sin(r.angle), cos(r.angle), 0},{0,0,0,1}};
    
    Matrix4 M(m_val);
    Matrix4 inverse_M(inverse_m_val);
    Matrix4 R(rotation_x);
    
    return multiplyMatrixWithMatrix(multiplyMatrixWithMatrix(inverse_M, R),M);

}

Vec4 Vec3toVec4(Vec3* vec3) {
    Vec4 vec4(vec3->x, vec3->y, vec3->z, 0.0, -1);
    return vec4;
}

Vec3 Vec4toVec3(Vec4 vec4) {
    Vec3 vec3(vec4.x, vec4.y, vec4.z, -1);
    return vec3;
}