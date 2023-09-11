
#include <cassert>
#include <malloc.h>
#include <cstdlib>
#include "GLTexture.h"
#include "GLFramebuffer.h"


// =================== Renderbuffer ==================

#define STATE_MACHINE_HACK \
	if(_id != _glState->renderbuffer) \
	{ \
		glBindRenderbuffer(_target, _id); \
		_glState->renderbuffer = _id; \
	}

namespace gls::internals
{

bool GLRenderbuffer::Create(GLState* gl_state, sizei samples, PixelFormat internal_format, sizei width, sizei height)
{
	if (_id)
		return false;

	_glState = gl_state;

	glGenRenderbuffers(1, &_id);
	if (!_id)
		return false;

	glBindRenderbuffer(GL_RENDERBUFFER, _id);
	_glState->renderbuffer = _id;
	_resType = ResourceType::Renderbuffer;
	_target = GL_RENDERBUFFER;

	glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GetGLEnum(internal_format), width, height);

	return true;
}

void GLRenderbuffer::Destroy()
{
	if (_id)
	{
		if (_id == _glState->renderbuffer)
		{
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			_glState->renderbuffer = 0;
		}
		glDeleteRenderbuffers(1, &_id);
		_id = 0;
	}
}

// =================== Framebuffer ==================

#undef STATE_MACHINE_HACK
#define STATE_MACHINE_HACK \
if(_id != _glState->drawFbuf) \
{ \
	glBindFramebuffer(_target, _id); \
	_glState->drawFbuf = _id; \
}


bool GLFramebuffer::Create(GLState* gl_state)
{
	if (_id)
		return false;

	_glState = gl_state;

	glGenFramebuffers(1, &_id);
	if (!_id)
		return false;

	_target = GL_DRAW_FRAMEBUFFER;
	_resType = ResourceType::Framebuffer;

	glBindFramebuffer(_target, _id);
	_glState->drawFbuf = _id;

	return true;
}

bool GLFramebuffer::CreateWithoutAttachments(GLState* gl_state, const FramebufferParams& params)
{
	if (Create(gl_state))
	{
		glFramebufferParameteri(_target, GL_FRAMEBUFFER_DEFAULT_WIDTH, params.width);
		glFramebufferParameteri(_target, GL_FRAMEBUFFER_DEFAULT_HEIGHT, params.height);
		glFramebufferParameteri(_target, GL_FRAMEBUFFER_DEFAULT_LAYERS, params.layers);
		glFramebufferParameteri(_target, GL_FRAMEBUFFER_DEFAULT_SAMPLES, params.samples);
		glFramebufferParameteri(_target, GL_FRAMEBUFFER_DEFAULT_FIXED_SAMPLE_LOCATIONS, params.fixedSampleLocations);

		return true;
	}

	return false;
}

void GLFramebuffer::Destroy()
{
	if (_id)
	{
		if (_id == _glState->drawFbuf)
		{
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
			_glState->drawFbuf = 0;
		}

		if (_id == _glState->readFbuf)
		{
			glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
			_glState->readFbuf = 0;
		}

		glDeleteFramebuffers(1, &_id);
		_id = 0;
	}
}

void GLFramebuffer::AttachTexture(AttachmentBuffer attachment, ITexture* texture, int level)
{
	assert(_id);
	STATE_MACHINE_HACK

	glFramebufferTexture(_target, GetGLEnum(attachment), dyn_cast_ptr<GLTexture*>(texture)->GetID(), level);
}

void GLFramebuffer::AttachTextureLayer(AttachmentBuffer attachment, ITexture* texture, int level, int layer)
{
	assert(_id);
	STATE_MACHINE_HACK

	glFramebufferTextureLayer(_target, GetGLEnum(attachment), dyn_cast_ptr<GLTexture*>(texture)->GetID(), level, layer);
}

void GLFramebuffer::AttachTextureFace(AttachmentBuffer attachment, ITexture* texture, int level, CubeFace face)
{
	assert(_id);
	STATE_MACHINE_HACK

	glFramebufferTexture2D(_target, GetGLEnum(attachment), GetGLEnum(face), dyn_cast_ptr<GLTexture*>(texture)->GetID(), level);
}

void GLFramebuffer::AttachRenderbuffer(AttachmentBuffer attachment, IRenderbuffer* renderbuffer)
{
	assert(_id);
	STATE_MACHINE_HACK

	glFramebufferRenderbuffer(_target, GetGLEnum(attachment), GL_RENDERBUFFER, dyn_cast_ptr<GLRenderbuffer*>(renderbuffer)->GetID());
}

FramebufferStatus GLFramebuffer::CheckStatus()
{
	assert(_id);
	STATE_MACHINE_HACK

	GLenum status = glCheckFramebufferStatus(_target);

	return GetFromGLEnum<FramebufferStatus>(status);
}

void GLFramebuffer::InvalidateFramebuffer(int num_attachments, const AttachmentBuffer* attachments)
{
	assert(_id);
	STATE_MACHINE_HACK

	if (num_attachments < 1 || num_attachments > 16)
	{
		assert(false);
		return;
	}

	GLenum* attach_enums = (GLenum*)alloca(sizeof(GLenum) * num_attachments);
	for (int i = 0; i < num_attachments; ++i)
		attach_enums[i] = GetGLEnum(attachments[i]);

	glInvalidateFramebuffer(_target, num_attachments, attach_enums);
}

void GLFramebuffer::InvalidateSubFramebuffer(int num_attachments, const AttachmentBuffer* attachments, int x, int y, int width, int height)
{
	assert(_id);
	STATE_MACHINE_HACK

	if (num_attachments < 1 || num_attachments > 16)
	{
		assert(false);
		return;
	}

	GLenum* attach_enums = (GLenum*)alloca(sizeof(GLenum) * num_attachments);
	for (int i = 0; i < num_attachments; ++i)
		attach_enums[i] = GetGLEnum(attachments[i]);

	glInvalidateSubFramebuffer(_target, num_attachments, attach_enums, x, y, width, height);
}

} // namespace gls::internals
