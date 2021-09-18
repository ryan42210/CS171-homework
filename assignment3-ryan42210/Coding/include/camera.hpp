#pragma once
#include "Eigen/Dense"
#include "film.hpp"
#include "ray.hpp"


class Camera {
protected:
    Eigen::Vector3f position;
    Eigen::Vector3f forward;
    Eigen::Vector3f right;
    Eigen::Vector3f up;
    float vertical_fov;
    float focal_length;
    Film film;

public:
    Camera(const Eigen::Vector3f& pos, float focal_len, float v_fov, const Eigen::Vector2i& film_res);
	void lookAt(const Eigen::Vector3f& look_at, const Eigen::Vector3f& ref_up);
    Ray generateRay(float dx, float dy) const;
    std::vector<Eigen::Vector2f> sampleInPx(float dx, float dy, int samples);
    void setPixel(int dx, int dy, Eigen::Vector3f value);
    const Film& getFilm() const;
    float getPxHeight() const;
};
