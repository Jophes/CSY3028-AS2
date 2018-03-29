#version 440  
layout (location = 0) in  vec3 VertexPosition;
layout (location = 1) in  vec3 VertexNormal;
layout (location = 2) in  vec2 VertexTexCoord;
out vec4 P;
out vec3 N;
out vec2 TexCoord;

uniform mat4 MV;
uniform mat4 MVP;

void main() {
    N = normalize(mat3(MV) * VertexNormal);
    P = MV * vec4(VertexPosition, 1.0);
    TexCoord = VertexTexCoord;
    gl_Position = MVP * vec4(VertexPosition, 1.0);
}
