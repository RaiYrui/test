#version 330 core

in vec2 TexCoords;
in vec3 Normal;
in vec3 Position;

out vec4 color;
uniform vec3 campos;
uniform int type;
uniform samplerCube texture_diffuse1;

void main()
{    
    float ratio = 1.00 / 1.52;
    vec3 I =normalize(Position- campos);
    vec3 Reflect = reflect(I,normalize(Normal));
    vec3 Refract = refract(I, normalize(Normal), ratio);
    float fresnel = 0.4+0.6*pow(clamp(0.0,dot(I,normalize(Normal)),1.0),4.0);
    vec4 Reflectcolor = texture(texture_diffuse1,Reflect);
    vec4 Refractcolor = texture(texture_diffuse1,Refract);
    color = (type==0)?Reflectcolor:((type==1)?Refractcolor:((type==2)?mix(Reflectcolor,Refractcolor,fresnel):Refractcolor));
    
}