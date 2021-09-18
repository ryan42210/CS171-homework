#pragma once
#include <Eigen/Dense>
#include <vector>
#include "ray.hpp"
#include "interaction.hpp"
#include "geometry.hpp"


typedef std::pair<Eigen::Vector3f, Eigen::Vector3f> LightSamplePair;


/**
 * Base class of lights
 */

class Light
{
protected:
	Eigen::Vector3f position;
	Eigen::Vector3f radiance;

public:
	Light(Eigen::Vector3f pos, Eigen::Vector3f power);
	Eigen::Vector3f getPosition() const;
	Eigen::Vector3f getRadiance() const;
	/* Get the emission at the specified position along the given direction */
	virtual Eigen::Vector3f emission(Eigen::Vector3f pos, Eigen::Vector3f dir) = 0;
	/* Sample a position on the light and obtain the corresponding PDF */
	virtual Eigen::Vector3f sample(Interaction& ref_it, float* pdf = nullptr) = 0;
	/* Compute the PDF of the given light sample */
	virtual float samplePdf(const Interaction& ref_it, Eigen::Vector3f pos) = 0;
	virtual bool rayIntersection(Interaction& interaction, const Ray& ray) = 0;
};


/**
 * Area lights
 */

class AreaLight : public Light
{
protected:
	Parallelogram geometry_delegation;
	Eigen::Vector2f area_size;

public:
	AreaLight(Eigen::Vector3f pos, Eigen::Vector3f power, Eigen::Vector2f size);
	virtual Eigen::Vector3f emission(Eigen::Vector3f pos, Eigen::Vector3f dir) override;
	virtual Eigen::Vector3f sample(Interaction& ref_it, float* pdf = nullptr) override;
	virtual float samplePdf(const Interaction& ref_it, Eigen::Vector3f pos) override;
	virtual bool rayIntersection(Interaction& interaction, const Ray& ray) override;
};
