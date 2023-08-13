#version 330 core 
layout(location = 0)in vec3 position;
uniform mat4 view;
uniform mat4 projection;

out vec3 Fragpos;

void main(){
    Fragpos=position;
    mat4 rotview = mat4(mat3(view));
    vec4 pos = projection*rotview*vec4(position,1.0);
    gl_Position = pos.xyww;
}