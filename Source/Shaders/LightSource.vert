#version 440

layout(location = 0) in vec3 inVertPosition;

layout(location = 0) flat out vec4 outLightColor;

layout(binding = 0) uniform samplerBuffer lightPalette;

layout(binding = 0) uniform SceneXforms
{
	mat4 viewProjMatrix;
	vec4 viewport;
};

out gl_PerVertex
{
	vec4 gl_Position;
};

const float scale = 7.0;

void main()
{
	vec4 lightPosRadius = texelFetch(lightPalette, gl_InstanceID * 2 + 0);
	outLightColor = texelFetch(lightPalette, gl_InstanceID * 2 + 1);

	gl_Position = viewProjMatrix * vec4(inVertPosition * scale + lightPosRadius.xyz, 1.0f);
}
