#include "scene.hpp"


/**
 * Scene class
 */

Scene::Scene()
    : light(nullptr)
{
}

Scene::Scene(Light* light)
    : light(light)
{
}

void Scene::addGeometry(Geometry* geom)
{
    geometries.push_back(geom);
}

int Scene::countGeometries() const
{
    return geometries.size();
}

void Scene::setLight(Light* new_light)
{
    light = new_light;
}

Light* Scene::getLight() const
{
    return light;
}

bool Scene::intersection(const Ray& ray, Interaction& interaction) const
{
    Interaction final_it;
    light->rayIntersection(final_it, ray);
    for (Geometry* geom : geometries)
    {
        Interaction cur_it;
        if (geom->rayIntersection(cur_it, ray))
        {
            if (final_it.entry_dist == -1 || cur_it.entry_dist < final_it.entry_dist)
            {
                final_it = cur_it;
            }
        }
    }

    interaction = final_it;
    if (final_it.entry_dist != -1 && final_it.entry_dist >= ray.range_min && final_it.entry_dist <= ray.range_max)
    {
        return true;
    }
    return false;
}

bool Scene::isShadowed(const Ray& ray) const
{
    Interaction in;
    return intersection(ray, in) && in.type == Interaction::Type::GEOMETRY;
}
