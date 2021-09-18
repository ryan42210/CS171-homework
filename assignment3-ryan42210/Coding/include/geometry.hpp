#pragma once
#include <utility>
#include "interaction.hpp"
#include "material.hpp"
#include "ray.hpp"

#define FRONT 1
#define BACK 2
#define LEFT 3
#define RIGHT 4
#define BOT 5
#define TOP 6

/**
 * Base class of geometries
 */

class Geometry
{
protected:
	Material* material;

public:
	Geometry() {};
	virtual ~Geometry() = default;
	virtual bool rayIntersection(Interaction& interaction, const Ray& ray) const = 0;
	void setMaterial(Material* mat);
};


/**
 * Parallelograms
 */

class Parallelogram : public Geometry
{
protected:
	/* The origin point */
	Eigen::Vector3f p0;
	/* Directions of the two sides */
	Eigen::Vector3f s0, s1;
	/* Lengths of the two sides */
	float s0_len, s1_len;
	/* Normal (orientation) */
	Eigen::Vector3f normal;

public:
	Parallelogram(Eigen::Vector3f p0, Eigen::Vector3f s0, Eigen::Vector3f s1, Eigen::Vector3f normal, Material* mat);
	virtual bool rayIntersection(Interaction& interaction, const Ray& ray) const override;
	Eigen::Vector3f getEdgeDir1() const;
	Eigen::Vector3f getEdgeDir2() const;
};


/**
 * Spheres
 */
class Sphere : public Geometry
{
protected:
	/* Radius of the sphere */
	float radius;
	/* Center of the sphere */
	Eigen::Vector3f p0;

public:
	Sphere(Eigen::Vector3f p0, float r, Material* mat);
	virtual bool rayIntersection(Interaction& interaction, const Ray& ray) const override;
};


class Ground : public Geometry
{
protected:
	float y0;
public:
	Ground(float y, Material* mat);
	virtual bool rayIntersection(Interaction& interaction, const Ray& ray) const override;
};

class Box : public Geometry
{
protected:
	/* half length of each edge of the box */
	float dist_to_center;
	Material* front;
	Material* back;
	Material* left;
	Material* right;
	Material* top;
	Material* bot;
public:
	Box(float d2c, Material* front, Material*back, Material* left, Material* right, Material* bot, Material* top);
	void setMaterial(Material* front, Material*back, Material* left, Material* right, Material* bot, Material* top);
	virtual bool rayIntersection(Interaction& interaction, const Ray& ray) const override;
};