
bool GLRenderContext::glextLoad_WGL_ARB_create_context()
{
	bool result = IsExtSupported("WGL_ARB_create_context");
	if(!result)
		return false;
	INIT_FUNC_PTR(wglCreateContextAttribsARB);
	_info.featuresWGL.ARB_create_context = result;
	return result;
}

bool GLRenderContext::glextLoad_WGL_ARB_extensions_string()
{
	_info.featuresWGL.ARB_extensions_string = IsExtSupported("WGL_ARB_extensions_string");
	return _info.featuresWGL.ARB_extensions_string;
}

bool GLRenderContext::glextLoad_WGL_ARB_multisample()
{
	_info.featuresWGL.ARB_multisample = IsExtSupported("WGL_ARB_multisample");
	return _info.featuresWGL.ARB_multisample;
}

bool GLRenderContext::glextLoad_WGL_ARB_pixel_format()
{
	bool result = IsExtSupported("WGL_ARB_pixel_format");
	if(!result)
		return false;
	INIT_FUNC_PTR(wglChoosePixelFormatARB);
	_info.featuresWGL.ARB_pixel_format = result;
	return result;
}

bool GLRenderContext::glextLoad_WGL_ARB_pixel_format_float()
{
	_info.featuresWGL.ARB_pixel_format_float = IsExtSupported("WGL_ARB_pixel_format_float");
	return _info.featuresWGL.ARB_pixel_format_float;
}

bool GLRenderContext::glextLoad_WGL_EXT_framebuffer_sRGB()
{
	_info.featuresWGL.EXT_framebuffer_sRGB = IsExtSupported("WGL_EXT_framebuffer_sRGB");
	return _info.featuresWGL.EXT_framebuffer_sRGB;
}

bool GLRenderContext::glextLoad_WGL_EXT_swap_control()
{
	bool result = IsExtSupported("WGL_EXT_swap_control");
	if(!result)
		return false;
	INIT_FUNC_PTR(wglSwapIntervalEXT);
	_info.featuresWGL.EXT_swap_control = result;
	return result;
}
