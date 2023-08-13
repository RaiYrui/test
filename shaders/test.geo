#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec2 texCoords;
    vec3 Normal;
    vec3 posworld;
} gs_in[];

out vec2 UV;
out vec3 Normal;
out vec3 posworld;

uniform float time;
vec3 getnormal(){
    vec3 a = vec3(gl_in[0].gl_Position)-vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position)-vec3(gl_in[1].gl_Position);
    return normalize(cross(a,b));
}
vec4 explode(vec4 pos,vec3 normal){
    float magnitude = 2.0f;
    vec3 direction = normal * ((sin(time) + 1.0f) / 2.0f) * magnitude;
    return pos + vec4(direction, 0.0f);
}
void main(){
    
    vec3 n = getnormal();
    //gl_Position = explode(gl_in[0].gl_Position, n);
    gl_Position = gl_in[0].gl_Position;
    UV = gs_in[0].texCoords;
    Normal = gs_in[0].Normal;
    posworld = gs_in[0].posworld;
    EmitVertex();
    // gl_Position = explode(gl_in[1].gl_Position, n);
    gl_Position = gl_in[1].gl_Position;
    UV = gs_in[1].texCoords;
    Normal = gs_in[1].Normal;
    posworld = gs_in[1].posworld;
    EmitVertex();
    //gl_Position = explode(gl_in[2].gl_Position, n);
     gl_Position = gl_in[2].gl_Position;
    UV = gs_in[2].texCoords;
    Normal = gs_in[2].Normal;
    posworld = gs_in[2].posworld;
    EmitVertex();
    EndPrimitive();
}