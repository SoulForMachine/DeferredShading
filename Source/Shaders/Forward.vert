#version 440

layout(binding = 0) uniform SceneXforms
{
	mat4 viewProjMatrix;
	vec4 viewport;
};

layout(location = 0) in vec3 inVertPosition;
layout(location = 1) in vec3 inVertNormal;
layout(location = 2) in vec3 inVertTangent;
layout(location = 3) in vec2 inVertTexcoords;

layout(location = 0) out vec3 outWorldPosition;
layout(location = 1) out vec3 outWorldNormal;
layout(location = 2) out vec3 outWorldTangent;
layout(location = 3) out vec3 outWorldBitangent;
layout(location = 4) out vec2 outTexcoords;


out gl_PerVertex
{
	vec4 gl_Position;
};


void main()
{
	gl_Position = viewProjMatrix * vec4(inVertPosition, 1.0);
	outWorldPosition = inVertPosition;
	outWorldNormal = inVertNormal;
	outWorldTangent = inVertTangent;
	outWorldBitangent = cross(outWorldNormal, outWorldTangent);
	outTexcoords = inVertTexcoords;
}
