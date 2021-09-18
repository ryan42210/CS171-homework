#include <omp.h>
#include "optics_data.hpp"
#include "volume_renderer.hpp"
#include "config.hpp"


extern Config conf;

/**
 * VolumeRenderer class
 */

VolumeRenderer::VolumeRenderer()
    : camera(NULL)
    , classifier(NULL)
    , geom_list(NULL)
{
}

void VolumeRenderer::setCamera(Camera* cam)
{
    camera = cam;
}

void VolumeRenderer::addLight(Light* li)
{
    lights.push_back(li);
}

void VolumeRenderer::addGeometry(ImplicitGeometry* implicit_geom)
{
    geom_list.push_back(implicit_geom);
}

void VolumeRenderer::setClassifier(Classifier* cls)
{
    classifier = cls;
}

void VolumeRenderer::renderFrontToBack()
{
    int res_x = camera->getFilm().resolution.x();
    int res_y = camera->getFilm().resolution.y();
    float dt = conf.step_len;
    int progress = 0;

    AABB bd_box(Eigen::Vector3f::Zero(), 0);
    for (auto geom : geom_list)
    {
        bd_box = AABB(bd_box, geom->getBBox());
    }

#ifndef NO_OMP
    #pragma omp parallel for
#endif
    for (int i = 0; i < res_x * res_y; ++i)
    {
        int dy = i / (float)res_x;
        int dx = i - dy * (float)res_x;
        std::vector<Eigen::Vector2f> shifts = MathUtils::girdsample(conf.spp);
        Eigen::Vector3f final_color = Eigen::Vector3f::Zero();
        for (auto pt : shifts)
        {
            Ray ray = camera->generateRay((float)dx+pt[0], (float)dy+pt[1]);
            Eigen::Vector3f color(0, 0, 0);
            Eigen::Vector3f alpha(0, 0, 0);
            
            float t_in, t_out;
            /* skip emtpy space */
            if (!bd_box.rayIntersection(ray, t_in, t_out)) continue;
            for (float t = t_in; t <= t_out; t += conf.step_len)
            {
                for (auto geom :geom_list)
                {
                    Eigen::Vector3f p = ray.origin + t* ray.direction;
                    VolumePointData vdata;
                    vdata.value = geom->getValue(p);
                    vdata.gradient = geom->computeGradient(p);
                    vdata.position = p;
                    OpticsData opd = classifier->transfer(vdata, camera, lights, conf.step_len);
                    color += ((Eigen::Vector3f(1.0f, 1.0f, 1.0f) - alpha).array() * opd.getColor().array()).matrix();
                    alpha += ((Eigen::Vector3f(1.0f, 1.0f, 1.0f) - alpha).array() * opd.getOpacity().array()).matrix();
                    if (alpha.minCoeff() > 0.999f) {
                        break;
                    }
                }
            }
            final_color += color;
        }
        camera->setPixel(dx, dy, final_color/conf.spp);
    #ifndef NO_OMP
        #pragma omp critical
    #endif
        printf("%.2f\r", (float)(progress++) / (res_x * res_y) * 100);
    }
};
