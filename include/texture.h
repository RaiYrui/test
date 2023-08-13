
#ifndef __TEXTUREL__
#define __TEXTUREL__
#include <iostream>
#include <stb_image.h>
#include <glad/glad.h>
#include <vector>
struct Texture
{
    int width;
    int height;
    GLuint id;
    char *path;
};
Texture *Loadtexture(const char *path);
Texture *LoadHDRimage(const char *path);
GLuint LoadSkytex();
GLuint loadBMP_custom(const char *imagepath);
unsigned char *loadBMP(const char *imagepath, unsigned int *width, unsigned int *height);
#endif