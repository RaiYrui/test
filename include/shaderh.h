#ifndef __SHADERH__
#define __SHADERH__
#include <iostream>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Shader
{
public:
    GLuint ID;
    Shader(const char *vpath, const char *fpath);
    Shader(const char *vpath, const char *fpath, const char *gpath);
    void use() const;
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setMat4(const std::string &name, glm::mat4 &value) const;
    void setvec3(const std::string &name, glm::vec3 value) const;
};

#endif