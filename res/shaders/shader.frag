#version 450 core

layout(location = 0) out vec4 colour;

in vec2 vTexCoords;
// in float vTexIndex;

uniform vec4 uColour;
uniform sampler2D uTexture;
// uniform sampler2D uTextures[2];

void main() {
    // int index = int(vTexIndex);
    vec4 texColor = texture(uTexture, vTexCoords);
    // vec4 texColor = texture(uTextures[index], vTexCoords);
    colour = texColor;
}
