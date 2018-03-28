#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexColour;

out vec4 Pos;
out vec3 Colour;

uniform mat4 MV;
uniform mat4 MVP;

void main() {
    Pos = MV * vec4(VertexPosition, 1.0);
    Colour = VertexColour;
    gl_Position = MVP * vec4(VertexPosition, 1.0);
}
