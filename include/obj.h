#ifndef __OBJLOAD__
#define __OBJLOAD__
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <shaderh.h>
#include <glm/glm.hpp>
#include <tangent.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
const int ASSIMPLOAD = 9;
const int NORMALLOAD = 8;
class OBJloader
{
public:
    OBJloader(const char *path, int way = 0, int mode = 0);
    void Draw(Shader shader);
    void Changeobj(const char *path);

private:
    std::vector<glm::vec3> vertex, normal;
    std::vector<glm::vec2> uv;
    GLuint VAO, VBO;
    GLuint vertexbuffer, uvbuffer, normalbuffer, tangentbuffer, bitangentbuffer;
    void setUp();
    void setUpnormal();
    bool LoadOBJ(const char *path,
                 std::vector<glm::vec3> &out_vertices,
                 std::vector<glm::vec2> &out_uvs,
                 std::vector<glm::vec3> &out_normals);
    // bool ASSIMPLoad(const char *path, std::vector<glm::vec3> &vertex, std::vector<glm::vec2> &uv, std::vector<glm::vec3> &noraml);
};

#endif