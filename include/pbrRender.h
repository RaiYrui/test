#ifndef __PBRRENDER__
#define __PBRRENDER__
#include <shaderh.h>
#include <glm/glm.hpp>
#include <texture.h>
#include <skybox.h>
struct PBRTexture
{
    Texture *roughness;
    Texture *albedo;
    Texture *ao;
    Texture *metallic;
    Texture *normal;
};
class PBR
{
private:
    GLuint captureFBO, captureRBO;

    GLuint irradianceMap;
    glm::mat4 captureProjection;
    glm::mat4 *captureViews;
    GLuint prefilterMap;
    GLuint brdfLUTTexture;
    Shader *enquire;
    Shader *irradiance;
    Shader *prefilter;
    Shader *brdf;
    GLuint sVAO;
    Skybox skybox;
    unsigned int quadVAO = 0;
    unsigned int quadVBO;

public:
    Texture *hdr;
    GLuint envCubemap;
    PBR();
    void LoadHDR();
    void BindTexture(PBRTexture *pt, Shader *shader);
};

#endif