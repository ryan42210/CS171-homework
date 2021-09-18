#ifndef LOADMESH_H
#define LOADMESH_H

#include<iostream>
#include<fstream>
#include<vector>
#include<string>

struct Mesh {
    int v_num, f_num;
    std::vector<float> vertexAttrib;
    std::vector<unsigned int> indices;
};

// load .object file
Mesh loadmesh(std::string meshfile) {
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

    std::vector<float> vertexAttrib;
    for (int i = 0; i < v_num; ++i) {
        vertexAttrib.push_back(vertices[i*3]);
        vertexAttrib.push_back(vertices[i*3 + 1]);
        vertexAttrib.push_back(vertices[i*3 + 2]);
        vertexAttrib.push_back(normal[i*3]);
        vertexAttrib.push_back(normal[i*3 + 1]);
        vertexAttrib.push_back(normal[i*3 + 2]);
    }

    std::vector<unsigned int> indices;
    for (int i = 0; i < f_num * 3; ++i) {
        unsigned int index;
        infile >> index;
        indices.push_back(index-1);
    }
    infile.close();

    Mesh obj = {v_num, f_num, vertexAttrib, indices};
    return obj;
}

#endif