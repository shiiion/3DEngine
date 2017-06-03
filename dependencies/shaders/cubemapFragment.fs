#version 330 core
precision highp float;

in vec3 texCoords;
out vec4 color;

uniform samplerCube skybox;

void main()
{    
    //color = texture(skybox, TexCoords) * vec4(1.4f, 1.4f, 1.4f, 1.0f);
	//color = texture(skybox, TexCoords) * vec4(1.3f, 1.3f, 1.3f, 1.0f);
	//color = texture(skybox, TexCoords) * vec4(1.2f, 1.2f, 1.2f, 1.0f);
	//color = texture(skybox, TexCoords) * vec4(1.1f, 1.1f, 1.1f, 1.0f);
	color = texture(skybox, texCoords);
}