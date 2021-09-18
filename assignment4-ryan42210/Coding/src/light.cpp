#include <utility>
#include <random>
#include "light.hpp"
#include "geometry.hpp"
#include "utils.hpp"

std::default_random_engine e2;

/**
 * Light class
 */

Light::Light(Eigen::Vector3f pos, Eigen::Vector3f power)
    : position(pos)
    , radiance(power)
{
}

Eigen::Vector3f Light::getPosition() const
{
    return position;
}

Eigen::Vector3f Light::getRadiance() const
{
    return radiance;
}


/**
 * AreaLight class
 */
AreaLight::AreaLight(Eigen::Vector3f pos, Eigen::Vector3f power, Eigen::Vector2f size)
    : Light(pos, power)
    , area_size(size)
    , geometry_delegation(
        pos - Eigen::Vector3f(size[0], 0, size[1]) / 2,
        Eigen::Vector3f(size[0], 0, 0),
        Eigen::Vector3f(0, 0, size[1]),
        Eigen::Vector3f(0, -1, 0), nullptr)
{
}

Eigen::Vector3f AreaLight::emission(Eigen::Vector3f pos, Eigen::Vector3f dir)
{
    Eigen::Vector3f res = std::max(dir.dot(geometry_delegation.getNormal()), 0.0f) * getRadiance();
    return res;
}

float AreaLight::samplePdf(const Interaction& ref_it, Eigen::Vector3f pos)
{
    float cos_term = ref_it.wi.dot(geometry_delegation.getNormal());
    float distance = (pos - ref_it.entry_point).norm();
    return abs(cos_term) / distance / distance;
}

Eigen::Vector3f AreaLight::sample(Interaction& ref_it, float* pdf)
{
    std::uniform_real_distribution<float> u(-0.5, 0.5);
    float xi1 = u(e2);
    float xi2 = u(e2);
    Eigen::Vector3f edgeDir1 = geometry_delegation.getEdgeDir().first;
    Eigen::Vector3f edgeDir2 = geometry_delegation.getEdgeDir().second;
    Eigen::Vector3f sample_pos
        = position + (xi1 * area_size[0] * edgeDir1) + (xi2 * area_size[1] * edgeDir2);
    ref_it.wi = (sample_pos - ref_it.entry_point).normalized();
    if (area_size[0] * area_size[1] != 0)
        *pdf = 1 / (area_size[0] * area_size[1]);
    return sample_pos;
}

bool AreaLight::rayIntersection(Interaction& interaction, const Ray& ray)
{
    bool intersection = geometry_delegation.rayIntersection(interaction, ray);
    interaction.type = Interaction::Type::LIGHT;
    return intersection;
}
