#pragma once
#include "Eigen/Dense"


/**
 * The Phong lighting model at the interacting point
 */

struct InteractionPhongModel
{
    Eigen::Vector3f diffusion;
    Eigen::Vector3f specular;
    float shininess;
};


/**
 * Data structure representing interaction between objects and rays
 */

struct Interaction
{
	enum Type
	{
		NONE,      /* No interaction */
		GEOMETRY,  /* Interaction happens on a geometry */
		LIGHT,      /* Interaction happens on a light source */
		ENVIRONMENT
	};

	/* Distance (in units of t) to intersection point */
	float entry_dist;
	/* Distance (in units of t) to the second intersection point(if existed) */
	float exit_dist;
	/* The px's height in world coordinate of the camera in the scene */
	float cam_factor;
	/* the width of a px on texture in world coordinate */
	float tex_width;
	/* Position of intersection point */
	Eigen::Vector3f entry_point;
	/* Normal of intersection point (if existed) */
	Eigen::Vector3f normal;
	/* Light direction shooting on this point */
	Eigen::Vector3f in_direction;
	/* UV coordinate of intersection point (if existed) */
	Eigen::Vector2f uv;
	/* Phong lighting model at the intersected point (if existed) */
	InteractionPhongModel lighting_model;
	/* Type of interacting object */
	Type type;

	Interaction() : entry_dist(-1), exit_dist(-1), type(Type::NONE) {}
};
