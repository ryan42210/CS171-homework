#pragma once

#include <utility>
#include <vector>
#include "light.hpp"
#include "geometry.hpp"

class Scene
{
protected:
	std::vector<Geometry *> geometries;
	Light* light;
	Eigen::Vector3f ambient_light;

public:
	Scene();
	Scene(Light* light);
	/* Adds a geometry to the scene */
	void addGeometry(Geometry* geom);
	/* Returns the number of geometries in the scene */
	int countGeometries() const;
	/* Sets a light source */
	void setLight(Light* new_light);
	/* Retrieves the light source */
	Light* getLight() const;
	/* Sets an ambient light for approximating indirect lighting */
	void setAmbientLight(Eigen::Vector3f ambient);
	/* Retrieves the ambient light */
	Eigen::Vector3f getAmbientLight() const;
	/* Checks whether a ray intersectes with the scene */
	bool intersection(const Ray& ray, Interaction& interaction, float cam_f = 0.1f) const;
	/* Checks whether a ray is shadowed in the scene */
	bool isShadowed(const Ray& ray) const;
};
