#include <utility>
#include <random>
#include "camera.hpp"
#include "constant.hpp"
#include "utils.hpp"


Camera::Camera(const Eigen::Vector3f& pos, float focal_len, float v_fov, const Eigen::Vector2i& film_res)
    : film(film_res)
    , vertical_fov(v_fov)
    , position(pos)
    , focal_length(focal_len)
    , forward(0, 0, 1)
    , right(1, 0, 0)
    , up(0, 1, 0)
{
}

void Camera::lookAt(const Eigen::Vector3f& look_at, const Eigen::Vector3f& ref_up)
{
    forward = (position - look_at).normalized();
    right = ref_up.cross(forward).normalized();
    up = forward.cross(right);
}

Ray Camera::generateRay(float dx, float dy) const
{
    Eigen::Vector2f screen_coord(dx, dy);
    Eigen::Vector3f screen_center = position - focal_length * forward;
    Eigen::Vector3f px_pos = screen_center + 
    ((2 * screen_coord.x() / film.resolution.x() - 1) * film.getAspectRatio() * focal_length * tanf(vertical_fov * PI / 360)) * right 
    + ((2 * screen_coord.y() / film.resolution.y() - 1) * focal_length * tanf(vertical_fov * PI / 360) * focal_length) * up;
    Ray ray(position, px_pos - position);
    return ray;
}

// dx and dy are the coordinate in rester space
void Camera::setPixel(int dx, int dy, Eigen::Vector3f value)
{
    film.pixel_array[dy * film.resolution.x() + dx] = value;
}

const Film& Camera::getFilm() const
{
    return film;
}

float Camera::getPxHeight() const
{
    return tanf(vertical_fov * PI / 360) / film.resolution[1];
}

// std::vector<Eigen::Vector2f> Camera::sampleInPx(float dx, float dy, int samples)
// {
//     std::vector<Eigen::Vector2f> sample_pt;
//     std::default_random_engine e;
//     std::uniform_real_distribution<float> u(0.0, 1.0);
//     for (int i = 0; i < samples; i++)
//     {
//         for (int j = 0; j < samples; j++)
//         {
//             float delta_x = u(e);
//             float delta_y = u(e);
//             sample_pt.push_back(Eigen::Vector2f(dx + i * delta_x, dy + j * delta_y));
//         }
//     }
//     return sample_pt;
// }

// std::vector<Eigen::Vector2f> Camera::sampleInPx(float dx, float dy, int samples)
// {
//     std::vector<Eigen::Vector2f> sample_pt;
//     float deg = 30 * PI / 180;
//     Eigen::Matrix2f rotate;
//     rotate << sinf(deg), -cosf(deg), cosf(deg), sinf(deg);
//     Eigen::Vector2f shift1 = Eigen::Vector2f(0.36, 0.36);
//     Eigen::Vector2f shift2 = Eigen::Vector2f(-0.36, 0.36);
//     Eigen::Vector2f shift3 = Eigen::Vector2f(0.36, -0.36);
//     Eigen::Vector2f shift4 = Eigen::Vector2f(-0.36, -0.36);
//     sample_pt.push_back(Eigen::Vector2f(dx, dy) + rotate * shift1);
//     sample_pt.push_back(Eigen::Vector2f(dx, dy) + rotate * shift2);
//     sample_pt.push_back(Eigen::Vector2f(dx, dy) + rotate * shift3);
//     sample_pt.push_back(Eigen::Vector2f(dx, dy) + rotate * shift4);
//     return sample_pt;
// }

std::vector<Eigen::Vector2f> Camera::sampleInPx(float dx, float dy, int samples)
{
    std::vector<Eigen::Vector2f> sample_pt;
    float delta_x = 1.0f/samples;
    float delta_y = 1.0f/samples;
    for (int i = 0; i < samples; i++)
    {
        for (int j = 0; j < samples; j++)
        {
            sample_pt.push_back(Eigen::Vector2f(dx + i * delta_x, dy + j * delta_y));
        }
    }
    return sample_pt;
}