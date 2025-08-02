#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoords;

out vec2 vTexCoords;

uniform mat4 u_MVP; //u_ModelViewProjectionMatrix

void main() {
	gl_Position = u_MVP * position;
	vTexCoords = texCoords;
}
