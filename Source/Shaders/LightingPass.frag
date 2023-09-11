#version 440

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

layout(binding = 0) uniform sampler2D diffuseTex;
layout(binding = 1) uniform sampler2D positionTex;
layout(binding = 2) uniform sampler2D normalTex;

layout(location = 0) out vec4 fragColor;

void main()
{
	vec2 uv = (gl_FragCoord.xy - viewport.xy) / viewport.zw;
	vec4 diffuseColor = texture(diffuseTex, uv);
	vec3 position = texture(positionTex, uv).xyz;
	vec3 normal = normalize(texture(normalTex, uv).xyz);

	vec3 lightVec = lightPositionRadius.xyz - position;
	float distance = length(lightVec);
	float falloff = pow(max(1.0 - distance / lightPositionRadius.w, 0.0), lightColorFalloffExp.a);
	float intensity = dot(normal, normalize(lightVec));

	fragColor = vec4(lightColorFalloffExp.rgb * diffuseColor.rgb * intensity * falloff, 1.0);
}
