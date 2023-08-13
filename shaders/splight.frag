#version 330 core
in vec2 UV;
in vec3 Normal;
in vec3 posworld;
uniform vec3 lightpos;
uniform sampler2D diff;
uniform sampler2D shadowMap;
uniform vec3 Campos;
uniform int flag;
uniform mat4 lightspace;
out vec4 color;
float ShadowCalculation(vec4 fragPosLightSpace,float bias)
{
    // 执行透视除法
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // 变换到[0,1]的范围
    projCoords = projCoords * 0.5 + 0.5;
    // 取得最近点的深度(使用[0,1]范围下的fragPosLight当坐标)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // 取得当前片元在光源视角下的深度
    float currentDepth = projCoords.z;
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    if(projCoords.z>1.0)
        shadow=0.0;
    return shadow;
}
void main(){
    vec4 fragPosLightSpace = lightspace * vec4(posworld,1.0f);
    float lightpower=20.0;
    float specularStrength = 0.5f;
    vec3 lightColor = vec3(1,1,1);
    vec3 n = normalize(Normal);
    vec3 l = normalize(lightpos-posworld);
    float bias = max(0.05*(1.0-dot(n,l)),0.005);
    vec3 diffusecolor = texture(diff,UV).rgb;
    float cost = clamp(dot(n,l),1,0);
    float distance = length(lightpos-posworld);
    vec3 E = normalize(Campos-posworld);
    vec3 refl= reflect(-l,n);
    vec3 v = normalize(Campos- posworld);
    vec3 halfway = normalize(l+v);
    float spec = pow(clamp((flag==0?dot(n,halfway):dot(E,refl)),1,0),32);
    vec3 ambientcolor = vec3(0.3,0.3,0.3)*diffusecolor;
    float shadow = ShadowCalculation(fragPosLightSpace,bias);       
    color.rgb=
    ambientcolor+
    diffusecolor*lightColor*lightpower*cost/(distance*distance)+
    spec*lightColor*specularStrength;
    color.a=1;

}