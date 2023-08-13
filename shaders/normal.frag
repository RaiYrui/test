#version 330 core

in vec2 UV;
in vec3 Fragpos;
in mat3 TBN;


uniform sampler2D diff;
uniform sampler2D normal;
uniform vec3 lightpos;
uniform vec3 Campos;

out vec4 color;
void main(){
    float specularStrength = 0.1f;
    vec3 lightColor = vec3(1,1,1);
    float lightpower = 10.0;
    vec3 diffuse = texture(diff,UV).rgb;
    vec3 n = texture(normal,UV).rgb;
    n= normalize(n*2.0-1.0);
    n=normalize(TBN*n); 
    float distance = length(lightpos-Fragpos);
    vec3 l = normalize(lightpos-Fragpos);
    float cost = clamp(dot(n,l),1,0);
    vec3 E = normalize(Campos - Fragpos);
    vec3 refl = reflect(-l,n);
    float spec = pow(clamp(dot(E,refl),1,0),32);
    vec3 ambient = diffuse * vec3(0.3,0.3,0.3);
    color.rgb=
    ambient+
    diffuse*lightColor*lightpower*cost/(distance*distance)+
    spec*lightColor*specularStrength;
    color.a=1;
}