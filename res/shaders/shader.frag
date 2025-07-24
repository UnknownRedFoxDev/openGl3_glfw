#version 330 core

layout(location = 0) out vec4 colour;

in vec2 vTexCoords;

uniform vec4 uColour;
uniform sampler2D uTexture;

void main() {
	vec4 texColor = texture(uTexture, vTexCoords);
	// colour = uColour;
	colour = texColor;
}
