
#ifndef _GLSLAYER_SAMPLER_STATE_H_
#define _GLSLAYER_SAMPLER_STATE_H_

#include "Common.h"
#include "Resource.h"


namespace gls
{

	struct SamplerStateDesc
	{
		TexAddressMode addressU = TexAddressMode::Repeat;
		TexAddressMode addressV = TexAddressMode::Repeat;
		TexAddressMode addressW = TexAddressMode::Repeat;
		TexFilter minFilter = TexFilter::NearestMipmapLinear;
		TexFilter magFilter = TexFilter::Linear;
		float borderColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		float minLOD = -1000.0f;
		float maxLOD = 1000.0f;
		float lodBias = 0.0f;
		TexCompareMode texCmpMode = TexCompareMode::None;
		CompareFunc texCmpFunc = CompareFunc::LessOrEqual;
		float maxAnisotropy = 1.0f;
	};


	class ISamplerState : public IResource
	{
	public:
	};

}


#endif // _GLSLAYER_SAMPLER_STATE_H_
