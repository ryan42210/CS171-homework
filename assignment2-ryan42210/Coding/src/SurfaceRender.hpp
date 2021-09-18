#ifndef SURFACERENDER_HPP
#define SURFACERENDER_HPP
#include "../inc/shader_m.h"
#include "MeshSur.hpp"

class SurfaceRender {
public:
    SurfaceRender() = default;
    SurfaceRender(const MeshSur&);
    ~SurfaceRender() = default;
    void loadSurface(const MeshSur& mesh);
    void loadEdgeSurface(const MeshSur& mesh);
    void draw(const Shader& shader, unsigned int texture);
    void drawEdge(const Shader& shader, unsigned int texture);

private:
    unsigned int VBO;
    unsigned int VAO_e, VAO_f;
    unsigned int EBO_e, EBO_f;
    int verticeNum;
    int faceNum, edgeNum;
    //you can add anything to the skelton code, but remember to construct it in the constructor function and destuctor function
};

#endif
