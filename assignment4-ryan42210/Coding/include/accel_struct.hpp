#pragma once
#include <utility>
#include <cmath>
#include <vector>
#include "ray.hpp"
#include "interaction.hpp"


/**
 * Axis-Aligned Bounding Box
 */

struct AABB {
    Eigen::Vector3f lb;
    Eigen::Vector3f ub;

    AABB();
    /* Construct AABB by coordinates of lower bound and upper bound */
    AABB(float lb_x, float lb_y, float lb_z, float ub_x, float ub_y, float ub_z);
    AABB(Eigen::Vector3f lb, Eigen::Vector3f ub);
    /* Construct AABB for a sphere */
    AABB(const Eigen::Vector3f& pos, float radius);
    /* Construct AABB for a triangle */
    AABB(const Eigen::Vector3f& v1, const Eigen::Vector3f& v2, const Eigen::Vector3f& v3);
    /* Construct AABB by merging two AABBs */
    AABB(const AABB& a, const AABB& b);
    /* Get the AABB center */
    Eigen::Vector3f getCenter() const;
    /* Get the length of a specified side on the AABB */
    float getDist(int c) const;
    /* Get the volume of the AABB */
    float getVolume() const;
    /* Check whether the AABB is overlapping with another AABB */
    bool isOverlap(const AABB& a) const;
    /* Get the diagonal length */
    float diagonalLength() const;
    /* Test intersection with a ray */
    bool rayIntersection(const Ray& ray, float& t_in, float& t_out) const;
};

struct Cell {
    std::vector<Eigen::Vector3i> v_index;
    AABB bd_box;
    bool is_empty;

    Cell();
    Cell(Eigen::Vector3f lb, Eigen::Vector3f ub);
    void addTrianle(int v_idx0, int v_idx1, int v_idx_2);
};

struct UniformGrid {
    std::vector<Cell> cells;
    Eigen::Vector3f cell_size;
    int res_x, res_y, res_z;
    Eigen::Vector3f lb;
    Eigen::Vector3f ub;

    UniformGrid();
    UniformGrid(Eigen::Vector3f lb, Eigen::Vector3f ub, int gird_res);
    void initialize(AABB bd_box, int gird_res);
    void constructGrid();
    /* add a triangle with given index to one cell with given index */
    void addTriangleToCell(Eigen::Vector3i cell_id, int v_idx0, int v_idx1, int v_idx2);
    /* initialize t0 and t_delta */
    void initializeT0(const Ray& ray, Eigen::Vector3f &t0, Eigen::Vector3i &start_cell, float t_in);
    void initializeTDelta(const Ray& ray, Eigen::Vector3f &t_delta);
    Cell getCell(int id_x, int id_y, int id_z);
    Cell getCell(Eigen::Vector3i& target_id);
    void operator=(UniformGrid unif);
};