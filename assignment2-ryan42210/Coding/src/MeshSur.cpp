#include "MeshSur.hpp"

void MeshSur::initialize(const vector<vector<glm::vec3>>& controlpoint, const vector<vector<float>> &weight, int degree) {
    this->controlpoint = controlpoint;
    this->weight = weight;
    surf = BsplineSurface(controlpoint, knots_u, knots_v, this->weight, degree);
    surf.getBuffer_object(vers, face_indices, edge_indices, sampleStep);
}

void MeshSur::setSampleStep(float step)
{
    this->sampleStep = step;
}