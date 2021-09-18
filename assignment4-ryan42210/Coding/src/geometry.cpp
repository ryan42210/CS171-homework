#include <math.h>
#include <algorithm>
#include "geometry.hpp"
#include "constant.hpp"
#include "utils.hpp"
#include "objloader.hpp"


/**
 * Geometry class
 */

void Geometry::setMaterial(BRDF* new_mat)
{
    material = new_mat;
}


/**
 * Parallelogram class
 */

Parallelogram::Parallelogram(Eigen::Vector3f p0, Eigen::Vector3f s0, Eigen::Vector3f s1, Eigen::Vector3f normal, BRDF* mat)
    : p0(p0)
    , normal(normal.normalized())
{
    s0_len = s0.norm();
    s1_len = s1.norm();
    this->s0 = s0.normalized();
    this->s1 = s1.normalized();

    setMaterial(mat);
    buildBoundingBox();
}

bool Parallelogram::rayIntersection(Interaction& interaction, const Ray& ray)
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
        if (material != nullptr)
        {
            interaction.material = (void*)material;
        }
        interaction.type = Interaction::Type::GEOMETRY;
        return true;
    }
    return false;
}

void Parallelogram::buildBoundingBox()
{
    Eigen::Vector3f p1 = p0 + s0 + s1;
    bounding_box.lb = p0.cwiseMin(p1);
    bounding_box.ub = p0.cwiseMax(p1);
}

std::pair<Eigen::Vector3f, Eigen::Vector3f> Parallelogram::getEdgeDir() const
{
    return std::pair<Eigen::Vector3f, Eigen::Vector3f>(s0, s1);
}

Eigen::Vector3f Parallelogram::getNormal() const
{
    return normal;
}

/**
 * Sphere class
 */

Sphere::Sphere(Eigen::Vector3f p0, float r, BRDF* mat)
    : p0(p0)
    , radius(r)
{
    setMaterial(mat);
    buildBoundingBox();
}

bool Sphere::rayIntersection(Interaction& interaction, const Ray& ray)
{
    float a = 1.0f;
    float b = 2 * ray.direction.dot(ray.origin - p0);
    float c = (ray.origin - p0).squaredNorm() - radius * radius;
    float delta = b * b - 4 * a * c;

    if (delta < 0)
    {
        return false;
    }

    float t0 = (-b - sqrt(delta)) / 2 * a;
    float t1 = (-b + sqrt(delta)) / 2 * a;

    if (t1 < 0)
    {
        return false;
    }
    else if (t0 < 0 && t1 >= 0)
    {
        t0 = t1;
    }

    if (t0 < ray.range_min || t0 > ray.range_max)
    {
        return false;
    }

    interaction.entry_dist = t0;
    interaction.exit_dist = t1;
    interaction.entry_point = ray.getPoint(t0);
    Eigen::Vector3f r_vec = interaction.entry_point - p0;
    interaction.normal = r_vec.normalized();
    if (material != nullptr)
    {
        interaction.material = (void*)material;
    }
    interaction.type = Interaction::Type::GEOMETRY;

    return true;
}

void Sphere::buildBoundingBox()
{
    bounding_box = AABB(p0, radius);
}


/**
 * TriangleMesh class
 */

TriangleMesh::TriangleMesh(std::string file_path, BRDF* mat)
{
    setMaterial(mat);

    std::vector<Eigen::Vector2f> out_uvs;
    std::vector<int> out_vt_index;
    loadObj(file_path.c_str(), vertices, out_uvs, normals, vertices_index, out_vt_index, normals_index);
    
    num_triangles = vertices_index.size() / 3;

    has_grid = false;

    buildBoundingBox();
}

bool TriangleMesh::raySingleTriangleIntersection(Interaction& interaction, const Ray& ray, int v0_idx, int v1_idx, int v2_idx) const
{
    /**
     * TODO: Ray intersection test with single triangle
     * Note: Remember that normals are interpolated using barycentric coordinates.
     */
    Eigen::Vector3f v0 = vertices[vertices_index[v0_idx]];
    Eigen::Vector3f v1 = vertices[vertices_index[v1_idx]];
    Eigen::Vector3f v2 = vertices[vertices_index[v2_idx]];

    Eigen::Vector3f tmp_vec;
    tmp_vec << 
        (ray.origin-v0).cross(v1-v0).dot(v2-v0),
        ray.direction.cross(v2-v0).dot(ray.origin-v0),
        (ray.origin-v0).cross(v1-v0).dot(ray.direction);
    Eigen::Vector3f res = (1.0f/(ray.direction.cross(v2-v0).dot(v1-v0))) * tmp_vec;
    float a = res[1],  b = res[2], c = 1 - a - b;
    float t = res[0];
    if (t < ray.range_min || t > ray.range_max) return false;
    if (a < 1 && a > 0 && b < 1 && b > 0 && c < 1 && c > 0)
    {
        Eigen::Vector3f intersection_point = a * v0 + b * v1 + c * v2;
        interaction.entry_point = intersection_point;
        interaction.entry_dist = t;
        interaction.exit_dist = t;

        interaction.normal = a * normals[normals_index[v0_idx]] + b * normals[normals_index[v1_idx]] + c * normals[normals_index[v2_idx]];
        return true;
    }
    return false;
}

bool TriangleMesh::rayIntersection(Interaction& interaction, const Ray& ray)
{
    float t_in, t_out;
    if (!bounding_box.rayIntersection(ray, t_in, t_out)) return false;

    Interaction final_interaction;
    if (has_grid) {
        /**
         * TODO: Use uniform grid to handle triangle intersection here
         * Note: Grid traversal algorithm must be used here.
         */
        Eigen::Vector3i target_cell;
        Eigen::Vector3i step;
        Eigen::Vector3f t_0;
        Eigen::Vector3f t_delta;
        int res_x = unif_gird.res_x;
        int res_y = unif_gird.res_y;
        int res_z = unif_gird.res_z;
        /* initialize */
        unif_gird.initializeT0(ray, t_0, target_cell, t_in);
        unif_gird.initializeTDelta(ray, t_delta);
        step[0] = (ray.direction[0] > 0) ? 1 : -1;
        step[1] = (ray.direction[1] > 0) ? 1 : -1;
        step[2] = (ray.direction[2] > 0) ? 1 : -1;
        /* increamental tranversal */
        bool intersected = false;
        int i = 0;
        do
        {
            i++;
            if (!unif_gird.getCell(target_cell).is_empty)
            {
                for (Eigen::Vector3i v_id : unif_gird.getCell(target_cell).v_index)
                {
                    Interaction cur_inter;
                    intersected = raySingleTriangleIntersection(cur_inter, ray, v_id[0], v_id[1], v_id[2]);
                    if (intersected && cur_inter.entry_dist > t_0.minCoeff()) {
                        intersected = false;
                    }
                    if (intersected && (final_interaction.entry_dist == -1 || cur_inter.entry_dist < final_interaction.entry_dist)) {
                        final_interaction = cur_inter;
                    }
                }
            }
            if (t_0[0] < t_0[1])
            {
                if (t_0[0] < t_0[2])
                {
                    target_cell[0] += step[0];
                    if (target_cell[0] == unif_gird.res_x || target_cell[0] == -1) break;
                    t_0[0] += t_delta[0];
                } else {
                    target_cell[2] += step[2];
                    if (target_cell[2] == unif_gird.res_z || target_cell[2] == -1) break;
                    t_0[2] += t_delta[2];
                }
            } else {
                if (t_0[1] < t_0[2])
                {
                    target_cell[1] += step[1];
                    if (target_cell[1] == unif_gird.res_y || target_cell[1] == -1) break;
                    t_0[1] += t_delta[1];
                } else {
                    target_cell[2] += step[2];
                    if (target_cell[2] == unif_gird.res_z || target_cell[2] == -1) break;
                    t_0[2] += t_delta[2];
                }
            }
        } while (!intersected);


    } else {
        for (int i = 0; i < num_triangles; i++) {
            Interaction cur_it;
            if (raySingleTriangleIntersection(cur_it, ray, 3 * i, 3 * i + 1, 3 * i + 2)) {
                if (final_interaction.entry_dist == -1 || cur_it.entry_dist < final_interaction.entry_dist) {
                    final_interaction = cur_it;
                }
            }
        }
    }

    if (final_interaction.entry_dist != -1)
    {
        interaction = final_interaction;
        if (material != nullptr)
        {
            interaction.material = material;
        }
        interaction.type = Interaction::Type::GEOMETRY;
        // std::cout << "intersect with geo\n";
        return true;
    }

    return false;
}

void TriangleMesh::buildBoundingBox()
{

    bounding_box.lb = vertices[0].cwiseMin(vertices[1]);
    bounding_box.ub = vertices[0].cwiseMax(vertices[1]);
    for (int i = 2; i < vertices.size(); i++)
    {
        bounding_box.lb = bounding_box.lb.cwiseMin(vertices[i]);
        bounding_box.ub = bounding_box.ub.cwiseMax(vertices[i]);
    }
}

void TriangleMesh::applyTransformation(const Eigen::Affine3f& t)
{
    for (int i = 0; i < vertices.size(); i++)
    {
        vertices[i] = t * vertices[i];
    }

    Eigen::Matrix3f t_inv_tr = t.linear().inverse().transpose();
    for (int i = 0; i < normals.size(); i++)
    {
        normals[i] = (t_inv_tr * normals[i]).normalized();
    }

    buildBoundingBox();
}

void TriangleMesh::buildUniformGrid() {
    /**
     * TODO: Build uniform grid here
     * Note: you may need to build your own data structures in the accel_struct.hpp and accel_struct.cpp
     */
    unif_gird.initialize(bounding_box, 20);
    std::cout<< "building uniform gird..." <<std::endl;
    for (int i = 0; i < num_triangles; i++)
    {
        AABB tri_bd_box(vertices[vertices_index[3*i]], vertices[vertices_index[3*i+1]], vertices[vertices_index[3*i+2]]);
        int cell_xmin = (tri_bd_box.lb[0]-bounding_box.lb[0]) / unif_gird.cell_size[0];
        int cell_xmax = (tri_bd_box.ub[0] - bounding_box.lb[0]) / unif_gird.cell_size[0];
        
        int cell_ymin = (tri_bd_box.lb[1]-bounding_box.lb[1]) / unif_gird.cell_size[1];
        int cell_ymax = (tri_bd_box.ub[1] - bounding_box.lb[1]) / unif_gird.cell_size[1];

        int cell_zmin = (tri_bd_box.lb[2]-bounding_box.lb[2]) / unif_gird.cell_size[2];
        int cell_zmax = (tri_bd_box.ub[2] - bounding_box.lb[2]) / unif_gird.cell_size[2];
        if (cell_xmax == unif_gird.res_x) cell_xmax -= 1;
        if (cell_ymax == unif_gird.res_y) cell_ymax -= 1;
        if (cell_zmax == unif_gird.res_z) cell_zmax -= 1;
        for (int cell_x = cell_xmin; cell_x <= cell_xmax; cell_x++) {
            for (int cell_y = cell_ymin; cell_y <= cell_ymax; cell_y++) {
                for (int cell_z = cell_zmin; cell_z <= cell_zmax; cell_z++) {
                    // std::cout<<"add1\\n";
                    unif_gird.addTriangleToCell(Eigen::Vector3i(cell_x, cell_y, cell_z), 3*i, 3*i+1, 3*i+2);
                }
            }
        }
    }
    
    std::cout<< "finish building uniform grid!" <<std::endl;
    has_grid = true;
}