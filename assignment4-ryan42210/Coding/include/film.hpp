#pragma once
#include <vector>
#include "Eigen/Dense"


struct Film {
    Eigen::Vector2i resolution;
    std::vector<Eigen::Vector3f> pixel_array;

    Film(const Eigen::Vector2i& res)
        : resolution(res)
    {
        pixel_array.resize(resolution.x() * resolution.y(), Eigen::Vector3f(1, 1, 1));
    }

	float getAspectRatio() const
    {
        return (float)(resolution.x()) / (float)(resolution.y());
    }
};
