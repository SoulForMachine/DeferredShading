
bool GLRenderContext::glextLoad_GLX_ARB_create_context()
{
	bool result = IsExtSupported("GLX_ARB_create_context");
	if(!result)
		return false;
	INIT_FUNC_PTR(glXCreateContextAttribsARB);
	_info.featuresGLX.ARB_create_context = result;
	return result;
}

bool GLRenderContext::glextLoad_GLX_ARB_fbconfig_float()
{
	_info.featuresGLX.ARB_fbconfig_float = IsExtSupported("GLX_ARB_fbconfig_float");
	return _info.featuresGLX.ARB_fbconfig_float;
}

bool GLRenderContext::glextLoad_GLX_ARB_multisample()
{
	_info.featuresGLX.ARB_multisample = IsExtSupported("GLX_ARB_multisample");
	return _info.featuresGLX.ARB_multisample;
}

bool GLRenderContext::glextLoad_GLX_EXT_framebuffer_sRGB()
{
	_info.featuresGLX.EXT_framebuffer_sRGB = IsExtSupported("GLX_EXT_framebuffer_sRGB");
	return _info.featuresGLX.EXT_framebuffer_sRGB;
}

bool GLRenderContext::glextLoad_GLX_EXT_swap_control()
{
	bool result = IsExtSupported("GLX_EXT_swap_control");
	if(!result)
		return false;
	INIT_FUNC_PTR(glXSwapIntervalEXT);
	_info.featuresGLX.EXT_swap_control = result;
	return result;
}

bool GLRenderContext::glextLoad_GLX_OML_swap_method()
{
	_info.featuresGLX.OML_swap_method = IsExtSupported("GLX_OML_swap_method");
	return _info.featuresGLX.OML_swap_method;
}
