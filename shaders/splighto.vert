#version 330 core
layout(location = 0 )in vec3 position;
layout(location = 1)in vec2 uv;
layout(location = 2)in vec3 normal;



out vec2 UV;
out vec3 Normal;
out vec3 posworld;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightpos;
void main(){
    gl_Position = projection * view * model * vec4(position, 1.0f);
    Normal = transpose(inverse(mat3(model))) * normal;
    posworld = (model*vec4(position,1.0f)).xyz;
    UV=uv;
}
