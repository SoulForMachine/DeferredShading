
GLenum glGetError();

// GLX_ARB_create_context

inline GLXContext glXCreateContextAttribsARB(Display *dpy, GLXFBConfig config, GLXContext share_context, Bool direct, const int *attrib_list)
{
	assert(ptr_glXCreateContextAttribsARB);
	GLXContext result;
	result = ptr_glXCreateContextAttribsARB(dpy, config, share_context, direct, attrib_list);
#if defined(DEBUG_GL_CHECK_FOR_ERROR)
	GLenum error;
	assert((error = glGetError()) == GL_NO_ERROR);
#endif
	return result;
}

// GLX_EXT_swap_control

inline void glXSwapIntervalEXT(Display *dpy, GLXDrawable drawable, int interval)
{
	assert(ptr_glXSwapIntervalEXT);
	ptr_glXSwapIntervalEXT(dpy, drawable, interval);
#if defined(DEBUG_GL_CHECK_FOR_ERROR)
	GLenum error;
	assert((error = glGetError()) == GL_NO_ERROR);
#endif
}
