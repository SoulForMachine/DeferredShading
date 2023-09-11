
#ifndef _GLSLAYER_FRAMEBUFFER_H_
#define _GLSLAYER_FRAMEBUFFER_H_

#include "Resource.h"


namespace gls
{

	class ITexture;


	class IRenderbuffer: public IResource
	{
	public:
	};


	struct FramebufferParams
	{
		int width;
		int height;
		int layers;
		int samples;
		bool fixedSampleLocations;
	};


	class IFramebuffer: public IResource
	{
	public:
		virtual void AttachTexture(AttachmentBuffer attachment, ITexture* texture, int level) = 0;
		virtual void AttachTextureLayer(AttachmentBuffer attachment, ITexture* texture, int level, int layer) = 0;
		virtual void AttachTextureFace(AttachmentBuffer attachment, ITexture* texture, int level, CubeFace face) = 0;
		virtual void AttachRenderbuffer(AttachmentBuffer attachment, IRenderbuffer* renderbuffer) = 0;
		virtual FramebufferStatus CheckStatus() = 0;
		virtual void InvalidateFramebuffer(int num_attachments, const AttachmentBuffer* attachments) = 0;
		virtual void InvalidateSubFramebuffer(int num_attachments, const AttachmentBuffer* attachments, int x, int y, int width, int height) = 0;
	};

}


#endif // _GLSLAYER_FRAMEBUFFER_H_
