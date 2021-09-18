#include <vector>
#include "material.hpp"
#include "constant.hpp"
#include "utils.hpp"


/**
 * ColorMaterial class
 */

ColorMaterial::ColorMaterial()
    : diffusion_color(Eigen::Vector3f(1.0, 1.0, 1.0))
    , specular_color(Eigen::Vector3f(0, 0, 0))
    , shininess(0.0)
{
}

ColorMaterial::ColorMaterial(Eigen::Vector3f diff_rgb, Eigen::Vector3f spec_rgb, float sh)
    : diffusion_color(diff_rgb)
    , specular_color(spec_rgb)
    , shininess(sh)
{
}

ColorMaterial::ColorMaterial(Eigen::Vector3f rgb, float sh = 16.0f)
    : diffusion_color(rgb)
    , specular_color(rgb)
    , shininess(sh)
{
}

InteractionPhongModel ColorMaterial::evaluate(const Interaction& in) const
{
    InteractionPhongModel m;
    m.diffusion = diffusion_color;
    m.specular = specular_color;
    m.shininess = shininess;
    return m;
}


/**
 * TextureMaterial class
 */

TextureMaterial::TextureMaterial()
    : texture(nullptr)
    , shininess(16.0f)
{
}

TextureMaterial::TextureMaterial(Texture* tex, float sh)
    : texture(tex)
    , shininess(sh)
{
}

InteractionPhongModel TextureMaterial::evaluate(const Interaction& in) const
{
    InteractionPhongModel m;
    if (texture->getLevelNums() > 1)
    {
        // dist_uv is the px's width in uv space
        float dist_uv = - 2 * in.entry_dist * in.cam_factor / in.normal.dot(in.in_direction);
        float dist_tex = (float)in.tex_width / texture->getDimension()[0];
        float l_continous = log2f(dist_uv / dist_tex);
        if (l_continous < 0) l_continous = 0;
        unsigned int level = floor(l_continous);
        float alpha = l_continous - level;
        // if (level >= texture->getLevelNums()) std::cout << "M" << std::endl;
        level = std::min(level, texture->getLevelNums() - 1);
        unsigned int level2 = std::min(level + 1, texture->getLevelNums() - 1);
        // linear interpolation between two layers
        Eigen::Vector3f color1 = texture->getPixel(in.uv[0], in.uv[1], level);
        Eigen::Vector3f color2 = texture->getPixel(in.uv[0], in.uv[1], level2);
        color1 = (1.0f-alpha) * color1 + alpha * color2;
        m.diffusion = color1;
        m.specular = color1;
    }
    else 
    {
        m.diffusion = texture->getPixel(in.uv[0], in.uv[1]);
        m.specular = texture->getPixel(in.uv[0], in.uv[1]);
    }
    m.shininess = shininess;
    return m;
}

Eigen::Vector2i TextureMaterial::getTexDim() const
{
    return texture->getDimension();
}