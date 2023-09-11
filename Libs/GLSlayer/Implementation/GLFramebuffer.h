
#ifndef _GL_FRAMEBUFFER_H_
#define _GL_FRAMEBUFFER_H_

#include "GLSlayer/Framebuffer.h"
#include "GLResource.h"


namespace gls::internals
{

class GLRenderbuffer : public IRenderbuffer, public GLResource
{
public:
	IMPLEMENT_IRESOURCE

	static const int typeID = TYPE_ID_RENDERBUFFER;

	GLRenderbuffer() = default;
	GLRenderbuffer(const GLRenderbuffer&) = delete;
	GLRenderbuffer& operator = (const GLRenderbuffer&) = delete;

	bool Create(GLState* gl_state, sizei samples, PixelFormat internal_format, sizei width, sizei height);
	void Destroy();

	virtual void* DynamicCast(int type_id) override { return (type_id == TYPE_ID_RENDERBUFFER) ? this : GLResource::DynamicCast(type_id); }

private:
	GLState* _glState;
};


class GLFramebuffer : public IFramebuffer, public GLResource
{
public:
	IMPLEMENT_IRESOURCE

	static const int typeID = TYPE_ID_FRAMEBUFFER;

	GLFramebuffer() = default;
	GLFramebuffer(const GLFramebuffer&) = delete;
	GLFramebuffer& operator = (const GLFramebuffer&) = delete;

	bool Create(GLState* gl_state);
	bool CreateWithoutAttachments(GLState* gl_state, const FramebufferParams& params);
	void Destroy();

	virtual void* DynamicCast(int type_id) override { return (type_id == TYPE_ID_FRAMEBUFFER) ? this : GLResource::DynamicCast(type_id); }
	virtual void AttachTexture(AttachmentBuffer attachment, ITexture* texture, int level) override;
	virtual void AttachTextureLayer(AttachmentBuffer attachment, ITexture* texture, int level, int layer) override;
	virtual void AttachTextureFace(AttachmentBuffer attachment, ITexture* texture, int level, CubeFace face) override;
	virtual void AttachRenderbuffer(AttachmentBuffer attachment, IRenderbuffer* renderbuffer) override;
	virtual FramebufferStatus CheckStatus() override;
	virtual void InvalidateFramebuffer(int num_attachments, const AttachmentBuffer* attachments) override;
	virtual void InvalidateSubFramebuffer(int num_attachments, const AttachmentBuffer* attachments, int x, int y, int width, int height) override;

private:
	GLState* _glState;
};

} // namespace gls::internals

#endif // _GL_FRAMEBUFFER_H_
