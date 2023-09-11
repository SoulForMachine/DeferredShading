#version 440

layout(binding = 0) uniform sampler2D diffuseTex;
layout(binding = 1) uniform sampler2D normalTex;

layout(location = 0) in vec3 inWorldPosition;
layout(location = 1) in vec3 inWorldNormal;
layout(location = 2) in vec3 inWorldTangent;
layout(location = 3) in vec3 inWorldBitangent;
layout(location = 4) in vec2 inTexcoords;

layout(location = 0) out vec4 fragColor;

layout(binding = 1) uniform LightData
{
	vec4 lightPosRadius;
	vec4 lightColorAndFalloffExp;
};

void main()
{
	vec4 diffuseColor = texture(diffuseTex, inTexcoords);
	vec3 tsTexNormal = normalize(texture(normalTex, inTexcoords).xyz * 2.0 - 1.0);

	vec3 lightVec = lightPosRadius.xyz - inWorldPosition;
	
	mat3 mat = mat3(normalize(inWorldTangent), normalize(inWorldBitangent), normalize(inWorldNormal));
	vec3 tsLightVec = normalize(lightVec) * mat;

	float distance = length(lightVec);
	float falloff = pow(max(1.0 - distance / lightPosRadius.w, 0.0), lightColorAndFalloffExp.a);
	float intensity = dot(tsTexNormal, tsLightVec);

	fragColor = vec4(lightColorAndFalloffExp.rgb * diffuseColor.rgb * intensity * falloff, 1.0);
}
