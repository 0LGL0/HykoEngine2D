#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTextureCoord;
layout(location = 2) in vec3 aColor;
layout(location = 3) in float aTextureIDIndx;

uniform mat4 VP;

out vec3 color;
out vec2 textureCoord;
flat out float textureIDIndx;

void main(){
	gl_Position = VP * vec4(aPos, 1.0f);

	color = aColor;
	textureCoord = aTextureCoord;
	textureIDIndx = aTextureIDIndx;
}
