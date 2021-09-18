#pragma once
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

void Scene::setAmbientLight(Eigen::Vector3f ambient)
{
    ambient_light = ambient;
}

Eigen::Vector3f Scene::getAmbientLight() const
{
    return ambient_light;
}

bool Scene::intersection(const Ray& ray, Interaction& interaction, float cam_f) const
{
    Interaction surfaceInteraction;
    light->rayIntersection(surfaceInteraction, ray);
    for (Geometry* geom : geometries)
    {
        Interaction curInteraction;
        curInteraction.in_direction = ray.direction;
        curInteraction.cam_factor = cam_f;
        if (geom->rayIntersection(curInteraction, ray))
        {
            if (surfaceInteraction.entry_dist == -1 || curInteraction.entry_dist < surfaceInteraction.entry_dist)
            {
                surfaceInteraction = curInteraction;
            }
        }
    }

    interaction = surfaceInteraction;
    if (surfaceInteraction.entry_dist != -1 && surfaceInteraction.entry_dist >= ray.range_min && surfaceInteraction.entry_dist <= ray.range_max)
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
