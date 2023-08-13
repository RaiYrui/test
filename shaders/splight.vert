#version 330 core
layout(location = 0 )in vec3 position;
layout(location = 1)in vec2 uv;
layout(location = 2)in vec3 normal;


out VS_OUT{
vec2 texCoords;
vec3 Normal;
vec3 posworld;
}vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightpos;
void main(){
    gl_Position = projection * view * model * vec4(position, 1.0f);
    vs_out.Normal = transpose(inverse(mat3(model))) * normal;
    vs_out.posworld = (model*vec4(position,1.0f)).xyz;
    vs_out.texCoords=uv;
}
