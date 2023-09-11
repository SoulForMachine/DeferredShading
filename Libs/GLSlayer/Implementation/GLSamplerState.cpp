
#include <cassert>
#include "GLSamplerState.h"


namespace gls::internals
{

GLSamplerState::GLSamplerState()
{
	_id = 0;
	_resType = ResourceType::SamplerState;
	_target = 0;
}

bool GLSamplerState::Create(const SamplerStateDesc& desc)
{
	if (_id)
		return false;

	glGenSamplers(1, &_id);
	if (!_id)
		return false;

	glSamplerParameteri(_id, GL_TEXTURE_WRAP_S, GetGLEnum(desc.addressU));
	glSamplerParameteri(_id, GL_TEXTURE_WRAP_T, GetGLEnum(desc.addressV));
	glSamplerParameteri(_id, GL_TEXTURE_WRAP_R, GetGLEnum(desc.addressW));
	glSamplerParameteri(_id, GL_TEXTURE_MIN_FILTER, GetGLEnum(desc.minFilter));
	glSamplerParameteri(_id, GL_TEXTURE_MAG_FILTER, GetGLEnum(desc.magFilter));
	glSamplerParameterfv(_id, GL_TEXTURE_BORDER_COLOR, desc.borderColor);
	glSamplerParameterf(_id, GL_TEXTURE_MIN_LOD, desc.minLOD);
	glSamplerParameterf(_id, GL_TEXTURE_MAX_LOD, desc.maxLOD);
	glSamplerParameterf(_id, GL_TEXTURE_LOD_BIAS, desc.lodBias);
	glSamplerParameteri(_id, GL_TEXTURE_COMPARE_MODE, GetGLEnum(desc.texCmpMode));
	glSamplerParameteri(_id, GL_TEXTURE_COMPARE_FUNC, GetGLEnum(desc.texCmpFunc));
	glSamplerParameterf(_id, GL_TEXTURE_MAX_ANISOTROPY_EXT, desc.maxAnisotropy);

	if (glGetError() != GL_NO_ERROR)
		return false;

	return true;
}

void GLSamplerState::Destroy()
{
	if (_id != 0)
	{
		glDeleteSamplers(1, &_id);
	}
}

} // namespace gls::internals
