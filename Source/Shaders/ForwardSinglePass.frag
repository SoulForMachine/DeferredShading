#version 440

layout(binding = 0) uniform sampler2D diffuseTex;
layout(binding = 1) uniform sampler2D normalTex;
layout(binding = 2) uniform isamplerBuffer lightIndices;
layout(binding = 3) uniform samplerBuffer lightPalette;

layout(location = 0) in vec3 inWorldPosition;
layout(location = 1) in vec3 inWorldNormal;
layout(location = 2) in vec3 inWorldTangent;
layout(location = 3) in vec3 inWorldBitangent;
layout(location = 4) in vec2 inTexcoords;

layout(location = 0) out vec4 fragColor;

layout(binding = 1) uniform LightData
{
	int numLights;
};

void main()
{
	vec4 diffuseColor = texture(diffuseTex, inTexcoords);
	vec3 tsTexNormal = normalize(texture(normalTex, inTexcoords).xyz * 2.0 - 1.0);
	mat3 ws2TsMat = mat3(normalize(inWorldTangent), normalize(inWorldBitangent), normalize(inWorldNormal));

	vec3 lightColor = vec3(0.0, 0.0, 0.0);

	for (int i = 0; i < numLights; ++i)
	{
		int lightIndex = texelFetch(lightIndices, i).r;
		vec4 lightPosRadius = texelFetch(lightPalette, lightIndex * 2 + 0);
		vec4 lightColorAndFalloffExp = texelFetch(lightPalette, lightIndex * 2 + 1);

		vec3 lightVec = lightPosRadius.xyz - inWorldPosition;
	
		vec3 tsLightVec = normalize(lightVec) * ws2TsMat;

		float distance = length(lightVec);
		float falloff = pow(max(1.0 - distance / lightPosRadius.w, 0.0), lightColorAndFalloffExp.a);
		float intensity = max(dot(tsTexNormal, tsLightVec), 0.0);

		lightColor += lightColorAndFalloffExp.rgb * intensity * falloff;
	}

	fragColor = vec4(lightColor * diffuseColor.rgb, 1.0);
}
