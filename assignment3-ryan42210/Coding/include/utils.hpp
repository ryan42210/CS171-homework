#pragma once
#include <iostream>
#include "Eigen/Dense"

#define UNREACHABLE std::cout << "Error: Unreachable code executed. Exit(-1)..." << std::endl; exit(-1);


/* Clamps the input x to the closed range [lo, hi] */
float clamp(float x, float lo, float hi);
float compress(float x, float lo, float hi);
/* Performs Gamma correction on x and outputs an integer between 0-255. */
unsigned char gamma_correction(float x);
/* Uniformly samples a real number in [a, b] */
float uniform_sample(float a, float b);

float sawtooth(float x, float period);

Eigen::Vector2i findInterval(float value, unsigned int px_num);