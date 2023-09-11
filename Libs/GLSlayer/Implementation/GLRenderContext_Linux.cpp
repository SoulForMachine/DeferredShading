
#include <cassert>
#include <cstring>
#include "GLSlayer/RenderContextInit.h"
#include "GLRenderContext.h"


static bool ctxErrorOccurred = false;
static int ctxErrorHandler( Display *dpy, XErrorEvent *ev )
{
	ctxErrorOccurred = true;
	return 0;
}

static
bool IsExtSupported(const char* extension, Display* display)
{
	if(!extension || !extension[0])
		return false;

	PFNGLGETSTRINGIPROC ptr_glGetStringi = (PFNGLGETSTRINGIPROC)glXGetProcAddressARB((const GLubyte*)"glGetStringi");
	PFNGLGETINTEGERVPROC ptr_glGetIntegerv = (PFNGLGETINTEGERVPROC)glXGetProcAddressARB((const GLubyte*)"glGetIntegerv");

	if (ptr_glGetStringi && ptr_glGetIntegerv)
	{
		GLint count = 0;
		ptr_glGetIntegerv(GL_NUM_EXTENSIONS, &count);
		for(int i = 0; i < count; ++i)
		{
			const char* ext_string = (const char*)ptr_glGetStringi(GL_EXTENSIONS, i);

			if(ext_string && !strcmp(ext_string, extension))
				return true;
		}
	}

	const char* ext_string = glXQueryExtensionsString(display, DefaultScreen(display));
	if(ext_string && strstr(ext_string, extension))
		return true;

	return false;
}

static
GLXFBConfig GetFBConfig(Display* display, const gls::FramebufferFormat& format)
{
	// check if sRGB framebuffer is supported
	bool sRGB = IsExtSupported("GLX_EXT_framebuffer_sRGB", display);

	int fb_attribs[] =
	{
		GLX_X_RENDERABLE    , True,
		GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
		GLX_RENDER_TYPE     , (int)gls::internals::GetGLEnum(format.colorBufferType),
		GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
		GLX_TRANSPARENT_TYPE, GLX_NONE,
		GLX_CONFIG_CAVEAT	, GLX_NONE,
		GLX_LEVEL			, 0,
		GLX_RED_SIZE        , format.colorBits / 4,
		GLX_GREEN_SIZE      , format.colorBits / 4,
		GLX_BLUE_SIZE       , format.colorBits / 4,
		GLX_ALPHA_SIZE      , format.colorBits / 4,
		GLX_BUFFER_SIZE		, format.colorBits,
		GLX_DEPTH_SIZE      , format.depthBits,
		GLX_STENCIL_SIZE    , format.stencilBits,
		GLX_DOUBLEBUFFER    , format.doubleBuffer ? True : False,
		GLX_SAMPLE_BUFFERS  , format.multisampleSamples ? 1 : 0,
		GLX_SAMPLES         , format.multisampleSamples,
		((sRGB && format.sRGB) ? GLX_FRAMEBUFFER_SRGB_CAPABLE_ARB : 0)	, ((sRGB && format.sRGB) ? 1 : 0),
		None
	};

	int fb_count;
	GLXFBConfig* configs = glXChooseFBConfig(display, DefaultScreen(display), fb_attribs, &fb_count);
	if(!configs)
		return 0;

	GLXFBConfig best_fbc = 0;
	for(int i = 0; i < fb_count; ++i)
	{
		XVisualInfo* vi = glXGetVisualFromFBConfig(display, configs[i]);
		if(vi)
		{
			int depth = vi->depth;
			XFree(vi);
			if(depth == 24)
			{
				best_fbc = configs[i];
				break;
			}
		}
	}

	XFree(configs);

	return best_fbc;
}


gls::IRenderContext* gls::CreateRenderContext(const gls::CreateContextInfo& info)
{
	gls::internals::GLRenderContext* render_context = new gls::internals::GLRenderContext(info.logger);
	bool result = render_context->Create(info.version, info.display, info.window, *info.format, info.debugContext, info.shareContext);
	if(!result)
	{
		delete render_context;
		render_context = 0;
	}
	return render_context;
}

bool gls::GetXVisualInfo(Display* display, const gls::FramebufferFormat& format, XVisualInfo& visual_info)
{
	GLXFBConfig fbc = GetFBConfig(display, format);
	if(!fbc)
		return false;

	XVisualInfo* vi = glXGetVisualFromFBConfig(display, fbc);
	if(!vi)
		return false;

	visual_info = *vi;
	XFree(vi);

	return true;
}

void gls::DestroyRenderContext(gls::IRenderContext* render_context)
{
	if(render_context)
	{
		static_cast<gls::internals::GLRenderContext*>(render_context)->Destroy();
		delete render_context;
	}
}

Window gls::SetContextWindow(gls::IRenderContext* render_context, Window window)
{
	auto rcImpl = static_cast<gls::internals::GLRenderContext*>(render_context);
	return rcImpl->SetContextWindow(window);
}

namespace gls::internals
{

Window GLRenderContext::SetContextWindow(Window window)
{
	Window old = _window;
	_window = window;
	return old;
}

bool GLRenderContext::Create(uint version, Display* display, Window window, const FramebufferFormat& format, bool debug_context, IRenderContext* shareContext)
{
	if(_initialized)
		return false;

	if(version < 330)
	{
		DebugMessage(DebugMessageSource::ThirdParty, DebugMessageType::Error, DebugMessageSeverity::High, ErrorMessageId::UnsupportedVersion);
		return false;
	}

	_window = window;
	_display = display;

	if(!CreateContext(version, format, debug_context, shareContext))
	{
		return false;
	}

	Bool result = glXMakeCurrent(_display, _window, _context);
	if(!result)
	{
		Destroy();
		DebugMessage(DebugMessageSource::ThirdParty, DebugMessageType::Error, DebugMessageSeverity::High, ErrorMessageId::CreateContext, version, "glXMakeCurrent");
		return false;
	}

	return InitCommon(version);
}

bool GLRenderContext::CreateContext(uint version, const FramebufferFormat& format, bool debug_context, IRenderContext* shareContext)
{
	ctxErrorOccurred = false;
	int (*oldHandler)(Display*, XErrorEvent*) = XSetErrorHandler(&ctxErrorHandler);

	GLXFBConfig fb_config = GetFBConfig(_display, format);
	if(!fb_config)
	{
		DebugMessage(DebugMessageSource::ThirdParty, DebugMessageType::Error, DebugMessageSeverity::High, ErrorMessageId::CreateContext, version, "failed to get GLXFBConfig.");
		return false;
	}

	// Initialize base GL version for some basic functions.
	if (!glextLoad_GL_VERSION_1_0())
	{
		DebugMessage(DebugMessageSource::ThirdParty, DebugMessageType::Error, DebugMessageSeverity::High, ErrorMessageId::CreateContext, version, "could not load version 1.0");
		return false;
	}

	GLXContext context = 0;
	if( IsExtSupported("GLX_ARB_create_context") &&
		glextLoad_GLX_ARB_create_context() &&
		ptr_glXCreateContextAttribsARB )
	{
		int context_flags = (version >= 300) ? GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB : 0;
		if(debug_context)
			context_flags |= GLX_CONTEXT_DEBUG_BIT_ARB;
		int attribs[] =
		{
			GLX_CONTEXT_MAJOR_VERSION_ARB, (int)version / 100,
			GLX_CONTEXT_MINOR_VERSION_ARB, (int)version % 100 / 10,
			GLX_CONTEXT_FLAGS_ARB, context_flags,
			GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
			None
		};

		GLXContext sharedContext = shareContext ? reinterpret_cast<GLRenderContext*>(shareContext)->_context : 0;
		context = glXCreateContextAttribsARB(_display, fb_config, sharedContext, True, attribs);
	}
	else
	{
		DebugMessage(
			DebugMessageSource::ThirdParty, DebugMessageType::Error, DebugMessageSeverity::High, ErrorMessageId::CreateContext, version,
			"extension GLX_ARB_create_context not supported, cannot create forward compatible context");
	}

	XSync(_display, False);
	XSetErrorHandler(oldHandler);

	if(!ctxErrorOccurred && context)
	{
		_context = context;
		_info.framebufferFormat = format;
		return true;
	}
	else
	{
		_context = 0;
		DebugMessage(DebugMessageSource::ThirdParty, DebugMessageType::Error, DebugMessageSeverity::High, ErrorMessageId::CreateContext, version, "glXCreateContextAttribsARB");
		return false;
	}
}

void GLRenderContext::Destroy()
{
	if(_context)
	{
		if(glXGetCurrentContext() == _context)
		{
			glXMakeCurrent(_display, 0, 0);
		}
		glXDestroyContext(_display, _context);

		DeinitCommon();
	}
}

bool GLRenderContext::LoadPlatformOpenGLExtensions()
{
	bool result = true;

	// GLX extensions
	LOAD_EXTENSION_REQ(GLX_ARB_multisample);
	LOAD_EXTENSION_REQ(GLX_EXT_swap_control);
	LOAD_EXTENSION(GLX_OML_swap_method);
	LOAD_EXTENSION(GLX_EXT_framebuffer_sRGB);
	LOAD_EXTENSION_REQ(GLX_ARB_create_context);
	LOAD_EXTENSION(GLX_ARB_fbconfig_float);

	return result;
}

bool GLRenderContext::IsExtSupported(const char* extension)
{
	return ::IsExtSupported(extension, _display);
}

bool GLRenderContext::SetCurrentContext()
{
	Bool result = glXMakeCurrent(_display, _window, _context);
	return (result == True);
}

void GLRenderContext::UnsetCurrentContext()
{
	glXMakeCurrent(_display, 0, 0);
}

void GLRenderContext::SwapBuffers()
{
	assert(_display && _window);
	glXSwapBuffers(_display, _window);
}

void GLRenderContext::SwapInterval(int interval)
{
	glXSwapIntervalEXT(_display, _window, interval);
}

} // namespace gls::internals

