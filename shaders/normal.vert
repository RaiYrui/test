#version 330 core
layout(location = 0 )in vec3 position;
layout(location = 1)in vec2 uv;
layout(location = 2)in vec3 normal;
layout(location = 3)in vec3 tangent;
layout(location = 4)in vec3 bitangent;

out vec2 UV;
out vec3 Fragpos;
out mat3 TBN;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main(){
    gl_Position = projection * view * model * vec4(position, 1.0f);
    vec3 T = tangent *normalize(vec3(model*vec4(position,1.0f)));
    vec3 B = bitangent *normalize(vec3(model*vec4(position,1.0f)));
    vec3 N = normal *normalize(vec3(model*vec4(position,1.0f)));
    TBN = mat3(T,B,N);
    UV = uv;
    Fragpos = vec3(model*vec4(position,1.0f));
    
}