#version 450 core

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec2 a_TexCoords;
// layout(location = 2) in float a_TexIndex;

out vec2 vTexCoords;
// out float vTexIndex;

uniform mat4 u_MVP; //u_ModelViewProjectionMatrix

void main() {
    gl_Position = u_MVP * a_Position;
    vTexCoords = a_TexCoords;
    // vTexIndex = a_TexIndex;
}
