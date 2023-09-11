#version 440

layout(binding = 0) uniform sampler2D diffuseTex;
layout(binding = 1) uniform sampler2D normalTex;

layout(location = 0) in vec3 inWorldPosition;
layout(location = 1) in vec3 inWorldNormal;
layout(location = 2) in vec3 inWorldTangent;
layout(location = 3) in vec3 inWorldBitangent;
layout(location = 4) in vec2 inTexcoords;

layout(location = 0) out vec4 fragPosition;
layout(location = 1) out vec4 fragNormal;
layout(location = 2) out vec4 fragColor;

void main()
{
	vec3 tsTexNormal = normalize(texture(normalTex, inTexcoords).xyz * 2.0 - 1.0);
	vec3 wsTexNormal = mat3(normalize(inWorldTangent), normalize(inWorldBitangent), normalize(inWorldNormal)) * tsTexNormal;

	fragPosition = vec4(inWorldPosition, 0.0);
	fragNormal = vec4(wsTexNormal, 0.0);
	fragColor = texture(diffuseTex, inTexcoords);
}
