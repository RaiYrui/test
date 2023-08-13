#ifndef __TANGENT__
#define __TANGENT__
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
void TangentBasis(std::vector<glm::vec3> &vertex, std::vector<glm::vec2> &uv, std::vector<glm::vec3> &normal,
                  std::vector<glm::vec3> &tangent, std::vector<glm::vec3> &bitangent);
#endif