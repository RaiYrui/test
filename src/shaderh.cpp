#include <shaderh.h>
Shader::Shader(const char *vpath, const char *fpath)
{
    std::string vertexcode;
    std::string fragmentcode;
    std::ifstream vfile;
    std::ifstream ffile;
    vfile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    ffile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        vfile.open(vpath);
        ffile.open(fpath);
        std::stringstream vstream, fstream;
        vstream << vfile.rdbuf();
        fstream << ffile.rdbuf();
        vfile.close();
        ffile.close();
        vertexcode = vstream.str();
        fragmentcode = fstream.str();
    }
    catch (std::fstream::failure &e)
    {
        std::cout << "error::Cannot read the shader files " << e.what() << std::endl;
    }
    const char *vcode = vertexcode.c_str();
    const char *fcode = fragmentcode.c_str();
    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[1024];
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vcode, nullptr);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 1024, NULL, infoLog);
        std::cout << "VERTEX SHADER COMPILE ERROR " << infoLog << std::endl;
    }
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fcode, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 1024, NULL, infoLog);
        std::cout << "FRAGMENT SHADER COMPILE ERROR " << infoLog << std::endl;
    }
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID, 1024, NULL, infoLog);
        std::cout << "FRAGMENT SHADER COMPILE ERROR " << infoLog << std::endl;
    }
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}
Shader::Shader(const char *vpath, const char *fpath, const char *gpath)
{
    std::string vertexcode;
    std::string fragmentcode;
    std::string geometrycode;
    std::ifstream vfile;
    std::ifstream ffile;
    std::ifstream gfile;
    vfile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    ffile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    gfile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        vfile.open(vpath);
        ffile.open(fpath);
        gfile.open(gpath);
        std::stringstream vstream, fstream, gstream;
        vstream << vfile.rdbuf();
        fstream << ffile.rdbuf();
        gstream << gfile.rdbuf();
        vfile.close();
        ffile.close();
        gfile.close();
        vertexcode = vstream.str();
        fragmentcode = fstream.str();
        geometrycode = gstream.str();
    }
    catch (std::fstream::failure &e)
    {
        std::cout << "error::Cannot read the shader files " << e.what() << std::endl;
    }
    const char *vcode = vertexcode.c_str();
    const char *fcode = fragmentcode.c_str();
    const char *gcode = geometrycode.c_str();
    GLuint vertex, fragment, geometry;
    GLint success;
    GLchar infoLog[1024];
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vcode, nullptr);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 1024, NULL, infoLog);
        std::cout << "VERTEX SHADER COMPILE ERROR " << infoLog << std::endl;
    }
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fcode, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 1024, NULL, infoLog);
        std::cout << "FRAGMENT SHADER COMPILE ERROR " << infoLog << std::endl;
    }
    geometry = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometry, 1, &gcode, nullptr);
    glCompileShader(geometry);
    glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(geometry, 1024, NULL, infoLog);
        std::cout << "FRAGMENT SHADER COMPILE ERROR " << infoLog << std::endl;
    }
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glAttachShader(ID, geometry);
    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID, 1024, NULL, infoLog);
        std::cout << "FRAGMENT SHADER COMPILE ERROR " << infoLog << std::endl;
    }
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    glDeleteShader(geometry);
}

void Shader::use() const
{
    glUseProgram(ID);
}
void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setMat4(const std::string &name, glm::mat4 &value) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}
void Shader::setvec3(const std::string &name, glm::vec3 value) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z);
}