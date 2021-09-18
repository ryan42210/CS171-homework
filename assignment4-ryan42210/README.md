# Assignment 4: Ray Tracing with Global Illumination

- Student Name: Zhanrui Zhang
- Student ID: 2019533227

Please read the [Assignment 4](http://faculty.sist.shanghaitech.edu.cn/faculty/liuxp/course/cg1/assignment4/assignment4.html) carefully.

## Instructions

### Compilation

The code skeleton utilizes cmake as the project building tool. We have added OpenMP support for you in the code skeleton. In case your environment is not compatible with OpenMP, we offer a macro named `NO_OMP` to enable/disable OpenMP in the cmake generation step:

```bash
# Suppose your present working directory is Coding/build/

# Enable OpenMP
cmake -DNO_OMP=0 ..

# Disable OpenMP
cmake -DNO_OMP=1 ..
```

### Execution

Once you manage to compile the project, you can run your ray tracing via typing the command below.

```bash
# Suppose your present working directory is Coding/build/

./rayTracing ../configs/config_all_diff.txt
```

The "config_all_diff.txt" is a configuration file for rendering diffusion-only scenes, where you can specify the number of available threads, samples per pixel, camera/light settings, output resolutions, etc. It is a plain-text file, where each option is stored in the format `key value`. You can edit "config.hpp" to add your own options if necessary.


## Third-party Libraries

Third-party libraries are placed in the `3rdLibs/` directory. You are NOT allowed to use any third-party libraries other than:

- Eigen
- progressbar (by Luigi Pertoldi)
- stb_image_write


## Copyright

Copyright 2021 ShanghaiTech Univeristy. Code skeleton by Yuehao Wang.
