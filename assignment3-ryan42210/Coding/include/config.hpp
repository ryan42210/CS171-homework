#pragma once
#include <string>
#include <iostream>
#include <fstream>
#ifndef NO_OMP
#include <omp.h>
#endif


#ifndef NO_OMP
#define CONF_DEFUALT_NUM_THREADS omp_get_max_threads()
#else
#define CONF_DEFUALT_NUM_THREADS 1
#endif
#define CONF_DEFAULT_OUTPUT_FILE "output.png"
#define CONF_DEFAULT_OUTPUT_RESOLUTION 1024
#define CONF_DEFAULT_CAMERA_ID 1
#define CONF_DEFAULT_LIGHT_ID 1
#define CONF_DEFAULT_NUM_SAMPLES 16
#define CONF_DEFAULT_SCENE_ID 1


struct Config
{
    int num_threads;
    std::string output_file;
    int output_resolution;
    int camera_id;
    int light_id;
    int num_samples;
    int scene_id;

    Config()
        : num_threads(CONF_DEFUALT_NUM_THREADS)
        , output_file(CONF_DEFAULT_OUTPUT_FILE)
        , output_resolution(CONF_DEFAULT_OUTPUT_RESOLUTION)
        , camera_id(CONF_DEFAULT_CAMERA_ID)
        , light_id(CONF_DEFAULT_LIGHT_ID)
        , num_samples(CONF_DEFAULT_NUM_SAMPLES)
        , scene_id(CONF_DEFAULT_SCENE_ID)
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
            else if (key == "num_threads")
            {
                num_threads = std::stoi(val);
            }
            else if (key == "num_samples")
            {
                num_samples = std::stoi(val);
            }
            else if (key == "scene_id")
            {
                scene_id = std::stoi(val);
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
        std::cout << "number of samples: " << num_samples << std::endl;
        std::cout << "camera setting: " << camera_id << std::endl;
        std::cout << "light setting: " << light_id << std::endl;
        std::cout << "scene choice: " << scene_id << std::endl;
        std::cout << "=====================" << std::endl;
        std::cout << std::endl;
    }

};
