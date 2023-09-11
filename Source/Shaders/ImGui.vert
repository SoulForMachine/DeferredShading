#version 440

layout(binding = 0) uniform Xform
{
	mat4 projMatrix;
};

layout(location = 0) in vec2 inVertPosition;
layout(location = 1) in vec2 inVertTexcoords;
layout(location = 2) in vec4 inVertColor;

layout(location = 0) out vec2 outTexcoords;
layout(location = 1) out vec4 outColor;


out gl_PerVertex
{
	vec4 gl_Position;
};


void main()
{
	gl_Position = projMatrix * vec4(inVertPosition, 0.0, 1.0);
	outTexcoords = inVertTexcoords;
	outColor = inVertColor;
}
