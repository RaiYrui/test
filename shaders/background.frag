#version 330 core 
out vec4 FragColor;

in vec3 Fragpos;

uniform samplerCube Map;

void main(){
    vec3 envColor = texture(Map,Fragpos).rgb;

    envColor = envColor/(envColor+vec3(1.0));
    envColor = pow(envColor,vec3(1.0/2.2));
    FragColor = vec4(envColor,1.0);
}