#version 440

layout(binding = 0) uniform SceneXforms
{
	mat4 viewProjMatrix;
	vec4 viewport;
};

layout(location = 0) in vec3 inVertPosition;


out gl_PerVertex
{
	vec4 gl_Position;
};


void main()
{
	gl_Position = viewProjMatrix * vec4(inVertPosition, 1.0);
}
