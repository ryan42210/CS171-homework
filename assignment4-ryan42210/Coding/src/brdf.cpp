#include <vector>
#include <random>
#include "Eigen/Dense"
#include "Eigen/Geometry"
#include "interaction.hpp"
#include "utils.hpp"
#include "constant.hpp"
#include "brdf.hpp"


/**
 * BRDF class
 */

BRDF::BRDF()
{
}


/**
 * IdealDiffusion class
 */

IdealDiffusion::IdealDiffusion(Eigen::Vector3f diff)
    : reflectivity(diff)
{
}
    
Eigen::Vector3f IdealDiffusion::eval(const Interaction& interact)
{
    return samplePdf(interact) * reflectivity;
}

float IdealDiffusion::samplePdf(const Interaction& interact)
{
    return 1/(2 * PI);
}

std::default_random_engine e;

float IdealDiffusion::sample(Interaction& interact)
{
    /** TODO */
    std::uniform_real_distribution<float> u(0.0, 1.0);
    /* uniform sample on a unit disk */
    float xi1 = u(e);
    float xi2 = u(e);
    float r = sqrt(xi1);
    float phi = xi2 * 2 * PI;
    /* project sample to hemisphere, up direction is +z axis */
    Eigen::Vector3f local_wi = Eigen::Vector3f(r*cos(phi), r*sin(phi), sqrt(1-xi1));
    Eigen::Matrix3f rotate = Eigen::Matrix3f::Zero();
    rotate = Eigen::Quaternionf::FromTwoVectors(Eigen::Vector3f(0, 0, 1.0f), interact.normal).toRotationMatrix();
    /* set direction for next path */
    interact.wi = rotate * local_wi;

    float pdf = sqrt(1-xi1) / PI;
    return pdf;
}

bool IdealDiffusion::isDelta() const
{
    return false;
}


/**
 * IdealSpecular class
 */

IdealSpecular::IdealSpecular(Eigen::Vector3f spec)
    : reflectivity(spec)
{
}

Eigen::Vector3f IdealSpecular::eval(const Interaction& interact)
{
    /** TODO */
    return samplePdf(interact) * reflectivity;
}

float IdealSpecular::samplePdf(const Interaction& interact)
{
    /** TODO */
    Eigen::Vector3f ref =  2 * interact.normal.dot(interact.wo) * interact.normal - interact.wo;
    if ((ref-interact.wi).norm() < 1e-3) return 1.0f;
    return 0.0f;
}

float IdealSpecular::sample(Interaction& interact)
{
    /** TODO */
    interact.wi = 2 * interact.normal.dot(interact.wo) * interact.normal - interact.wo;
    return 1.0f;
}

bool IdealSpecular::isDelta() const
{
    return true;
}


/**
 * IdealTransmission class
 */

IdealTransmission::IdealTransmission(Eigen::Vector3f reflect, float idx_refract)
    : reflectivity(reflect)
    , ior(idx_refract)
{

}

Eigen::Vector3f IdealTransmission::eval(const Interaction& interact)
{
    /** TODO */
    return samplePdf(interact) * reflectivity;
}

float IdealTransmission::samplePdf(const Interaction& interact)
{
    /** TODO */
    // float cosi = interact.wi.dot(interact.normal);
    // Eigen::Vector3f dir;
    // if (cosi >= 0)
    // {
    //     dir = - ior * interact.wi + (ior*cosi - sqrt(1-ior*ior*(1-cosi*cosi)))*interact.normal;
    // } else {
    //     cosi = -cosi;
    //     dir = - 1/ior * interact.wi + ((1/ior)*cosi + sqrt((1-1/ior/ior)*(1-cosi*cosi)))*interact.normal;
    // }
    // if (dir.dot(interact.wo) > 0.999);
    //     return 1.0f;
    return 1.0f;
}

float IdealTransmission::sample(Interaction& interact)
{
    /** TODO */
    // float cosi = interact.wo.dot(interact.normal);
    // Eigen::Vector3f dir;
    // if (cosi > 0)
    // {
    //     dir = - 1/ior * interact.wo + (1/ior*cosi - sqrt(1 - 1/(ior*ior)*(1-cosi*cosi) ) )*interact.normal;
    // } else {
    //     Eigen::Vector3f n = - interact.normal;
    //     cosi = interact.wo.dot(interact.normal);
    //     dir = - ior * interact.wo + (ior*cosi + sqrt((1-ior*ior)*(1-cosi*cosi)))*n;
    // }
    return 1.0f;
}

bool IdealTransmission::isDelta() const
{
    return true;
}
