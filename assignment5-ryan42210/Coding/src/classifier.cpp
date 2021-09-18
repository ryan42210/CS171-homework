#define TINYCOLORMAP_WITH_EIGEN
#include "tinycolormap.hpp"
#include "classifier.hpp"
#include "constant.hpp"
#include "utils.hpp"
#include <algorithm>


/**
 * IsosurfaceClassifier class
 */

IsosurfaceClassifier::IsosurfaceClassifier(float isoval)
    : isovalue(isoval)
{
}

OpticsData IsosurfaceClassifier::transfer(VolumePointData v_data, Camera *cam, const std::vector<Light*>& lights, float dt) const
{
    OpticsData optics;
    optics.color = Eigen::Vector3f::Zero();
    optics.transparency = Eigen::Vector3f(1.0f, 1.0f, 1.0f);
    float delta = v_data.value - isovalue;
    float gauss_delta = expf(-delta*delta/2/pow(dt, 1));
    if (1 - gauss_delta < 0.4) {
        for (auto l : lights) {
            float spec, diff, ambi;
            optics.transparency = Eigen::Vector3f::Zero();
            Eigen::Vector3f in = (l->getPosition() - v_data.position).normalized();
            Eigen::Vector3f n = v_data.gradient.normalized();
            Eigen::Vector3f eye = (cam->getPosition() - v_data.position).normalized();
            Eigen::Vector3f reflect = 2 * n.dot(in) * n - in;
            diff = std::max(n.dot(in), 0.0f);
            spec = pow(std::max(reflect.dot(eye), 0.0f), 32);
            ambi = 0.1f;

            const tinycolormap::Color mapped_color = tinycolormap::GetColor((n[0]+1.0f)/2, tinycolormap::ColormapType::Turbo);

            Eigen::Array3f base_color = Eigen::Array3f(0, 1, 0);
            base_color[0] = mapped_color.r();
            base_color[1] = mapped_color.g();
            base_color[2] = mapped_color.b();
            optics.color += (spec+diff+ambi) * (base_color * l->getColor().array()).matrix();
        }
    }
    return optics;
}

