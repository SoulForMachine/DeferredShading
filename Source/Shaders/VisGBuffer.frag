#version 440

layout(location = 0) in vec2 inUV;

layout(location = 0) out vec4 fragColor;

layout(binding = 0) uniform sampler2D TexSampler;

layout(binding = 1) uniform TexViewData
{
	vec4 rangeMin;
	vec4 rangeMax;
	int texComponents;
};


void main()
{
	vec3 tex = texture(TexSampler, inUV).rgb;

	if (texComponents == 1)
	{
		tex = tex.rrr;
	}

	fragColor = vec4((tex - rangeMin.xyz) / (rangeMax.xyz - rangeMin.xyz), 1.0);
}
