#version 330 core
layout(location = 0)in vec3 pos;
layout(location = 1)in vec3 normal;

out vec3 Normal;
out vec3 Position;

uniform mat4 M;
uniform mat4 V;
uniform mat4 MVP;

void main(){

    vec4 positon = MVP* vec4(pos.x,pos.y,pos.z,1.0);
    Normal= mat3(transpose(inverse(M)))*normal;
    Position = vec3(M*vec4(pos.x,pos.y,pos.z,1.0));
    gl_Position = positon.xyzw;
}