#version 330 core 
in vec3 Fragpos;
out vec4 FragColor;

uniform samplerCube Map;
const float PI = 3.14159265359f;

void main(){
    vec3 n =normalize(Fragpos);
    vec3 irradiance = vec3(0.0);

    vec3 up =vec3(0.0,1.0,0.0);
    vec3 right =normalize(cross(up,n));
    up= normalize(cross(n,right));

    float sampleDelta = 0.025;
    float nrSamples = 0.0; 
    for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
    {
        for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
        {
        // spherical to cartesian (in tangent space)
        vec3 tangentSample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
        // tangent space to world
        vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * n; 

        irradiance += texture(Map, sampleVec).rgb * cos(theta) * sin(theta);
        nrSamples++;
        }
    }
    irradiance = PI * irradiance * (1.0 / float(nrSamples));
    FragColor = vec4(irradiance,1.0);
}