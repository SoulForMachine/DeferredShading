#version 440

layout(location = 0) in vec3 inVertPosition;

layout(binding = 0) uniform SceneXforms
{
	mat4 viewProjMatrix;
	vec4 viewport;
};

layout(binding = 1) uniform LightData
{
	vec4 lightPositionRadius;
	vec4 lightColorFalloffExp;
};

out gl_PerVertex
{
	vec4 gl_Position;
};

void main()
{
	gl_Position = viewProjMatrix * vec4(inVertPosition * lightPositionRadius.w + lightPositionRadius.xyz, 1.0f);
}
