#include <cmath>
#include "implicit_geom.hpp"
#include "constant.hpp"
#include "utils.hpp"


/**
 * ImplicitGeometry class
 */

AABB ImplicitGeometry::getBBox() const
{
    return bounding_box;
}

bool ImplicitGeometry::bboxRayIntersection(Ray r, float &t_entry, float &t_exit) const
{
    return bounding_box.rayIntersection(r, t_entry, t_exit);
}


/**
 * GenusTwoSurface class
 */

GenusTwoSurface::GenusTwoSurface(Eigen::Vector3f pos, Eigen::Vector3f range)
{
    bounding_box = AABB(pos - range / 2.0, pos + range / 2.0);
}

float GenusTwoSurface::getValue(Eigen::Vector3f p)
{
    float x = p.x();
    float y = p.y();
    float z = p.z();

    return 2 * y * (y * y - 3 * x * x) * (1 - z * z) + pow(x * x + y * y, 2) - (9 * z * z - 1) * (1 - z * z);
}

Eigen::Vector3f GenusTwoSurface::computeGradient(Eigen::Vector3f p)
{
    float x = p.x();
    float y = p.y();
    float z = p.z();

    Eigen::Vector3f grad;
    grad[0] = 4*x*(x*x+y*y) - 12*x*y*(1-z*z);
    grad[1] = 4*y*(x*x+y*y) + 4*y*y*(1-z*z) + 2*(-3*x*x+y*y)*(1-z*z);
    grad[2] = -4*y*(-3*x*x+y*y)*z - 18*z*(1-z*z) + 2*z*(-1+9*z*z);

    return grad;
}


/**
 * WineGlassSurface class
 */

WineGlassSurface::WineGlassSurface(Eigen::Vector3f pos, Eigen::Vector3f range)
{
    bounding_box = AABB(pos - range / 2.0, pos + range / 2.0);
}

float WineGlassSurface::getValue(Eigen::Vector3f p)
{
    float x = p.x();
    float y = p.y();
    float z = p.z();

    /* Out of domain: Not-A-Number */
    if (z + 3.2 <= 0)
    {
        return INFINITY;
    }

    return x * x + y * y - pow(log(z + 3.2), 2) - 0.09;
}

Eigen::Vector3f WineGlassSurface::computeGradient(Eigen::Vector3f p)
{
    float x = p.x();
    float y = p.y();
    float z = p.z();

    Eigen::Vector3f grad;
    grad[0] = 2*x;
    grad[1] = 2*y;
    grad[2] = -2*log(z+3.2) / (z+3.2);
    if (z == 0) grad[2] = INFINITY;

    return grad;
}


/**
 * PorousSurface class
 */

PorousSurface::PorousSurface(Eigen::Vector3f pos, Eigen::Vector3f range)
{
    bounding_box = AABB(pos - range / 2.0, pos + range / 2.0);
}

float PorousSurface::getValue(Eigen::Vector3f p)
{
    float x = p.x();
    float y = p.y();
    float z = p.z();

    return -0.02 + pow(-0.88 + pow(y, 2), 2) * pow(2.92 * (-1 + x) * pow(x, 2) * (1 + x) + 1.7 * pow(y, 2), 2) + pow(-0.88 + pow(z, 2), 2) * pow(2.92 * (-1 + y) * pow(y, 2) * (1 + y) + 1.7 * pow(z, 2), 2) + pow(-0.88 + pow(x, 2), 2) * pow(1.7 * pow(x, 2) + 2.92 * (-1 + z) * pow(z, 2) * (1 + z), 2);
}

Eigen::Vector3f PorousSurface::computeGradient(Eigen::Vector3f p)
{
    float x = p.x();
    float y = p.y();
    float z = p.z();
    Eigen::Vector3f grad;
    grad[0] = 
    2*(2.92*(-1 + x)*x*x + 5.84 * (-1 + x)*x*(1 + x) + 2.92*x*x*(1 + x))*pow((-0.88 + y*y), 2) * (2.92*(-1 + x)*x*x*(1 + x) + 1.7*y*y) + 6.8*x* pow((-0.88 + x*x), 2)* (1.7*x*x + 2.92*(-1 + z)*z*z*(1 + z)) + 4*x*(-0.88 + x*x)*pow((1.7*x*x + 2.92*(-1 + z)*z*z*(1 + z)), 2);
    grad[1] = 
    6.8*y*pow((-0.88 + y*y),2) * (2.92*(-1 + x)*x*x*(1 + x) + 1.7*y*y) + 4*y*(-0.88 + y*y)*pow((2.92*(-1 + x)*x*x*(1 + x) + 1.7*y*y), 2) + 2*(2.92*(-1 + y)*y*y + 5.84*(-1 + y)*y*(1 + y) + 2.92*y*y*(1 + y))*pow((-0.88 + z*z),2)*(2.92*(-1 + y)*y*y*(1 + y) + 1.7*z*z); 
    grad[2] = 
    6.8*z*pow((-0.88 + z*z),2)*(2.9*(-1 + y)*y*y*(1 + y) + 1.7*z*z) + 4*z*(-0.88 + z*z)*pow((2.92*(-1 + y)*y*y*(1 + y) + 1.7*z*z),2) + 2*pow((-0.88 + x*x),2)*(2.92*(-1 + z)*z*z + 5.84*(-1 + z)*z*(1 + z) + 2.92*z*z*(1 + z))*(1.7*x*x + 2.92*(-1 + z)*z*z*(1 + z));

    return grad;
}
