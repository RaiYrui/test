#version 330 core
in vec3 Normal;
in vec3 Position;

out vec4 FragColor;
uniform samplerCube tex;
uniform vec3 campos;

void main(){
    float ratio = 1.00 / 1.52;
    vec3 I =normalize(Position- campos);
    //vec3 R = reflect(I,normalize(Normal));
    vec3 R = refract(I, normalize(Normal), ratio);
    FragColor = texture(tex,R);
}