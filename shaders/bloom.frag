#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform vec3 lightpos;
uniform sampler2D diff;
uniform vec3 viewPos;

void main()
{           
    vec3 lightcolor = vec3(5.0,5.0,5.0);
    vec3 color = texture(diff, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);
    // ambient
    vec3 ambient = 0.0 * color;
    // lighting
    vec3 lighting = vec3(0.0);
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);

        // diffuse
        vec3 lightDir = normalize(lightpos - fs_in.FragPos);
        float diff = max(dot(lightDir, normal), 0.0);
        vec3 result = lightcolor * diff * color;      
        // attenuation (use quadratic as we have gamma correction)
        float distance = length(fs_in.FragPos - lightpos);
        result *= 1.0 / (distance * distance);
        lighting += result;
     result = ambient + lighting;
    // check whether result is higher than some threshold, if so, output as bloom threshold color
    float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(result, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
    FragColor = vec4(result, 1.0);
}