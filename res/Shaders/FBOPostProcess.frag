#version 450 core

uniform sampler2D FBO_RGB;

in vec2 texCoord;
out vec4 outColor;

void main() {
	outColor = texture(FBO_RGB, texCoord);
}