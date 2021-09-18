#ifndef NO_OMP
#include <omp.h>
#endif
#include "progressbar.hpp"
#include "integrator.hpp"
#include "constant.hpp"
#include "light.hpp"
#include "utils.hpp"
#include "config.hpp"
#include "brdf.hpp"



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
 * PathTracingIntegrator class
 */

PathTracingIntegrator::PathTracingIntegrator(Scene* scene, Camera* camera)
    : Integrator(scene, camera)
{  
}

void PathTracingIntegrator::render()
{
    int dx, dy;
    int res_x = camera->getFilm().resolution.x(), res_y = camera->getFilm().resolution.y();

    /* Initialize a progress bar */
    progressbar progress_bar(res_x * res_y);

#ifndef NO_OMP
    #pragma omp parallel for private(dy)
#endif
    for (dx = 0; dx < res_x; ++dx)
    {
        for (dy = 0; dy < res_y; ++dy)
        {
            Eigen::Vector3f px_rad = Eigen::Vector3f::Zero();
            std::vector<Eigen::Vector2f> samples = mathutils::girdsample(conf.spp);
            for (auto pt : samples)
            {
                Ray ray = camera->generateRay(dx + pt[0], dy + pt[1]);
                // px_rad += dir_light(ray);
                px_rad += radiance(ray, conf.max_depth);
            }
            px_rad /= conf.spp;
            if (isnan(px_rad[0]) || isnan(px_rad[1]) || isnan(px_rad[2])) {
                std::cout <<"res nan\n";
            }
            if (isinf(px_rad[0]) || isinf(px_rad[1]) || isinf(px_rad[2])) {
                std::cout <<"res inf\n";
            }
            camera->setPixel(dx, dy, px_rad);

#ifndef NO_OMP
            #pragma omp critical
#endif
            progress_bar.update();
        }
    }
}

Eigen::Vector3f PathTracingIntegrator::radiance(Ray ray, int depth)
{
    Interaction interaction;
    if (!scene->intersection(ray, interaction)) return Eigen::Vector3f::Zero();
    interaction.wo = -ray.direction;

    if (interaction.type == Interaction::Type::NONE || depth < 1)
    {
        return Eigen::Vector3f::Zero();
    }
    else if (interaction.type == Interaction::Type::LIGHT)
    {
        return scene->getLight()->emission(interaction.entry_point, interaction.wo);
    }
    
    // sample on light source
    Eigen::Vector3f dir_light = Eigen::Vector3f::Zero();
    Eigen::Vector3f dir_brdf = Eigen::Vector3f::Zero();
    float p_wd_l = 1.0f;
    float p_wd_b = 1.0f;
    if (interaction.material != nullptr)
    {
        // std::cout<<"calculate shading!\n";
        Eigen::Vector3f sample_pos = scene->getLight()->sample(interaction, &p_wd_l);
        Ray new_ray(interaction.entry_point +0.01*interaction.wi, 20*interaction.wi);
        BRDF *brdf_p = (BRDF *)interaction.material;
        if (!scene->isShadowed(new_ray))
        {
            float dist = (sample_pos - interaction.entry_point).norm();
            float ex_term = scene->getLight()->samplePdf(interaction, sample_pos);
            Eigen::Vector3f L_i = scene->getLight()->emission(sample_pos, -interaction.wi);

            Eigen::Vector3f f_r = brdf_p->eval(interaction);
            dir_light = interaction.normal.dot(interaction.wi) * ex_term * (L_i.array() * f_r.array()).matrix() / p_wd_l;
        }
        p_wd_b = brdf_p->sample(interaction);
        Ray next_path_brdf(interaction.entry_point +0.001*interaction.wi, 20*interaction.wi);
        if (!scene->isShadowed(next_path_brdf))
        {
            Eigen::Vector3f f_r = brdf_p->eval(interaction);
            Eigen::Vector3f L_i = scene->getLight()->emission(sample_pos, -interaction.wi);
            dir_brdf = interaction.normal.dot(interaction.wi) * (L_i.array() * f_r.array()).matrix() / p_wd_b;
        }
        dir_light = (p_wd_l*p_wd_l/(p_wd_b*p_wd_b+p_wd_l*p_wd_l))*dir_light + (p_wd_b*p_wd_b/(p_wd_b*p_wd_b+p_wd_l*p_wd_l))*dir_brdf;
        if (isnan(dir_light[0]) || isnan(dir_light[1]) || isnan(dir_light[2])){
            dir_light = Eigen::Vector3f::Zero();
        }
        if (isinf(dir_light[0]) || isinf(dir_light[1]) || isinf(dir_light[2])){
            dir_light = Eigen::Vector3f::Zero();
        }
    }

    /* sample by on surface */
    
    Eigen::Vector3f indir_light = Eigen::Vector3f::Zero();
    
    float p_wi = 0;
    if (interaction.material != nullptr)
    {
        BRDF *brdf_p = (BRDF *)interaction.material;
        // p_w should be calculated before f_r
        p_wi = brdf_p->sample(interaction);
        Eigen::Vector3f f_r = brdf_p->eval(interaction);
        
        Ray next_path(interaction.entry_point +0.01*interaction.wi, 20*interaction.wi);
        if (scene->isShadowed(next_path) && interaction.normal.dot(interaction.wi) > 0)
        // if (true)
        {
            Eigen::Vector3f L_i = radiance(next_path, depth-1);
            indir_light = interaction.normal.dot(interaction.wi) * (L_i.array() * f_r.array()).matrix() / p_wi;
        }
    }
    if (isnan(indir_light[0]) || isnan(indir_light[1]) || isnan(indir_light[2])) {
        indir_light = Eigen::Vector3f::Zero();
    }
    if (isinf(indir_light[0]) || isinf(indir_light[1]) || isinf(indir_light[2])) {
        indir_light = Eigen::Vector3f::Zero();
    }

    return dir_light + indir_light;
}
