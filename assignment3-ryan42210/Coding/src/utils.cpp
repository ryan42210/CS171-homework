#include <random>
#include "utils.hpp"


float clamp(float x, float lo, float hi)
{
    return x < lo ? lo : x > hi ? hi : x;
}

float compress(float x, float lo, float hi)
{
    return (x - lo)/(hi - lo);
}

unsigned char gamma_correction(float x)
{
	return (unsigned char)(pow(clamp(x, 0.0, 1.0), 1 / 1.5) * 255);
}


static std::default_random_engine random_generator;

float uniform_sample(float a, float b)
{
    std::uniform_real_distribution<float> dis(a, b);
    return dis(random_generator);
}

// return a float number in [0, 1] 
float sawtooth(float x, float period)
{
    float tmp = fmodf(x, period);
    if (tmp < 0) tmp += period;
    float res = compress(tmp, 0, period);
    return res;
}

Eigen::Vector2i findInterval(float value, unsigned int px_num)
{
    if (value < 0.5) return Eigen::Vector2i(px_num-1, 0);
    else if (value >= px_num - 0.5) return Eigen::Vector2i(px_num - 1, 0);
    // if (value < 0.5) return Eigen::Vector2i(px_num - 1, 0);
    // else if (value >= px_num - 0.5) return Eigen::Vector2i(0, px_num - 1);
    else 
    {
        Eigen::Vector2i id_pair;
        id_pair << (int)floorf(value - 0.5), (int)ceilf(value - 0.5);
        return id_pair;
    }
}