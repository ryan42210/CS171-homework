#pragma once
#include <Eigen/Dense>
#include <utility>
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
	Eigen::Vector3f color;

public:
	Light() = default;
	Light(Eigen::Vector3f pos, Eigen::Vector3f rgb);
	Eigen::Vector3f getPosition() const;
	Eigen::Vector3f getColor() const;
	virtual Eigen::Vector3f getEnv(const Interaction& inter) const;
	virtual std::vector<LightSamplePair> samples() const = 0;
	virtual bool rayIntersection(Interaction& interaction, const Ray& ray) const = 0;
};


/**
 * Point lights
 */

class PointLight : public Light
{
public:
	PointLight(Eigen::Vector3f pos, Eigen::Vector3f rgb);
	virtual std::vector<LightSamplePair> samples() const override;
	virtual bool rayIntersection(Interaction& interaction, const Ray& ray) const override;
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
	AreaLight(Eigen::Vector3f pos, Eigen::Vector3f rgb, Eigen::Vector2f size);
	virtual std::vector<LightSamplePair> samples() const override;
	virtual bool rayIntersection(Interaction& interaction, const Ray& ray) const override;
};


class EnvironmentLight : public Light
{
protected:
	Box env_cube;
	std::vector<LightSamplePair> ls;
public:
	EnvironmentLight(float d2c, Material* f, Material* bk, Material* l, Material* r, Material* bt, Material* t);
	virtual Eigen::Vector3f getEnv(const Interaction& inter) const;
	virtual std::vector<LightSamplePair> samples() const override;
	virtual bool rayIntersection(Interaction& interaction, const Ray& ray) const override;
};