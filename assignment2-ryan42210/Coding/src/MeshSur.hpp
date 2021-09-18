#ifndef MESHSUR_HPP
#define MESHSUR_HPP

#include "BsplineSurface.hpp"
#include "ControlPoint.hpp"
#include "vertex.hpp"

class MeshSur {
public:
    MeshSur() = default;
    MeshSur(const vector<float> knots_u, const vector<float>& knots_v, float step)
    {
        this->knots_u = knots_u;
        this->knots_v = knots_v;
        this->sampleStep = step;
    }
    ~MeshSur() = default;
    //func
    void initialize(const vector<vector<glm::vec3>>& controlpoint, const vector<vector<float>> &weight, int degree);

    //set
    void setSampleStep(float step);

    //getter
    vector<Vertex> getVertices() const {
        return vers;
    }

    vector<unsigned int> getFaceIndices() const {
        return face_indices;
    }
    vector<unsigned int> getEdgeIndices() const
    {
        return edge_indices;
    }

private:
    vector<vector<glm::vec3>> controlpoint;
    vector<float> knots_u;
    vector<float> knots_v;
    vector<vector<float>> weight;
    float sampleStep = 0.01;
    BsplineSurface surf;

    //out
    vector<Vertex> vers;
    vector<unsigned int> face_indices;
    vector<unsigned int> edge_indices;

    //you can add anything to the skelton code, but remember to construct it in the constructor function and destuctor function
};

#endif