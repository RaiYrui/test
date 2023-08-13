#include <obj.h>
bool OBJloader::LoadOBJ(const char *path,
                        std::vector<glm::vec3> &out_vertices,
                        std::vector<glm::vec2> &out_uvs,
                        std::vector<glm::vec3> &out_normals)
{
    printf("Loading OBJ file %s...\n", path);

    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec2> temp_uvs;
    std::vector<glm::vec3> temp_normals;

    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        printf("Impossible to open the file ! right path ? \n");
        getchar();
        return false;
    }
    vertex.clear();
    uv.clear();
    normal.clear();
    while (1)
    {

        char lineHeader[128];

        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break;
        // else : parse lineHeader

        if (strcmp(lineHeader, "v") == 0)
        {
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            temp_vertices.push_back(vertex);
        }
        else if (strcmp(lineHeader, "vt") == 0)
        {
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y);
            uv.y = -uv.y;
            temp_uvs.push_back(uv);
        }
        else if (strcmp(lineHeader, "vn") == 0)
        {
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            temp_normals.push_back(normal);
        }
        else if (strcmp(lineHeader, "f") == 0)
        {
            char buf[128];
            fscanf(file, "%s", buf);

            int v, n, t;
            if (strstr(buf, "//"))
            {

                sscanf(buf, "%d//%d", &v, &n);
                vertexIndices.push_back(v);
                normalIndices.push_back(n);
                fscanf(file, "%d//%d", &v, &n);
                vertexIndices.push_back(v);
                normalIndices.push_back(n);
                fscanf(file, "%d//%d", &v, &n);
                vertexIndices.push_back(v);
                normalIndices.push_back(n);
            }
            else if (sscanf(buf, "%d/%d/%d", &v, &t, &n) == 3)
            {

                vertexIndices.push_back(v);
                uvIndices.push_back(t);
                normalIndices.push_back(n);
                fscanf(file, "%d/%d/%d", &v, &t, &n);
                vertexIndices.push_back(v);
                uvIndices.push_back(t);
                normalIndices.push_back(n);
                fscanf(file, "%d/%d/%d", &v, &t, &n);
                vertexIndices.push_back(v);
                uvIndices.push_back(t);
                normalIndices.push_back(n);
            }
            else if (sscanf(buf, "%d/%d", &v, &t) == 2)
            {

                vertexIndices.push_back(v);
                uvIndices.push_back(t);
                fscanf(file, "%d/%d", &v, &t);
                vertexIndices.push_back(v);
                uvIndices.push_back(t);
                fscanf(file, "%d/%d", &v, &t);
                vertexIndices.push_back(v);
                uvIndices.push_back(t);
            }
            else
            {

                sscanf(buf, "%d", &v);
                vertexIndices.push_back(v);
                fscanf(file, "%d", &v);
                vertexIndices.push_back(v);
                fscanf(file, "%d", &v);
                vertexIndices.push_back(v);
            }
        }
        else
        {

            char stupidBuffer[1000];
            fgets(stupidBuffer, 1000, file);
        }
    }

    for (unsigned int i = 0; i < vertexIndices.size(); i++)
    {
        unsigned int vertexIndex = vertexIndices[i];
        glm::vec3 vertex = temp_vertices[vertexIndex - 1];
        out_vertices.push_back(vertex);

        glm::vec2 uv;
        if (temp_uvs.size())
        {
            unsigned int uvIndex = uvIndices[i];
            uv = temp_uvs[uvIndex - 1];
        }
        else
            // uv = glm::vec2(float(i)/vertexIndices.size(),1.0f-float(i)/vertexIndices.size());
            uv = glm::vec2(0, 0);
        out_uvs.push_back(uv);

        glm::vec3 normal;
        if (normalIndices.size())
        {
            unsigned int normalIndex = normalIndices[i];
            normal = temp_normals[normalIndex - 1];
        }
        else
            normal = glm::vec3(1, 1, 1);
        out_normals.push_back(normal);
    }

    fclose(file);
    std::cerr << "file loaded!\n";
    return true;
}
OBJloader::OBJloader(const char *path, int way, int mode)
{
    if (way == ASSIMPLOAD)
    {
        // if (!this->ASSIMPLoad(path, vertex, uv, normal))
        // {
        //     printf("FAILED TO LOAD OBJ\n");
        //     return;
        // }
    }
    else
    {
        if (!this->LoadOBJ(path, vertex, uv, normal))
        {
            printf("FAILED TO LOAD OBJ\n");
            return;
        }
    }
    if (mode == NORMALLOAD)
        this->setUpnormal();
    else
        this->setUp();
}
void OBJloader::setUp()
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(glm::vec3), &vertex[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, uv.size() * sizeof(glm::vec2), &uv[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glGenBuffers(1, &normalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, normal.size() * sizeof(glm::vec3), &normal[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void OBJloader::Draw(Shader shader)
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertex.size());
    glBindVertexArray(0);
}
void OBJloader::setUpnormal()
{
    std::vector<glm::vec3> tangent;
    std::vector<glm::vec3> bitangent;
    TangentBasis(vertex, uv, normal, tangent, bitangent);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(glm::vec3), &vertex[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, uv.size() * sizeof(glm::vec2), &uv[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glGenBuffers(1, &normalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, normal.size() * sizeof(glm::vec3), &normal[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glGenBuffers(1, &tangentbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, tangentbuffer);
    glBufferData(GL_ARRAY_BUFFER, tangent.size() * sizeof(glm::vec3), &tangent[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glGenBuffers(1, &bitangentbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, bitangentbuffer);
    glBufferData(GL_ARRAY_BUFFER, bitangent.size() * sizeof(glm::vec3), &bitangent[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void OBJloader::Changeobj(const char *path)
{

    if (!this->LoadOBJ(path, vertex, uv, normal))
    {
        printf("FAILED TO LOAD OBJ\n");
        return;
    }
    this->setUp();
}
// bool OBJloader::ASSIMPLoad(const char *path, std::vector<glm::vec3> &vertex, std::vector<glm::vec2> &uv, std::vector<glm::vec3> &noraml)
// {
//     Assimp::Importer import;
//     const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
//     if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
//     {
//         std::cout << "FILE OPEN ERROR" << std::endl;
//         return false;
//     }
//     for (int i = 0; i < scene->mNumMeshes; ++i)
//     {
//         aiMesh *aimesh = scene->mMeshes[i];
//         for (int j = 0; j < aimesh->mNumVertices; ++j)
//         {
//             glm::vec3 vvv;
//             vvv.x = aimesh->mVertices[j].x;
//             vvv.y = aimesh->mVertices[j].y;
//             vvv.z = aimesh->mVertices[j].z;
//             vertex.push_back(vvv);

//             vvv.x = aimesh->mNormals[j].x;
//             vvv.y = aimesh->mNormals[j].y;
//             vvv.z = aimesh->mNormals[j].z;
//             normal.push_back(vvv);

//             glm::vec2 vv(0, 0);
//             if (aimesh->mTextureCoords[0])
//             {
//                 vv.x = aimesh->mTextureCoords[0][j].x;
//                 vv.y = aimesh->mTextureCoords[0][j].y;
//             }
//             uv.push_back(vv);
//         }
//         delete aimesh;
//     }
//     delete scene;
//     scene = nullptr;
//     return true;
// }