#include <math.h>
#include "geometry.hpp"
#include "constant.hpp"
#include "utils.hpp"

#define WIDTH 32.0f
/**
 * Geometry class
 */

void Geometry::setMaterial(Material* new_mat)
{
    material = new_mat;
}


/**
 * Parallelogram class
 */

Parallelogram::Parallelogram(Eigen::Vector3f p0, Eigen::Vector3f s0, Eigen::Vector3f s1, Eigen::Vector3f normal, Material* mat)
    : p0(p0)
    , normal(normal.normalized())
{
    s0_len = s0.norm();
    s1_len = s1.norm();
    this->s0 = s0.normalized();
    this->s1 = s1.normalized();

    setMaterial(mat);
}

bool Parallelogram::rayIntersection(Interaction& interaction, const Ray& ray) const
{
    if (ray.direction.dot(normal) == 0)
    {
        return false;
    }
    
    float t = (p0 - ray.origin).dot(normal) / ray.direction.dot(normal);
    Eigen::Vector3f p0_p = ray.getPoint(t) - p0;
    float q0 = p0_p.dot(s0) / s0_len;
    float q1 = p0_p.dot(s1) / s1_len;
    if (q0 >= 0 && q0 <= s0.norm() && q1 >= 0 && q1 <= s1.norm() && t >= ray.range_min && t <= ray.range_max)
    {
        interaction.entry_dist = t;
        interaction.exit_dist = t;
        interaction.normal = normal;
        interaction.entry_point = ray.getPoint(t);
        interaction.uv[0] = q0;
        interaction.uv[1] = q1;
        if (material != nullptr)
        {
            interaction.lighting_model = material->evaluate(interaction);
        }
        interaction.type = Interaction::Type::GEOMETRY;
        return true;
    }
    return false;
}

Eigen::Vector3f Parallelogram::getEdgeDir1() const
{
    return s0_len * s0;
}

Eigen::Vector3f Parallelogram::getEdgeDir2() const
{
    return s1_len * s1;
}


/**
 * Sphere class
 */

Sphere::Sphere(Eigen::Vector3f p0, float r, Material* mat)
    : p0(p0)
    , radius(r)
{
    setMaterial(mat);
}

bool Sphere::rayIntersection(Interaction& interaction, const Ray& ray) const
{
    float t_o2mid = (p0 - ray.origin).dot(ray.direction);
    if (t_o2mid < 0) return false;
    float d_square = (p0 - ray.origin).norm() * (p0 - ray.origin).norm() - t_o2mid * t_o2mid;
    
    if (d_square < 0 || d_square > radius*radius) return false;
    
    float t_inter2mid = sqrtf(radius*radius - d_square);
    float t0 = t_o2mid - t_inter2mid;
    float t1 = t_o2mid + t_inter2mid;

    if (t0 > ray.range_min && t1 < ray.range_max)
    {
        Eigen::Vector3f p = ray.origin + t0 * ray.direction;
        interaction.entry_dist = t0;
        interaction.exit_dist = t1;
        interaction.entry_point = p;
        interaction.normal = (p - p0).normalized();
        float x = (p - p0).x();
        float y = (p - p0).y();
        float z = (p - p0).z();
        float theta = compress(acosf(y / radius), 0.0f, PI);
        float phi = compress(abs(atan2f(z, x)), 0.0f, PI);
        interaction.uv[0] = phi;
        interaction.uv[1] = theta;
        if (material != nullptr)
        {
            interaction.lighting_model = material->evaluate(interaction);
        }
        interaction.type = Interaction::Type::GEOMETRY;
        return true;
    }
    return false;
}

Ground::Ground(float y, Material* mat)
    : y0(y)
{
    setMaterial(mat);
}

bool Ground::rayIntersection(Interaction& interaction, const Ray& ray) const
{
    float dist = (y0 - ray.origin[1]) / ray.direction[1];
    if (dist > ray.range_min && dist < ray.range_max)
    {
        interaction.entry_dist = dist;
        interaction.exit_dist = dist;
        interaction.entry_point = ray.origin + dist * ray.direction;
        interaction.normal = Eigen::Vector3f(0, 1, 0);
        interaction.uv[0] = sawtooth(interaction.entry_point[0], WIDTH);
        interaction.uv[1] = sawtooth(interaction.entry_point[2], WIDTH);
        interaction.tex_width = WIDTH;
        if (material != nullptr)
        {
            interaction.lighting_model = material->evaluate(interaction);
        }
        interaction.type = Interaction::Type::GEOMETRY;
        return true;
    }
    return false;
}

Box::Box(float d2c, Material* front, Material*back, Material* left, Material* right, Material* bot, Material* top)
    : dist_to_center(d2c)
{
    setMaterial(front, back, left, right, bot, top);
}

void Box::setMaterial(Material* f, Material* bk, Material* l, Material* r, Material* bt, Material* t)
{
    front = f;
    back = bk;
    left = l;
    right = r;
    bot = bt;
    top = t;
}

bool Box::rayIntersection(Interaction& interaction, const Ray& ray) const
{
    int face_type = 0;
    // t to front
    float t = (dist_to_center - ray.origin[2]) / ray.direction[2];
    face_type = FRONT;
    // t to back
    if (t < 0)
    {
        t = (-dist_to_center - ray.origin[2]) / ray.direction[2];
        face_type = BACK;
    }
    // t to left
    float t1 = (dist_to_center - ray.origin[0]) / ray.direction[0];
    int tmp = LEFT;
    // t to right
    if (t1 < 0) {
        t1 = (-dist_to_center - ray.origin[0]) / ray.direction[0];
        tmp = RIGHT;
    }
    if (t1 < t)
    {
        t = t1;
        face_type = tmp;
    }
    // t to top
    float t2 = (dist_to_center - ray.origin[1]) / ray.direction[1];
    tmp = TOP;
    // t to botton
    if (t2 < 0) {
        t2 = (-dist_to_center - ray.origin[1]) / ray.direction[1];
        tmp = BOT;
    }
    if (t2 < t) t = t2, face_type = tmp;
    interaction.entry_dist = t;
    interaction.entry_point = ray.origin + t * ray.direction;
    interaction.exit_dist = t;
    
    if (face_type == FRONT)
    {
        interaction.uv[0] = compress(-interaction.entry_point[0], -dist_to_center, dist_to_center);
        interaction.uv[1] = compress(-interaction.entry_point[1], -dist_to_center, dist_to_center);
    }
    else if (face_type == BACK)
    {
        interaction.uv[0] = compress(interaction.entry_point[0], -dist_to_center, dist_to_center);
        interaction.uv[1] = compress(-interaction.entry_point[1], -dist_to_center, dist_to_center);
    }
    else if (face_type == LEFT)
    {
        interaction.uv[0] = compress(interaction.entry_point[2], -dist_to_center, dist_to_center);
        interaction.uv[1] = compress(-interaction.entry_point[1], -dist_to_center, dist_to_center);
    }
    else if (face_type == RIGHT)
    {
        interaction.uv[0] = compress(-interaction.entry_point[2], -dist_to_center, dist_to_center);
        interaction.uv[1] = compress(-interaction.entry_point[1], -dist_to_center, dist_to_center);
    }
    else if (face_type == TOP)
    {
        interaction.uv[0] = compress(-interaction.entry_point[0], -dist_to_center, dist_to_center);
        interaction.uv[1] = compress(interaction.entry_point[2], -dist_to_center, dist_to_center);
    }
    else if (face_type == BOT)
    {
        interaction.uv[0] = compress(-interaction.entry_point[2], -dist_to_center, dist_to_center);
        interaction.uv[1] = compress(interaction.entry_point[0], -dist_to_center, dist_to_center);
    }
    
    if (face_type == FRONT && front != nullptr) interaction.lighting_model = front->evaluate(interaction);
    if (face_type == BACK && back != nullptr) interaction.lighting_model = back->evaluate(interaction);
    if (face_type == LEFT && left != nullptr) interaction.lighting_model = left->evaluate(interaction);
    if (face_type == RIGHT && right != nullptr) interaction.lighting_model = right->evaluate(interaction);
    if (face_type == BOT && bot != nullptr) interaction.lighting_model = bot->evaluate(interaction);
    if (face_type == TOP && top != nullptr) interaction.lighting_model = top->evaluate(interaction);
    
    interaction.type = Interaction::Type::ENVIRONMENT;
    
    return true;
}