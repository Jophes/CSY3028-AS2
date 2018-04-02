#version 440  
layout (location = 0) in  vec3 VertexPosition;
layout (location = 1) in  vec3 VertexNormal;
layout (location = 2) in  vec2 VertexTexCoord;

out vec3 Position;
out vec3 Normal;
out vec2 TexCoord;

uniform mat4 MV;
uniform mat4 MVP;

void main() {
    Position = vec3(MV * vec4(VertexPosition, 1.0));
    Normal = normalize(mat3(MV) * VertexNormal);
    TexCoord = VertexTexCoord;
    gl_Position = MVP * vec4(VertexPosition, 1.0);
}
