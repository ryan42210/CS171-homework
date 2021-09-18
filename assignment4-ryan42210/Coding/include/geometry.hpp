#pragma once
#include <utility>
#include <vector>
#include <unordered_map>
#include <cmath>
#include "interaction.hpp"
#include "brdf.hpp"
#include "ray.hpp"
#include "accel_struct.hpp"


/**
 * Base class of geometries
 */

class Geometry
{
protected:
	/* Material attached on the geometry */
	BRDF* material;
	/* The axis-aligned bounding box of the geometry */
	AABB bounding_box;

public:
	Geometry() {};
	virtual ~Geometry() = default;
	virtual bool rayIntersection(Interaction& interaction, const Ray& ray) = 0;
	virtual void buildBoundingBox() = 0;
	void setMaterial(BRDF* mat);
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
	Parallelogram(Eigen::Vector3f p0, Eigen::Vector3f s0, Eigen::Vector3f s1, Eigen::Vector3f normal, BRDF* mat);
	virtual bool rayIntersection(Interaction& interaction, const Ray& ray) override;
	virtual void buildBoundingBox() override;
	std::pair<Eigen::Vector3f, Eigen::Vector3f> getEdgeDir() const;
	Eigen::Vector3f getNormal() const;
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
	Sphere(Eigen::Vector3f p0, float r, BRDF* mat);
	virtual bool rayIntersection(Interaction& interaction, const Ray& ray) override;
	virtual void buildBoundingBox() override;
};


/**
 * Triangle meshes
 */

class TriangleMesh : public Geometry
{
protected:
	/* Number of triangles */
	int num_triangles;
	/* List of verteices and vertex indices */
	std::vector<Eigen::Vector3f> vertices;
	std::vector<int> vertices_index;
	/* List of normals and normal indices */
	std::vector<Eigen::Vector3f> normals;
	std::vector<int> normals_index;

	UniformGrid unif_gird;
	/* Is uniform grid established */
	bool has_grid;

	/* Test whether the given ray is intersected with the triangle (v0_idx, v1_idx, v2_idx) */
	bool raySingleTriangleIntersection(Interaction& interaction, const Ray& ray, int v0_idx, int v1_idx, int v2_idx) const;
	
public:
	TriangleMesh(std::string file_path, BRDF* mat);
	virtual bool rayIntersection(Interaction& interaction, const Ray& ray) override;
	virtual void buildBoundingBox() override;
	void applyTransformation(const Eigen::Affine3f& t);
	void buildUniformGrid();
};
