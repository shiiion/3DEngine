#version 330 
precision highp float;

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texture;

out vec3 worldPos;
out vec2 texCoord;

uniform mat4 transform;
uniform mat4 model;

void main()
{
	texCoord = texture;
	worldPos = (model * vec4(position, 1.0)).xyz;
	gl_Position = transform * vec4(position, 1.0f);
}