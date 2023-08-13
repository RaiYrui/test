#ifndef __SKYBOX__
#define __SKYBOX__
#include <iostream>
#include <glad/glad.h>
class Skybox
{
public:
    GLuint static Initcube();
    GLuint static Initskybox();
};
#endif