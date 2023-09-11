
#ifndef _GL_SAMPLER_STATE_H_
#define _GL_SAMPLER_STATE_H_

#include "GLSlayer/SamplerState.h"
#include "GLResource.h"


namespace gls::internals
{

class GLSamplerState : public ISamplerState, public GLResource
{
public:
	IMPLEMENT_IRESOURCE

	static const int typeID = TYPE_ID_SAMPLER_STATE;

	GLSamplerState();

	GLSamplerState(const GLSamplerState&) = delete;
	GLSamplerState& operator = (const GLSamplerState&) = delete;

	virtual void* DynamicCast(int type_id) override { return (type_id == TYPE_ID_SAMPLER_STATE) ? this : GLResource::DynamicCast(type_id); }
	bool Create(const SamplerStateDesc& desc);
	void Destroy();
};

} // namespace gls::internals

#endif // _GL_SAMPLER_STATE_H_
