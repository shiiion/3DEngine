#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D screenTexture;

const float offset = 1.0 / 300;  

void main()
{
    vec2 offsets[9] = vec2[](
        vec2(-offset, offset),  // top-left
        vec2(0.0f,    offset),  // top-center
        vec2(offset,  offset),  // top-right
        vec2(-offset, 0.0f),    // center-left
        vec2(0.0f,    0.0f),    // center-center
        vec2(offset,  0.0f),    // center-right
        vec2(-offset, -offset), // bottom-left
        vec2(0.0f,    -offset), // bottom-center
        vec2(offset,  -offset)  // bottom-right    
    );

    float kernel[9] = float[](
		 //-1, -1, -1, -1,  9, -1, -1, -1, -1 //Narcotic Effects
		 1, 1, 1, 1, -8, 1, 1, 1, 1 //Edge Detection
		 //-2, -1, 0, -1, 1, 1, 0, 1, 2 //Emboss
		//1.0 / 16, 2.0 / 16, 1.0 / 16, 2.0 / 16, 4.0 / 16, 2.0 / 16, 1.0 / 16, 2.0 / 16, 1.0 / 16   //Blur
	//0.5997023498159715,0.34553243048391263,-0.2708298674538042,0,47.43192855600873,-0.037703249837783157,0.8609577587992641,0.15059552388459913,0,-36.96841498319127,0.24113635128153335,-0.07441037908422492,0.44972182064877153,0,-7.562075277591283,0,0,0,1,0 //vintage
    );
    
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];
    
    color = vec4(col, 1.0);

	color = texture(screenTexture, TexCoords);
}  