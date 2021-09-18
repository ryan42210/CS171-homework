#include <random>
#include <algorithm>
#include "utils.hpp"
#include "constant.hpp"


/**
 * strutils namespace
 */

void strutils::ltrim(std::string& s)
{
   s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
}

void strutils::rtrim(std::string& s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}

void strutils::trim(std::string& s)
{
    ltrim(s);
    rtrim(s);
}


/**
 * mathutils namespace
 */


float mathutils::clamp(float x, float lo, float hi)
{
    return x < lo ? lo : x > hi ? hi : x;
}

unsigned char mathutils::gamma_correction(float x)
{
	return (unsigned char)(pow(mathutils::clamp(x, 0.0, 1.0), 1 / 2.2) * 255);
}

static std::default_random_engine random_generator;

std::vector<float> mathutils::unif(float a, float b, int N)
{
    std::vector<float> res;
    std::uniform_real_distribution<float> dis(a, b);

    for (int i = 0; i < N; i++) {
        res.push_back(dis(random_generator));
    }
    return res;
}

std::vector<Eigen::Vector2f> mathutils::girdsample(int samples)
{
    samples = sqrt(samples);
    std::vector<Eigen::Vector2f> sample_pt;
    float delta_x = 1.0f/samples;
    float delta_y = 1.0f/samples;
    for (int i = 0; i < samples; i++)
    {
        for (int j = 0; j < samples; j++)
        {
            sample_pt.push_back(Eigen::Vector2f(i * delta_x, j * delta_y));
        }
    }
    return sample_pt;
}