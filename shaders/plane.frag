#version 330 core 
in vec2 UV;
in vec3 Fragpos;
in vec3 Normal;
out vec4 color;
const float PI = 3.14159265359f;
uniform sampler2D roughmap;
uniform sampler2D albedomap;
uniform sampler2D aomap;
uniform sampler2D normalMap;
uniform sampler2D metmap;
uniform samplerCube Map;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;
uniform vec3 lightpos[3];
uniform vec3 lightcolor[3];
uniform vec3 Campos;
uniform int flag;


vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(normalMap, UV).rgb * 2.0 - 1.0;

    vec3 Q1  = dFdx(Fragpos);
    vec3 Q2  = dFdy(Fragpos);
    vec2 st1 = dFdx(UV);
    vec2 st2 = dFdy(UV);

    vec3 N   = normalize(Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}
vec3 fresnelschlick(float cost,vec3 f0){
    return f0 +(1.0-f0)*pow(clamp(1.0-cost,0.0,1.0),5.0);
}
float DistributionGGX(vec3 N,vec3 H,float roughness){
    float a = roughness*roughness;
    float a2=a*a;
    float NdotH = max(dot(N,H),0.0);
    float NdotH2=NdotH*NdotH;
    float num = a2;
    float denom = (NdotH2*(a2-1.0)+1.0);
    denom = PI*denom*denom;
    return num/denom;
}
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}   

void main(){
    float lightpower = 5.0f;
    
    vec3 albedo = pow(texture(albedomap,UV).rgb,vec3(2.2));
    float roughness = texture(roughmap,UV).r;
    float ao = texture(aomap,UV).r;
    float met = texture(metmap,UV).r;
    vec3 n = getNormalFromMap();
    //vec3 n =normalize(Normal);
    vec3 v = normalize(Campos-Fragpos);
    
    vec3 f0 = vec3(0.04);
    f0 = mix(f0,albedo,met);
    vec3 Lo = vec3(0.0);
    for(int i=0;i<3;++i){
        vec3 l = normalize(lightpos[i]-Fragpos);
        vec3 h = normalize(v+l);
        float distance = length(lightpos[i]-Fragpos);
        float attenuation = 1.0/(distance*distance);
        vec3 radians = lightcolor[i]*attenuation;

        float NDF = DistributionGGX(n,h,roughness);
        float G =GeometrySmith(n,v,l,roughness);
        vec3 F = fresnelschlick(max(dot(h,v),0.0),f0);

        vec3 ks = F;
        vec3 kd = vec3(1.0)-ks;
        kd*=1.0-met;

        vec3 nominator = NDF*G*F;
        float denominator = 4.0*max(dot(n,v),0.0)*max(dot(n,l),0.0)+0.0001;
        vec3 specular = nominator/denominator;
        float NdotL = max(dot(n,l),0.0);
        Lo+=(kd*albedo/PI+specular)*radians*NdotL;
    }
    vec3 kS = fresnelSchlickRoughness(max(dot(n, v), 0.0), f0, roughness);
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - met;	
    vec3 r = reflect(-v,n);
    vec3 irradiance = texture(Map, n).rgb;
    vec3 diffuse      = irradiance * albedo;
    vec3 without =  vec3(0.03) * albedo * ao;
    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(prefilterMap, r,  roughness * MAX_REFLECTION_LOD).rgb;    
    vec2 brdf  = texture(brdfLUT, vec2(max(dot(n, v), 0.0), roughness)).rg;
    vec3 specular = prefilteredColor * (kS * brdf.x + brdf.y);
    vec3 ambient =(flag==0)? (kD * diffuse+specular) * ao:without;

    vec3 res = ambient+Lo;
    //res *=lightpower;
    res = res/(res+vec3(1.0));
    res = pow(res,vec3(1.0/2.2));
    color=vec4(res,1.0);
}