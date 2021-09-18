#include "light.hpp"
#include "geometry.hpp"
#include "utils.hpp"
#include "constant.hpp"


/**
 * Light class
 */

Light::Light(Eigen::Vector3f pos, Eigen::Vector3f rgb)
    : position(pos)
    , color(rgb)
{
}

Eigen::Vector3f Light::getPosition() const
{
    return position;
}

Eigen::Vector3f Light::getColor() const
{
    return color;
}

Eigen::Vector3f Light::getEnv(const Interaction& inter) const
{
    return Eigen::Vector3f::Zero();
}


/**
 * PointLight class
 */

PointLight::PointLight(Eigen::Vector3f pos, Eigen::Vector3f rgb)
    : Light(pos, rgb)
{
}

std::vector<LightSamplePair> PointLight::samples() const
{
    std::vector<LightSamplePair> ls;
    ls.push_back(LightSamplePair(position, color));
    return ls;
}

bool PointLight::rayIntersection(Interaction& interaction, const Ray& ray) const
{
    Eigen::Vector3f dist = position - ray.origin;
    float t = dist.norm();
    if (ray.direction == dist.normalized() && t >= ray.range_min && t <= ray.range_max)
    {
        interaction.entry_dist = t;
        interaction.exit_dist = t;
        interaction.entry_point = ray.getPoint(t);
        interaction.type = Interaction::Type::LIGHT;
        return true;
    }
    return false;
}


/**
 * AreaLight class
 */
AreaLight::AreaLight(Eigen::Vector3f pos, Eigen::Vector3f rgb, Eigen::Vector2f size)
    : Light(pos, rgb)
    , area_size(size)
    , geometry_delegation(
        pos - Eigen::Vector3f(size[0], 0, size[1]) / 2,
        Eigen::Vector3f(size[0], 0, 0),
        Eigen::Vector3f(0, 0, size[1]),
        Eigen::Vector3f(0, 1, 0), nullptr)
{
}

std::vector<LightSamplePair> AreaLight::samples() const
{
    std::vector<LightSamplePair> ls;
    int light_res = LIGHT_SAMPLE;
    for (int i = 0; i < light_res; i++) {
        for (int j = 0; j < light_res; j++) {
            Eigen::Vector3f pos 
                = position
                + ((float)(i + 0.5f) / (float)light_res - 0.5f) * geometry_delegation.getEdgeDir1() 
                + ((float)(j + 0.5f) / (float)light_res - 0.5f) * geometry_delegation.getEdgeDir2();
            ls.push_back(LightSamplePair(pos, color));
        }
    }
    return ls;
}

bool AreaLight::rayIntersection(Interaction& interaction, const Ray& ray) const
{
    bool intersection = geometry_delegation.rayIntersection(interaction, ray);
    interaction.type = Interaction::Type::LIGHT;
    return intersection;
}


/* set environment light with cube box*/
EnvironmentLight::EnvironmentLight(float d2c, Material* f, Material* bk, Material* l, Material* r, Material* bt, Material* t)
    : Light()
    , env_cube(d2c, f, bk, l, r, bt, t)
{
}

Eigen::Vector3f EnvironmentLight::getEnv(const Interaction& inter) const
{
    return inter.lighting_model.diffusion;
}

std::vector<LightSamplePair> EnvironmentLight::samples() const
{
    return this->ls;
}

bool EnvironmentLight::rayIntersection(Interaction& interaction, const Ray& ray) const
{
    bool intersect = env_cube.rayIntersection(interaction, ray);
    interaction.type = Interaction::Type::ENVIRONMENT;
    return intersect;
}