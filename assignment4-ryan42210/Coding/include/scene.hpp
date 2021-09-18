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
	/* Checks whether a ray intersectes with the scene */
	bool intersection(const Ray& ray, Interaction& interaction) const;
	/* Checks whether a ray is shadowed in the scene */
	bool isShadowed(const Ray& ray) const;
};
