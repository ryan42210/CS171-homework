#ifndef BSPLINESURFACE_HPP
#define BSPLINESURFACE_HPP
#include "vertex.hpp"
using namespace std;
class BsplineSurface {
public:
    BsplineSurface() = default;
    BsplineSurface(const vector<vector<glm::vec3>>& cnPoint, const vector<float>& knots_u, vector<float>& knots_v, vector<vector<float>>& weight, unsigned int degree)
    {
        this->m_cnPoint = cnPoint;
        this->m_knots_u = knots_u;
        this->m_knots_v = knots_v;
        this->m_degree = degree;
        this->m_weight = weight;
    }
    BsplineSurface(const BsplineSurface& sur)
    {
        this->m_cnPoint = sur.m_cnPoint;
        this->m_knots_u = sur.m_knots_u;
        this->m_knots_v = sur.m_knots_v;
        this->m_degree = sur.m_degree;
        this->m_weight = sur.m_weight;
    }
    BsplineSurface& operator=(const BsplineSurface& sur)
    {
        this->m_cnPoint = sur.m_cnPoint;
        this->m_knots_u = sur.m_knots_u;
        this->m_knots_v = sur.m_knots_v;
        this->m_degree = sur.m_degree;
        this->m_weight = sur.m_weight;
        return *this;
    }
    ~BsplineSurface() = default;

    //func
    inline glm::vec3 calculateNormal(const glm::vec3& vector1, const glm::vec3& vector2);

    glm::vec3 calculatePos(const float u, const float v, glm::vec3 &normal);

    glm::vec4 DeBoorCurve(const vector<glm::vec4>& controlpoint, const vector<float>& knots, const float& t, unsigned int degree, glm::vec3 &tan_vec);

    void sampleVertices(float step, vector<glm::vec3>& v_pos, vector<glm::vec3> &v_norm, vector<glm::vec2> &v_tex);
    
    void getBuffer_object(vector<Vertex>& vers, vector<unsigned int>& face_indices, vector<unsigned int>& edge_indices, float step = 0.02f);
    
    unsigned int insertNum(const vector<float> knots, float t, unsigned int &degree);

private:
    vector<vector<glm::vec3>> m_cnPoint;
    vector<float> m_knots_u;
    vector<float> m_knots_v;
    vector<vector<float>> m_weight;
    unsigned int m_degree;
    //you can add anything to the skelton code, but remember to construct it in the constructor function and destructor function
};

#endif