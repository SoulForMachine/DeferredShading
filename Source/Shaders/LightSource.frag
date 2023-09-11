#version 440

layout(location = 0) flat in vec4 inLightColor;

layout(location = 0) out vec4 fragColor;


void main()
{
	fragColor = vec4(inLightColor.rgb, 1.0);
}
