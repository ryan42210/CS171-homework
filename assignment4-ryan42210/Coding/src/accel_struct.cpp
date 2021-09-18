#pragma once
#include "accel_struct.hpp"
#include <algorithm>
#include <iostream>


/**
 * AABB class
 */

AABB::AABB()
    : lb(0, 0, 0)
    , ub(0, 0, 0)
{
}
    
AABB::AABB(float lb_x, float lb_y, float lb_z, float ub_x, float ub_y, float ub_z)
{
    lb = Eigen::Vector3f(lb_x, lb_y, lb_z);
    ub = Eigen::Vector3f(ub_x, ub_y, ub_z);
}

AABB::AABB(Eigen::Vector3f lb, Eigen::Vector3f ub)
    : lb(lb)
    , ub(ub)
{
}

AABB::AABB(const Eigen::Vector3f& pos, float radius)
{
    Eigen::Vector3f r(radius, radius, radius);
    lb = pos - r;
    ub = pos + r;
}

AABB::AABB(const Eigen::Vector3f& v1, const Eigen::Vector3f& v2, const Eigen::Vector3f& v3)
{
    lb = v1.cwiseMin(v2).cwiseMin(v3);
    ub = v1.cwiseMax(v2).cwiseMax(v3);
}

AABB::AABB(const AABB& a, const AABB& b)
{
    lb = Eigen::Vector3f(a.lb.cwiseMin(b.lb));
    ub = Eigen::Vector3f(a.ub.cwiseMax(b.ub));
}

Eigen::Vector3f AABB::getCenter() const
{
    return (lb + ub) / 2;
}

float AABB::getDist(int c) const
{
    return ub[c] - lb[c];
}

float AABB::getVolume() const
{
    return getDist(2) * getDist(1) * getDist(0);
}

bool AABB::isOverlap(const AABB& a) const
{
    return ((a.lb[0] >= this->lb[0] && a.lb[0] <= this->ub[0]) || (this->lb[0] >= a.lb[0] && this->lb[0] <= a.ub[0])) &&
        ((a.lb[1] >= this->lb[1] && a.lb[1] <= this->ub[1]) || (this->lb[1] >= a.lb[1] && this->lb[1] <= a.ub[1])) &&
        ((a.lb[2] >= this->lb[2] && a.lb[2] <= this->ub[2]) || (this->lb[2] >= a.lb[2] && this->lb[2] <= a.ub[2]));

}

float AABB::diagonalLength() const
{
    return (ub - lb).norm();
}

bool AABB::rayIntersection(const Ray& ray, float& t_in, float& t_out) const
{
    float dir_frac_x = (ray.direction[0] == 0.0) ? 1.0e32 : 1.0f / ray.direction[0];
    float dir_frac_y = (ray.direction[1] == 0.0) ? 1.0e32 : 1.0f / ray.direction[1];
    float dir_frac_z = (ray.direction[2] == 0.0) ? 1.0e32 : 1.0f / ray.direction[2];

    float tx1 = (lb[0] - ray.origin[0]) * dir_frac_x;
    float tx2 = (ub[0] - ray.origin[0]) * dir_frac_x;
    float ty1 = (lb[1] - ray.origin[1]) * dir_frac_y;
    float ty2 = (ub[1] - ray.origin[1]) * dir_frac_y;
    float tz1 = (lb[2] - ray.origin[2]) * dir_frac_z;
    float tz2 = (ub[2] - ray.origin[2]) * dir_frac_z;

    t_in = std::max(std::max(std::min(tx1, tx2), std::min(ty1, ty2)), std::min(tz1, tz2));
    t_out = std::min(std::min(std::max(tx1, tx2), std::max(ty1, ty2)), std::max(tz1, tz2));

    /* When t_out < 0 and the ray is intersecting with AABB, the whole AABB is behind us */
    if (t_out < 0)
    {
        return false;
    }

    return t_out >= t_in;
}

/* construct cell */
Cell::Cell()
{
}

Cell::Cell(Eigen::Vector3f lb, Eigen::Vector3f ub)
{
    bd_box.lb = lb;
    bd_box.ub = ub;
}

void Cell::addTrianle(int v_idx0, int v_idx1, int v_idx2)
{
    is_empty = false;
    v_index.push_back(Eigen::Vector3i(v_idx0, v_idx1, v_idx2));
}



/* uniform gird acceleration structure */
UniformGrid::UniformGrid()
{
}


UniformGrid::UniformGrid(Eigen::Vector3f lb, Eigen::Vector3f ub, int grid_res)
    : lb(lb)
    , ub(ub)
    , cell_size((ub - lb) / grid_res)
    , res_x(grid_res)
    , res_y(grid_res)
    , res_z(grid_res)
{
    constructGrid();
}

void UniformGrid::initialize(AABB bd_box, int grid_res)
{
    lb = bd_box.lb;
    ub = bd_box.ub;
    cell_size = (bd_box.ub - bd_box.lb) / grid_res;
    res_x = grid_res;
    res_y = grid_res;
    res_z = grid_res;
    constructGrid();
}

void UniformGrid::constructGrid()
{
    for (int i = 0; i < res_x; i++) {
        for (int j = 0; j < res_y; j++) {
            for (int k = 0; k < res_z; k++) {
                float lx = lb[0] + i * cell_size[0];
                float ly = lb[1] + j * cell_size[1];
                float lz = lb[2] + k * cell_size[2];
                Cell cell(Eigen::Vector3f(lx, ly, lz), Eigen::Vector3f(lx+cell_size[0], ly+cell_size[1], lz+cell_size[2]));
                this->cells.push_back(cell);
            }
        }
    }
    return;
}

void UniformGrid::addTriangleToCell(Eigen::Vector3i cell_id, int v_idx0, int v_idx1, int v_idx2)
{
    cells[cell_id[0]*res_y*res_z + cell_id[1]*res_z + cell_id[2]].addTrianle(v_idx0, v_idx1, v_idx2);
    return;
}


void UniformGrid::initializeT0(const Ray& ray, Eigen::Vector3f &t0, Eigen::Vector3i &start_cell, float t_in)
{
    Eigen::Vector3f origin_gird = ray.origin - lb;
    if (t_in > 0) origin_gird = ray.origin - lb + t_in * ray.direction;
    
    start_cell <<
        floor(origin_gird[0] / cell_size[0]), 
        floor(origin_gird[1] / cell_size[1]), 
        floor(origin_gird[2] / cell_size[2]);
    if (start_cell[0] == res_x)  start_cell[0] -= 1;
    if (start_cell[1] == res_y)  start_cell[1] -= 1;
    if (start_cell[2] == res_z)  start_cell[2] -= 1;
    
    if (start_cell[0] < 0) start_cell[0] = 0;
    if (start_cell[1] < 0) start_cell[1] = 0;
    if (start_cell[2] < 0) start_cell[2] = 0;

    if (ray.direction[0] > 0) t0[0] = ((start_cell[0]+1) * cell_size[0] - origin_gird[0]) / ray.direction[0];
    else if (ray.direction[0] == 0) t0[0] = 1.0e32;
    else if (ray.direction[0] < 0) t0[0] = ((start_cell[0]) * cell_size[0] - origin_gird[0]) / ray.direction[0];
    
    if (ray.direction[1] > 0) t0[1] = ((start_cell[1]+1) * cell_size[1] - origin_gird[1]) / ray.direction[1];
    else if (ray.direction[1] == 0) t0[1] = 1.0e32;
    else if (ray.direction[1] < 0) t0[1] = ((start_cell[1]) * cell_size[1] - origin_gird[1]) / ray.direction[1];
    
    if (ray.direction[2] > 0) t0[2] = ((start_cell[2]+1) * cell_size[2] - origin_gird[2]) / ray.direction[2];
    else if (ray.direction[2] == 0) t0[2] = 1.0e32;
    else if (ray.direction[2] < 0) t0[2] = ((start_cell[2]) * cell_size[2] - origin_gird[2]) / ray.direction[2];

    if (t_in > 0)
    {
        t0[0] += t_in;
        t0[1] += t_in;
        t0[2] += t_in;
    }

    return;
}

void UniformGrid::initializeTDelta(const Ray& ray, Eigen::Vector3f &t_delta)
{
    t_delta[0] = (ray.direction[0] == 0) ? 1.0e32 : cell_size[0] / abs(ray.direction[0]);
    t_delta[1] = (ray.direction[1] == 0) ? 1.0e32 : cell_size[1] / abs(ray.direction[1]);
    t_delta[2] = (ray.direction[2] == 0) ? 1.0e32 : cell_size[2] / abs(ray.direction[2]);
    return;
}

void UniformGrid::operator=(UniformGrid unif)
{
    lb = unif.lb;
    ub = unif.ub;
    cells = unif.cells;
    cell_size = unif.cell_size;
    res_x = unif.res_x;
    res_y = unif.res_y;
    res_z = unif.res_z;
}


Cell UniformGrid::getCell(int id_x, int id_y, int id_z)
{
    return cells[id_x*res_y*res_z + id_y*res_z + id_z];
}

Cell UniformGrid::getCell(Eigen::Vector3i& target_id)
{
    if (target_id[0] >= res_x || target_id[1] >= res_y || target_id[2] >= res_z)
    {
        std::cout<<"mark\n";
        std::cout<<target_id[0] << target_id[1] << target_id[2] << "\n";
    }
    return cells[target_id[0]*res_y*res_z + target_id[1]*res_z + target_id[2]];
}

