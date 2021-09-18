#pragma once
#include <vector>
#include "Eigen/Dense"
#include "interaction.hpp"
#include "texture.hpp"


/**
 * Base class of materials
 */

class Material
{
public:
    Material() {};
	virtual ~Material() = default;
    virtual InteractionPhongModel evaluate(const Interaction& in) const = 0;
};


/**
 * Color-based materials
 */

class ColorMaterial : public Material
{
protected:
    Eigen::Vector3f diffusion_color;
    Eigen::Vector3f specular_color;
    float shininess;

public:
    ColorMaterial();
    ColorMaterial(Eigen::Vector3f diff_rgb, Eigen::Vector3f spec_rgb, float sh = 16.0f);
    ColorMaterial(Eigen::Vector3f rgb, float shininess);
    virtual InteractionPhongModel evaluate(const Interaction& in) const override;
};


/**
 * Texture-based materials
 */

class TextureMaterial : public Material
{
protected:
    Texture* texture;
    float shininess;

public:
    TextureMaterial();
    TextureMaterial(Texture* tex, float sh = 16.0f);
    virtual InteractionPhongModel evaluate(const Interaction& in) const override;
    Eigen::Vector2i getTexDim() const;
};