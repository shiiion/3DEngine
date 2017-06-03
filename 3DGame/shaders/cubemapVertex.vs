#version 330 core
precision highp float;

layout (location = 0) in vec3 position;
out vec3 texCoords;

uniform mat4 transform;

void main()
{
    vec4 pos = transform * vec4(position, 1.0);  
	gl_Position = pos.xyww;  
    texCoords = position;
} 

/*
Therefore cubemap will always be before other objects.
Depth value is z component -> maximum depth value is 1 -> z/w -> w/w = 1
Noramlized device coordinates
Use of GL_LEQUAL considers case of when cubemap intersects with object -> object always will render in front
*/