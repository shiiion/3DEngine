#version 330 
precision highp float;

in vec3 worldPos;
in vec2 texCoord;

out vec4 fragColor;

uniform vec3 cameraPosition;
uniform sampler2D normalMap;
uniform samplerCube skybox;

void main()
{
	fragColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

	//vec3 Normal = vec3(0, texture(normalMap, texCoord).y, 0);
	//vec3 Normal = vec3(texture(normalMap, texCoord).x, 0, 0);
	//vec3 Normal = vec3(0, 0, texture(normalMap, texCoord).z);
	vec3 Normal = texture(normalMap, texCoord).xyz * 2.0f - 1.0f;
	vec3 Incident = normalize(worldPos - cameraPosition);
	vec3 R = refract(Incident, vec3(-Normal.x, -Normal.y, Normal.z), 1.0f/1.0f); //1/refractive index -> 1.0f/2.0f
	fragColor = texture(skybox, R);
	//fragColor = texture(normalMap, texCoord);
	//fragColor  = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
