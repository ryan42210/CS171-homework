### **Introduction**

In this assignment, you are required to create your first graphics program using [OpenGL](https://www.opengl.org/). As you have learned in the course as well as the tutorials, to start up your first graphics program, you will need to use [GLFW](http://www.glfw.org/) to create a window. By default, GLFW will create a window with double  buffering enabled. Then, you can use the basic OpenGL calls to draw 3D  objects on the window as well as manipulate the camera to navigate  through the virtual scene. To reduce aliasing artifacts, you can also  enable antialiasing in OpenGL by multi-sampling.

The skeleton homework template is built with [GLFW](https://www.glfw.org/)+[GLAD](https://glad.dav1d.de/), you can build the template with [cmake](https://cmake.org/) in the following platform.

#### Ubuntu

```bash
sudo apt-get install cmake
```

Install the glfw.

```bash
sudo apt-get install libglfw3-dev
git clone https://github.com/glfw/glfw
mkdir build && cd build && cmake ../glfw && make -j4 && sudo make install
```

if something is missing ,you can install the following package.

```
sudo apt-get install libxinerama-dev

sudo apt-get install libxcursor-dev

sudo apt-get install libxi-dev
```

```bash
git clone https://github.com/SHTU-CS17101/cs17101-hw1-xxxx.git
mkdir build && cd build && cmake ..
```

#### Windows

```bash
git clone https://github.com/SHTU-CS17101/cs17101-hw1-xxxx.git
mkdir build
cd build
cmake ..
```
#### Mac

```bash
git clone https://github.com/SHTU-CS17101/cs17101-hw1-xxxx.git
mkdir build && cd build && cmake ..
```

