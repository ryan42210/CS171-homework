#pragma once
#include <iostream>
#include <string>
#include "Eigen/Dense"

#define UNREACHABLE std::cout << "Error: Unreachable code executed. Exit(-1)..." << std::endl; exit(-1);


/**
 * String processing utilities
 */

namespace strutils {
    /* Trim from left */
    void ltrim(std::string& s);
    /* Trim from right */
    void rtrim(std::string& s);
    /* Trim from both left and right */
    void trim(std::string& s);
};


/**
 * Mathematical utilities
 */

namespace mathutils {
    /* Clamps the input x to the closed range [lo, hi] */
    float clamp(float x, float lo, float hi);
    /* Performs Gamma correction on x and outputs an integer between 0-255. */
    unsigned char gamma_correction(float x);
    /* Uniformly sample N numbers among [a, b] */
    std::vector<float> unif(float a, float b, int N = 1);
    /* grid sample in a [0,1] x [0,1] */
    std::vector<Eigen::Vector2f> girdsample(int samples);
};

