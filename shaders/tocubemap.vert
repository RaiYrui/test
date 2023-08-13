#version 330 core 
layout(location = 0)in vec3 position;

out vec3 Fragpos;

uniform mat4 view;
uniform mat4 projection;

void main(){
    Fragpos = position;
    gl_Position = projection*view*vec4(Fragpos,1.0);
}