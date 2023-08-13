#include <stb_image.h>
#include <texture.h>
Texture *Loadtexture(const char *path)
{
    Texture *temp = new Texture();

    try
    {
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        int width, height, Channel;
        unsigned char *data = stbi_load(path, &width, &height, &Channel, 0);

        if (data)
        {
            GLenum fomat;
            if (Channel == 1)
                fomat = GL_RED;
            else if (Channel == 3)
                fomat = GL_RGB;
            else if (Channel == 4)
                fomat = GL_RGBA;
            glTexImage2D(GL_TEXTURE_2D, 0, fomat, width, height, 0, fomat, GL_UNSIGNED_BYTE, data);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }

        stbi_image_free(data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
        temp->height = height;
        temp->width = width;
        temp->path = (char *)path;
        temp->id = texture;
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
    return temp;
}
GLuint LoadSkytex()
{
    GLuint id;

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
    std::vector<std::string> face = {
        "../textures/right.jpg",
        "../textures/left.jpg",
        "../textures/top.jpg",
        "../textures/bottom.jpg",
        "../textures/back.jpg",
        "../textures/front.jpg"};
    for (int i = 0; i < face.size(); ++i)
    {
        try
        {
            int width, height, Channel;
            unsigned char *data = stbi_load(face[i].c_str(), &width, &height, &Channel, 0);
            if (data)
            {
                GLenum fomat;
                if (Channel == 1)
                    fomat = GL_RED;
                else if (Channel == 3)
                    fomat = GL_RGB;
                else if (Channel == 4)
                    fomat = GL_RGBA;

                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
            }
            else
            {
                std::cout << "Failed to load texture" << std::endl;
            }
            stbi_image_free(data);
        }
        catch (std::exception &e)
        {
            std::cout << e.what() << std::endl;
        }
    }
    std::cout << "here" << std::endl;
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    return id;
}

GLuint loadBMP_custom(const char *imagepath)
{

    printf("Reading image %s\n", imagepath);
    unsigned char header[54];
    unsigned int dataPos;
    unsigned int imageSize;
    unsigned int width, height;
    unsigned char *data;
    FILE *file = fopen(imagepath, "rb");
    if (!file)
    {
        printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath);
        getchar();
        return 0;
    }
    if (fread(header, 1, 54, file) != 54)
    {
        printf("Not a correct BMP file\n");
        fclose(file);
        return 0;
    }
    if (header[0] != 'B' || header[1] != 'M')
    {
        printf("Not a correct BMP file\n");
        fclose(file);
        return 0;
    }
    if (*(int *)&(header[0x1E]) != 0)
    {
        printf("Not a correct BMP file\n");
        fclose(file);
        return 0;
    }
    if (*(int *)&(header[0x1C]) != 24)
    {
        printf("Not a correct BMP file\n");
        fclose(file);
        return 0;
    }
    dataPos = *(int *)&(header[0x0A]);
    imageSize = *(int *)&(header[0x22]);
    width = *(int *)&(header[0x12]);
    height = *(int *)&(header[0x16]);
    if (imageSize == 0)
        imageSize = width * height * 3;
    if (dataPos == 0)
        dataPos = 54;
    data = new unsigned char[imageSize];
    fread(data, 1, imageSize, file);
    fclose(file);
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    delete[] data;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    return textureID;
}
unsigned char *loadBMP(const char *imagepath, unsigned int *width, unsigned int *height)
{

    printf("Reading image %s\n", imagepath);
    unsigned char header[54];
    unsigned int dataPos;
    unsigned int imageSize;

    unsigned char *data;
    FILE *file = fopen(imagepath, "rb");
    if (!file)
    {
        printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath);
        getchar();
        return 0;
    }
    if (fread(header, 1, 54, file) != 54)
    {
        printf("Not a correct BMP file\n");
        fclose(file);
        return 0;
    }
    if (header[0] != 'B' || header[1] != 'M')
    {
        printf("Not a correct BMP file\n");
        fclose(file);
        return 0;
    }
    if (*(int *)&(header[0x1E]) != 0)
    {
        printf("Not a correct BMP file\n");
        fclose(file);
        return 0;
    }
    if (*(int *)&(header[0x1C]) != 24)
    {
        printf("Not a correct BMP file\n");
        fclose(file);
        return 0;
    }
    dataPos = *(int *)&(header[0x0A]);
    imageSize = *(int *)&(header[0x22]);
    *width = *(int *)&(header[0x12]);
    *height = *(int *)&(header[0x16]);
    if (imageSize == 0)
        imageSize = (*width) * (*height) * 3;
    if (dataPos == 0)
        dataPos = 54;
    data = new unsigned char[imageSize];
    fread(data, 1, imageSize, file);
    fclose(file);

    return data;
}
Texture *LoadHDRimage(const char *path)
{
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrComponnets;
    float *data = stbi_loadf(path, &width, &height, &nrComponnets, 0);
    GLuint HDRtexture;
    Texture *temp = new Texture();
    if (data)
    {
        glGenTextures(1, &HDRtexture);
        glBindTexture(GL_TEXTURE_2D, HDRtexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
        temp->height = height;
        temp->width = width;
        temp->path = (char *)path;
        temp->id = HDRtexture;
        return temp;
    }

    std::cout << "FAILED TO LOAD HDR TEXTURE" << std::endl;
    return nullptr;
}