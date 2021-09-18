#ifndef HEADER_HPP
#define HEADER_HPP
#include <glad/glad.h>
#include <GLFW/glfw3.h>



#ifdef _WIN32
#include <windef.h>
#endif
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
// #include <GL/glu.h>
#endif
#include "../3rdLibs/glm/glm/glm.hpp"
#include "../3rdLibs/glm/glm/gtc/matrix_transform.hpp"
#include "../3rdLibs/glm/glm/gtc/type_ptr.hpp"
#include "../3rdLibs/glm/glm/gtx/string_cast.hpp"
#include <vector>
#include <string>
#include <iostream>
#endif