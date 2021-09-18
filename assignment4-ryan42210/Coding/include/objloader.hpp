#pragma once
#include <vector>
#include <Eigen/Dense>


//////////////////////////////////////////////////////////////////////////////////////////////
// Explaination for loadObj:                                                                //
//      path: path of obj file.                                                             //
//      out_vertices: store pos dictionary                                                  //
//      out_uvs: store uv dictionary                                                        //
//      out_normals: store normal dictionary                                                //
//      out_v_index: index of pos                                                           //
//      out_vt_index: index of uv                                                           //
//      out_vn_index: index of normal                                                       //
//                                                                                          //
//      take out_v_index for example: it should be :                                        //
//      pos1  pos2  pos3                                                                    //
//         0,    4,    6,  // first triangle, begin from 0                                  //
//         3,    5,    8,  // seconde                                                       //
//         ...                                                                              //
//////////////////////////////////////////////////////////////////////////////////////////////

bool loadObj(
    const char* path,
    std::vector <Eigen::Vector3f>& out_vertices,
    std::vector <Eigen::Vector2f>& out_uvs,
    std::vector <Eigen::Vector3f>& out_normals,
    std::vector <int>& out_v_index,
    std::vector <int>& out_vt_index,
    std::vector <int>& out_vn_index
);

