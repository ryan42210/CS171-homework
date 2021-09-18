#ifndef NURBS_DATA_H
#define NURBS_DATA_H

#include "../3rdLibs/glm/glm/glm.hpp"
#include "../3rdLibs/glm/glm/gtc/type_ptr.hpp"
#include "../3rdLibs/glm/glm/gtc/matrix_transform.hpp"

#include<vector>

using namespace std;

vector<vector<glm::vec3>> nurbs_cnPoint = {
{
    glm::vec3(1, 0, 0),
    glm::vec3(1, 0, 1),
    glm::vec3(0, 0, 1),
    glm::vec3(-1, 0, 1),
    glm::vec3(-1, 0, 0),
},{
    glm::vec3(1, 0, 0),
    glm::vec3(1, 1, 1),
    glm::vec3(0, 1, 1),
    glm::vec3(-1, 1, 1),
    glm::vec3(-1, 0, 0),
},{
    glm::vec3(1, 0, 0),
    glm::vec3(1, 1, 0),
    glm::vec3(0, 1, 0),
    glm::vec3(-1, 1, 0),
    glm::vec3(-1, 0, 0),
},{
    glm::vec3(1, 0, 0),
    glm::vec3(1, 1, -1),
    glm::vec3(0, 1, -1),
    glm::vec3(-1, 1, -1),
    glm::vec3(-1, 0, 0),
},{
    glm::vec3(1, 0, 0),
    glm::vec3(1, 0, -1),
    glm::vec3(0, 0, -1),
    glm::vec3(-1, 0, -1),
    glm::vec3(-1, 0, 0),
},{
    glm::vec3(1, 0, 0),
    glm::vec3(1, -1, -1),
    glm::vec3(0, -1, -1),
    glm::vec3(-1, -1, -1),
    glm::vec3(-1, 0, 0),
},{
    glm::vec3(1, 0, 0),
    glm::vec3(1, -1, 0),
    glm::vec3(0, -1, 0),
    glm::vec3(-1, -1, 0),
    glm::vec3(-1, 0, 0),
},{
    glm::vec3(1, 0, 0),
    glm::vec3(1, -1, 1),
    glm::vec3(0, -1, 1),
    glm::vec3(-1, -1, 1),
    glm::vec3(-1, 0, 0),
},{
    glm::vec3(1, 0, 0),
    glm::vec3(1, 0, 1),
    glm::vec3(0, 0, 1),
    glm::vec3(-1, 0, 1),
    glm::vec3(-1, 0, 0),
}
};

// vector<vector<float>> nurbs_weight = {
//     {1, 1, 1, 1, 1, 1, 1, 1, 1},
//     {0.7071, 0.5, 0.7071, 0.5, 0.7071, 0.5, 0.7071, 0.5, 0.7071},
//     {1, 0.7071, 1, 0.7071, 1, 0.7071, 1, 0.7071, 1},
//     {0.7071, 0.5, 0.7071, 0.5, 0.7071, 0.5, 0.7071, 0.5, 0.7071},
//     {1, 1, 1, 1, 1, 1, 1, 1, 1},
// };

vector<vector<float>> nurbs_weight = {
    {1, 0.7071, 1, 0.7071, 1},
    {1, 0.5, 0.7071, 0.5, 1},
    {1,0.7071, 1, 0.7071, 1},
    {1, 0.5,0.7071, 0.5 , 1},
    {1,0.7071, 1, 0.7071, 1},
    {1, 0.5,0.7071, 0.5 , 1},
    {1,0.7071, 1, 0.7071, 1},
    {1, 0.5,0.7071, 0.5 , 1},
    {1,0.7071, 1, 0.7071, 1},
};

vector<float> kts_u = {0, 0, 0, 0.25, 0.25, 0.5, 0.5, 0.75, 0.75, 1, 1, 1};
vector<float> kts_v = {0, 0, 0, 0.5, 0.5, 1, 1, 1};

#endif