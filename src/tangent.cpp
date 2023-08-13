#include <tangent.h>
void TangentBasis(std::vector<glm::vec3> &vertex, std::vector<glm::vec2> &uv, std::vector<glm::vec3> &normal,
                  std::vector<glm::vec3> &tangent, std::vector<glm::vec3> &bitangent)
{
    for (unsigned int i = 0; i < vertex.size(); i += 3)
    {
        glm::vec3 &v0 = vertex[i + 0];
        glm::vec3 &v1 = vertex[i + 1];
        glm::vec3 &v2 = vertex[i + 2];

        glm::vec2 &uv0 = uv[i + 0];
        glm::vec2 &uv1 = uv[i + 1];
        glm::vec2 &uv2 = uv[i + 2];

        glm::vec3 deltapos1 = v1 - v0;
        glm::vec3 deltapos2 = v2 - v0;

        glm::vec2 deltauv1 = uv1 - uv0;
        glm::vec2 deltauv2 = uv2 - uv0;

        float r = 1.0f / (deltauv1.x * deltauv2.y - deltauv1.y * deltauv2.x);
        glm::vec3 t = (deltapos1 * deltauv2.y - deltapos2 * deltauv1.y) * r;
        glm::vec3 b = (deltapos2 * deltauv1.x - deltapos1 * deltauv2.x) * r;

        tangent.push_back(t);
        tangent.push_back(t);
        tangent.push_back(t);

        bitangent.push_back(b);
        bitangent.push_back(b);
        bitangent.push_back(b);
    }
    for (unsigned int i = 0; i < vertex.size(); ++i)
    {
        glm::vec3 &n = normal[i];
        glm::vec3 &t = tangent[i];
        glm::vec3 &b = bitangent[i];

        t = glm::normalize(t - n * glm::dot(n, t));

        if (glm::dot(glm::cross(n, t), b) < 0.0f)
        {
            t = t * -1.0f;
        }
    }
}