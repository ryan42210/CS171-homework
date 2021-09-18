#pragma once
#include <string>
#include <iostream>
#include <fstream>
#ifndef NO_OMP
#include <omp.h>
#endif
#include "utils.hpp"


#ifndef NO_OMP
#define CONF_DEFUALT_NUM_THREADS omp_get_max_threads()
#else
#define CONF_DEFUALT_NUM_THREADS 1
#endif
#define CONF_DEFAULT_OUTPUT_FILE "output.png"
#define CONF_DEFAULT_OUTPUT_RESOLUTION 1024
#define CONF_DEFAULT_CAMERA_ID 1
#define CONF_DEFAULT_LIGHT_ID 1
#define CONF_DEFAULT_DELTA_MAT false
#define CONF_DEFAULT_SPP 16
#define CONF_DEFAULT_MAX_DEPTH 5
#define CONF_DEFAULT_INTEGRAL_METHOD "mis"


struct Config
{
    /* The number of using threads */
    int num_threads;
    /* Output file */
    std::string output_file;
    /* Resolution of output */
    int output_resolution;
    /* ID of camera settings: 1 2 3 */
    int camera_id;
    /* ID of light settings: 1 2 */
    int light_id;
    /* Whether delta materials are supported, e.g., specular or transmisson */
    bool enable_delta_mat;
    /* Samples per pixel */
    int spp;
    /* The max number of reflection times */
    int max_depth;
    /* Name of integral method:
       * mis: multiple importance sampling
       * bso: BRDF sampling only
       * lso: Light sampling only
     */
    std::string integral_method;
    

    Config()
        : num_threads(CONF_DEFUALT_NUM_THREADS)
        , output_file(CONF_DEFAULT_OUTPUT_FILE)
        , output_resolution(CONF_DEFAULT_OUTPUT_RESOLUTION)
        , camera_id(CONF_DEFAULT_CAMERA_ID)
        , light_id(CONF_DEFAULT_LIGHT_ID)
        , enable_delta_mat(CONF_DEFAULT_DELTA_MAT)
        , spp(CONF_DEFAULT_SPP)
        , max_depth(CONF_DEFAULT_MAX_DEPTH)
        , integral_method(CONF_DEFAULT_INTEGRAL_METHOD)
    {
    }

    bool parseConfigFile(std::string file_path)
    {
        std::string token;
        std::ifstream file(file_path);

        std::string line;
        int line_num = 1;
        while (std::getline(file, line))
        {
            strutils::trim(line);

            if (line.find("#") == 0 || line.empty())
            {
                continue;
            }

            std::stringstream line_stream(line);
            std::string key, val;
            line_stream >> key;
            if (!(line_stream >> val)) {
                std::cout << "Config: Wrong syntax at " << file_path << ":" << line_num << ". Value field not found." << std::endl;
                return false;
            }

            if (key == "output_resolution")
            {
                output_resolution = std::stoi(val);
            }
            else if (key == "output_file")
            {
                output_file = val;
            }
            else if (key == "camera_id")
            {
                camera_id = std::stoi(val);
            }
            else if (key == "light_id")
            {
                light_id = std::stoi(val);
            }
            else if (key == "delta_mat")
            {
                enable_delta_mat = (val == "true" || val == "1");
            }
            else if (key == "num_threads")
            {
                num_threads = std::stoi(val);
            }
            else if (key == "spp")
            {
                spp = std::stoi(val);
            }
            else if (key == "max_depth")
            {
                max_depth = std::stoi(val);
            }
            else if (key == "integral_method")
            {
                integral_method = val;
            }

            if (line_stream >> key)
            {
                std::cout << "Config: Wrong syntax at " << file_path << ":" << line_num << ". Multiple values are not supported." << std::endl;
                return false;
            }

            ++line_num;
        }

        return true;
    }

    void printConfig()
    {
        std::cout << "=====================" << std::endl;
#ifndef NO_OMP
        std::cout << "max number of threads: " << omp_get_max_threads() << std::endl;
        omp_set_num_threads(num_threads);
#pragma omp parallel
        {
#pragma omp single
            {
                std::cout << "number of using threads: " << omp_get_num_threads() << std::endl;
            }
        }
#endif
        std::cout << "output file: " << output_file << std::endl;
        std::cout << "output resolution: " << output_resolution << " x " << output_resolution << std::endl;
        std::cout << "samples per pixel: " << spp << std::endl;
        std::cout << "max depth: " << max_depth << std::endl;
        std::cout << "integral method: " << integral_method << std::endl;
        std::cout << "camera setting: " << camera_id << std::endl;
        std::cout << "light setting: " << light_id << std::endl;
        std::cout << "enable delta materials: " << enable_delta_mat << std::endl;
        std::cout << "=====================" << std::endl;
        std::cout << std::endl;
    }

};
