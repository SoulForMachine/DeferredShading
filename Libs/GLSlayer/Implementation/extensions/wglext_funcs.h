
GLenum glGetError();

// WGL_ARB_create_context

inline HGLRC wglCreateContextAttribsARB(HDC hDC, HGLRC hShareContext, const int *attribList)
{
	assert(ptr_wglCreateContextAttribsARB);
	HGLRC result;
	result = ptr_wglCreateContextAttribsARB(hDC, hShareContext, attribList);
#if defined(DEBUG_GL_CHECK_FOR_ERROR)
	GLenum error;
	assert((error = glGetError()) == GL_NO_ERROR);
#endif
	return result;
}

// WGL_ARB_pixel_format

inline BOOL wglChoosePixelFormatARB(HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats)
{
	assert(ptr_wglChoosePixelFormatARB);
	BOOL result;
	result = ptr_wglChoosePixelFormatARB(hdc, piAttribIList, pfAttribFList, nMaxFormats, piFormats, nNumFormats);
#if defined(DEBUG_GL_CHECK_FOR_ERROR)
	GLenum error;
	assert((error = glGetError()) == GL_NO_ERROR);
#endif
	return result;
}

// WGL_EXT_swap_control

inline BOOL wglSwapIntervalEXT(int interval)
{
	assert(ptr_wglSwapIntervalEXT);
	BOOL result;
	result = ptr_wglSwapIntervalEXT(interval);
#if defined(DEBUG_GL_CHECK_FOR_ERROR)
	GLenum error;
	assert((error = glGetError()) == GL_NO_ERROR);
#endif
	return result;
}
