#include "BsplineSurface.hpp"

/**
 * Evaluate Normal of two vector
 * @param[in] vector1
 * @param[in] vector2
 * @return glm::vec3 is the normal of these two vector.
 */
inline glm::vec3 BsplineSurface::calculateNormal(const glm::vec3& vector1, const glm::vec3& vector2) {
    //finish code in calculateNormal
    return glm::cross(vector1, vector2);
}

/**
 * Evaluate point on a Bspline surface
 * @param[in] u
 * @param[in] v
 * @return glm::vec3 point on the bspline surface  at parameter (u,v).
 */
glm::vec3 BsplineSurface::calculatePos(const float u, const float v, glm::vec3 &normal) {
    //finish your code here
    glm::vec3 normal_u;
    glm::vec3 normal_v;
    vector<glm::vec4> v_interPoint;
    vector<glm::vec4> u_interPoint;

    vector<vector<glm::vec4>> weightedConPoint;

    // construct control point with normal;
    for (size_t i = 0; i < m_cnPoint.size(); i++) {
        vector<glm::vec4> inter;
        for (size_t j = 0; j < m_cnPoint.front().size(); j++) {
            inter.push_back(glm::vec4(m_cnPoint[i][j] * m_weight[i][j], m_weight[i][j]));
        }
        weightedConPoint.push_back(inter);
    }

    for (size_t i = 0; i < weightedConPoint.size(); i++) {
        v_interPoint.push_back(DeBoorCurve(weightedConPoint[i], m_knots_v, v, m_degree, normal_u));
    }
    DeBoorCurve(v_interPoint, m_knots_u, u, m_degree, normal_u);

    // slip the order of control points
    vector<vector<glm::vec4>> cnPoint_flip;
    for (size_t i = 0; i < weightedConPoint.front().size(); i++) {
        vector<glm::vec4> inter;
        for (size_t j = 0; j < weightedConPoint.size(); j++) {
            inter.push_back(weightedConPoint[j][i]);
        }
        cnPoint_flip.push_back(inter);
    }

    // evaluate in another direction
    for (size_t i = 0; i < cnPoint_flip.size(); i++) {
        u_interPoint.push_back(DeBoorCurve(cnPoint_flip[i], m_knots_u, u, m_degree, normal_v));
    }
    glm::vec4 pos = DeBoorCurve(u_interPoint, m_knots_v, v, m_degree, normal_v);
    glm::vec3 res = glm::vec3(pos/pos.w);

    normal = calculateNormal(normal_u, normal_v);
    return res;
}

/**
 * Evaluate point on a Bspline curve
 * @param[in] control_point Control points of the curve with normal.
 * @param[in] knots Knot vector of the curve.
 * @param[in] t Parameter to evaluate the curve at.
 * @return glm::vec3 point on the curve at parameter t.
 */
glm::vec4 BsplineSurface::DeBoorCurve(const vector<glm::vec4>& controlpoint, const vector<float>& knots, const float& t, unsigned int degree, glm::vec3 &tan_vec) {
    // t correspond to knot interval k
    unsigned int insert_n = degree;
    unsigned int k = insertNum(knots, t, insert_n);
    vector<glm::vec4> interPoint;
    // vector<glm::vec4> interPoint;
    // build content for intermediate point
    for (int i = 0; i <= degree; i++) {
        // interPoint.push_back(glm::vec4(controlpoint[i + k - degree], weight[i+k-degree]));
        interPoint.push_back(glm::vec4(controlpoint[k + i - degree]));
    }

    for (int i = 1; i <= degree; i++) {
        for (int j = degree; j >= i; j--) {
            float alpha = (t - knots[j + k - degree]) / (knots[j + 1 + k - i] - knots[j + k - degree]);
            // for t = 1;
            if (knots[j + k - degree] == knots[j + 1 + k - i]) alpha = 1;
            // calculate tangent in in last loop
            if (i == degree && j == i) {
                glm::vec3 p1 = glm::vec3(interPoint[j]/interPoint[j].w);
                glm::vec3 p2 = glm::vec3(interPoint[j-1]/interPoint[j-1].w);
                tan_vec = p1 - p2;
            }
            interPoint[j] = (1.0f - alpha) * interPoint[j-1] + alpha * interPoint[j];
        }
    }
    return glm::vec4(interPoint[degree]);
}


/**
 * Sample point on a given step Bspline surface
 * @param[in] step Step size for you sampling on the surface.
 * @param[out] v_pos the point position for your sampling resolution.
 * @param[out] v_norm the point normal for your sampling resolution.
 */
void BsplineSurface::sampleVertices(float step, vector<glm::vec3>& v_pos, vector<glm::vec3> &v_norm, vector<glm::vec2> &v_tex) {
    //finish your code here.]
    int i = 0, j = 0;
    if (!v_pos.empty() || !v_norm.empty() || !v_norm.empty()) {
        cerr << "position or normal or texture vector is not empty." << endl;
        return;
    }

    for (float u = 0; u <= 1; u += step, i++) {
        for (float v = 0; v <= 1; v += step, j++) {
            glm::vec3 normal;
            glm::vec3 pos = calculatePos(u, v, normal);
            v_pos.push_back(pos);
            v_norm.push_back(normal);
            v_tex.push_back(glm::vec2(u, v));
        }
    }
    return;
}

/**
 * GenerateBuffer object for opengl to render
 * float step  Step size for you sampling on the surface. 
 * @param[out] vers including position and normal
 * @param[out] face indices for render triangles
 * @param[out] edge_indices for render lines
 */
void BsplineSurface::getBuffer_object(vector<Vertex>& vers, vector<unsigned int>& face_indices, vector<unsigned int>& edge_indices, float step) {
    //finish your code here.
    vector<glm::vec3> v_pos;
    vector<glm::vec3> v_norm;
    vector<glm::vec2> v_tex;
    sampleVertices(step, v_pos, v_norm, v_tex);
    
    if (!vers.empty()) return;
    
    for (int i = 0; i < v_pos.size(); i++) {
        Vertex point;
        point.Normal = v_norm[i];
        point.Position = v_pos[i];
        point.Texture = v_tex[i];
        vers.push_back(point);
    }

    if (!edge_indices.empty() || !face_indices.empty()) {
        cerr << "face or edge indices vector not empty." <<endl;
    }

    unsigned int point_num;
    point_num = 1/step + 1;

    for (unsigned int i = 0; i < point_num - 1; i++) {
        for (unsigned int j = 0; j < point_num - 1; j++) {
            // construct face indices
            face_indices.push_back(i*point_num+j);
            face_indices.push_back(i*point_num+j+1);
            face_indices.push_back((i+1)*point_num+j);

            face_indices.push_back(i*point_num+j+1);
            face_indices.push_back((i+1)*point_num+j);
            face_indices.push_back((i+1)*point_num+j+1);
            
            // construct edge indices
            edge_indices.push_back(i*point_num+j);
            edge_indices.push_back(i*point_num+j+1);
            
            edge_indices.push_back(i*point_num+j);
            edge_indices.push_back((i+1)*point_num+j);
            if (j == point_num - 2) {
                edge_indices.push_back((i+1)*point_num-1);
                edge_indices.push_back((i+2)*point_num-1);
            }
            if (i == point_num - 2) {
                edge_indices.push_back((i+1)*point_num+j);
                edge_indices.push_back((i+1)*point_num+j+1);
            }
        }
    }
    return;
}

unsigned int BsplineSurface::insertNum(const vector<float> knots, float t, unsigned int &degree) {
    if (t == 1.0f) t = 0.999f;
    for (size_t i = 0; i < knots.size() - 1; i++) {
        if (knots[i] <= t && t < knots[i+1]) return i;
    }
    // degree = 1;
    // for (size_t i = 0; i < knots.size() - 1; i++) {
    //     if (knots[i] != knots[i+1]) return i;
    // }
    return 0;
}