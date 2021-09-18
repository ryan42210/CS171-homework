#ifndef LOADMESH_M_H
#define LOADMESH_M_H

#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include "../3rdLibs/glm/glm/glm.hpp"
#include "../3rdLibs/glm/glm/gtc/type_ptr.hpp"
#include "../3rdLibs/glm/glm/gtc/matrix_transform.hpp"


struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
};

struct Mesh_m {
    int v_num, f_num;
    std::vector<Vertex> vertexAttrib;
    std::vector<unsigned int> indices;
};
// load .object file
Mesh_m loadmesh_m(std::string meshfile) {
    std::ifstream infile;
    infile.open(meshfile, std::ios::in);
    if (!infile.is_open()) {
        std::cout << "fail to open mesh file" << std::endl;
    }

    int v_num, f_num;
    infile >> v_num >> f_num;

    std::vector<float> vertices;
    for (int i = 0; i < v_num * 3; ++i) {
        float v_cordi;
        infile >> v_cordi;
        vertices.push_back(v_cordi);
    }
    std::vector<float> normal;
    for (int i = 0; i < v_num * 3; ++i) {
        float n;
        infile >> n;
        normal.push_back(n);
    }

    std::vector<Vertex> vertexAttrib;
    for (int i = 0; i < v_num; ++i) {
        Vertex v;
        v.position.x = vertices[3*i];
        v.position.y = vertices[3*i+1];
        v.position.z = vertices[3*i+2];
        v.normal.x = vertices[3*i];
        v.normal.y = vertices[3*i+1];
        v.normal.z = vertices[3*i+2];
        vertexAttrib.push_back(v);
    }

    std::vector<unsigned int> indices;
    for (int i = 0; i < f_num * 3; ++i) {
        unsigned int index;
        infile >> index;
        indices.push_back(index-1);
    }
    infile.close();

    Mesh_m obj = {v_num, f_num, vertexAttrib, indices};
    return obj;
}

#endif