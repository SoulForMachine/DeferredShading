#version 440

layout(binding = 0) uniform sampler2D uiTexture;

layout(location = 0) in vec2 inTexcoords;
layout(location = 1) in vec4 inColor;

layout(location = 0) out vec4 fragColor;

void main()
{
	fragColor = inColor * texture(uiTexture, inTexcoords);
}
