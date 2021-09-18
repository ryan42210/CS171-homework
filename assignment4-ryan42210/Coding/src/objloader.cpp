#include <string>
#include <map>
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.hpp"
#include "objloader.hpp"


bool loadObj(const char* path, std::vector<Eigen::Vector3f>& out_vertices,
    std::vector<Eigen::Vector2f>& out_uvs, std::vector<Eigen::Vector3f>& out_normals,
    std::vector<int>& out_v_index, std::vector<int>& out_vt_index, std::vector<int>& out_vn_index)
{
    printf("-- Loading model %s\n", path);

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn;
    std::string err;

    bool success = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path);
    if (!success)
    {
        printf("Failed to open %s: %s\n", path, err.c_str());
        return false;
    }
    if (!warn.empty())
    {
        printf("Warning at loading %s: %s\n", path, warn.c_str());
        return false;
    }
    if (shapes.size() == 0)
    {
        printf("No shape in %s", path);
        return false;
    }
    else if (shapes.size() > 1)
    {
        printf("Not Supported: more than 1 shapes found in %s", path);
        return false;
    }

    tinyobj::shape_t* sh = &shapes[0];
    tinyobj::mesh_t* mesh = &(sh->mesh);

    for (size_t i = 0; i < attrib.vertices.size(); i += 3)
    {
        out_vertices.push_back(Eigen::Vector3f(attrib.vertices[i], attrib.vertices[i + 1], attrib.vertices[i + 2]));
    }
    for (size_t i = 0; i < attrib.texcoords.size(); i += 2)
    {
        out_uvs.push_back(Eigen::Vector2f(attrib.texcoords[i], attrib.texcoords[i + 1]));
    }
    for (size_t i = 0; i < attrib.normals.size(); i += 3)
    {
        out_normals.push_back(Eigen::Vector3f(attrib.normals[i], attrib.normals[i + 1], attrib.normals[i + 2]));
    }

    size_t index_offset = 0;
    for (size_t f = 0; f < mesh->num_face_vertices.size(); ++f)
    {
        int fv = mesh->num_face_vertices[f];
        if (fv > 3)
        {
            printf("Warning at loading %s: faces with more than 3 vertices are not supported.", path);
            return false;
        }
        for (size_t v = 0; v < fv; ++v)
        {
            tinyobj::index_t idx = mesh->indices[index_offset + v];

            out_v_index.push_back(idx.vertex_index);
            out_vt_index.push_back(idx.texcoord_index);
            out_vn_index.push_back(idx.normal_index);
        }
        index_offset += fv;
    }

    printf("  # vertices: %d\n", attrib.vertices.size());
    printf("  # faces: %d\n", mesh->num_face_vertices.size());

    return true;
}