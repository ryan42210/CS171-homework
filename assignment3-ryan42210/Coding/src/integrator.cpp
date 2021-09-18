#ifndef NO_OMP
#include <omp.h>
#endif
#include "integrator.hpp"
#include "material.hpp"
#include "constant.hpp"
#include "light.hpp"
#include "utils.hpp"
#include "config.hpp"


extern Config conf;


/**
 * Integrator class
 */

Integrator::Integrator(Scene* scn, Camera* cam)
    : scene(scn)
    , camera(cam)
{
}


/**
 * PhongLightingIntegrator class
 */

PhongLightingIntegrator::PhongLightingIntegrator(Scene* scn, Camera* cam)
    : Integrator(scn, cam)
{
}

void PhongLightingIntegrator::render(int id)
{
    int dx, dy;

#ifndef NO_OMP
    #pragma omp parallel for private(dy)
#endif
    for (dx = 0; dx < camera->getFilm().resolution.x(); ++dx)
    {
        for (dy = 0; dy < camera->getFilm().resolution.y(); ++dy)
        {
            Eigen::Vector3f rad = Eigen::Vector3f::Zero();
            auto pt_v = camera->sampleInPx(dx, dy, conf.num_samples);
            for (Eigen::Vector2f pt : pt_v)
            {
                Ray ray = camera->generateRay(pt[0], pt[1]);
                Interaction inter;
                if (scene->intersection(ray, inter, camera->getPxHeight()))
                {
                    if (id == 1 || id == 3) rad += radiance(inter, ray);
                    else if (id == 2) rad += spec_rad(inter, ray);
                }
            }
            camera->setPixel(dx, dy, rad/pt_v.size());
        }
    }
}

Eigen::Vector3f PhongLightingIntegrator::radiance(const Interaction& interaction, const Ray& ray) const
{
    /* If the ray hits a light source, return the light color */
    if (interaction.type == Interaction::Type::LIGHT)
    {
        return scene->getLight()->getColor();
    }

    if (interaction.type == Interaction::Type::ENVIRONMENT)
    {
        return scene->getLight()->getEnv(interaction);
    }

    /* Ambient component */
    Eigen::Vector3f amb_comp = (scene->getAmbientLight().array() * interaction.lighting_model.diffusion.array()).matrix();
    Eigen::Array3f diff = Eigen::Array3f::Zero();
    Eigen::Array3f spec = Eigen::Array3f::Zero();
    if (interaction.type == Interaction::Type::GEOMETRY)
    {
        auto light_samples = scene->getLight()->samples();
        for (LightSamplePair ls : light_samples)
        {
            // light direction from object to light source
            float diff_factor = 0;
            float spec_factor = 0;
            Eigen::Vector3f light_dir = (ls.first - interaction.entry_point).normalized();
            Ray sur_to_light(interaction.entry_point, ls.first - interaction.entry_point);
            if (!scene->isShadowed(sur_to_light))
            {
                diff_factor = std::max(interaction.normal.dot(light_dir), 0.0f);
                Eigen::Vector3f reflect = (2 * interaction.normal.dot(light_dir) * interaction.normal - light_dir).normalized();
                spec_factor = powf(std::max(reflect.dot(-ray.direction), 0.0f), interaction.lighting_model.shininess);

                diff += diff_factor * ls.second.array() * interaction.lighting_model.diffusion.array() / light_samples.size();
                spec += spec_factor * ls.second.array() * interaction.lighting_model.diffusion.array() / light_samples.size();
            }
        }
    }

    return diff.matrix() + spec.matrix() + amb_comp;
}

Eigen::Vector3f PhongLightingIntegrator::spec_rad(const Interaction& interaction, const Ray& ray) const
{
    if (interaction.type == Interaction::Type::LIGHT)
    {
        return scene->getLight()->getColor();
    }

    if (interaction.type == Interaction::Type::ENVIRONMENT)
    {
        return scene->getLight()->getEnv(interaction);
    }

    if (interaction.type == Interaction::Type::GEOMETRY)
    {
        Eigen::Vector3f reflect_dir = 2 * interaction.normal.dot(-ray.direction) * interaction.normal + ray.direction;
        Ray reflect_ray(interaction.entry_point, reflect_dir);
        Interaction inter;
        scene->intersection(reflect_ray, inter);
        return (interaction.lighting_model.specular.array() * scene->getLight()->getEnv(inter).array()).matrix();
    }

    return Eigen::Vector3f::Zero();
}
