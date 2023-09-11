
bool GLRenderContext::glextLoad_GL_VERSION_1_0()
{
	bool result = true;
	INIT_FUNC_PTR(glCullFace);
	INIT_FUNC_PTR(glFrontFace);
	INIT_FUNC_PTR(glLineWidth);
	INIT_FUNC_PTR(glPolygonMode);
	INIT_FUNC_PTR(glScissor);
	INIT_FUNC_PTR(glTexParameteri);
	INIT_FUNC_PTR(glTexParameteriv);
	INIT_FUNC_PTR(glColorMask);
	INIT_FUNC_PTR(glDepthMask);
	INIT_FUNC_PTR(glDisable);
	INIT_FUNC_PTR(glEnable);
	INIT_FUNC_PTR(glFinish);
	INIT_FUNC_PTR(glFlush);
	INIT_FUNC_PTR(glBlendFunc);
	INIT_FUNC_PTR(glLogicOp);
	INIT_FUNC_PTR(glDepthFunc);
	INIT_FUNC_PTR(glPixelStorei);
	INIT_FUNC_PTR(glReadBuffer);
	INIT_FUNC_PTR(glReadPixels);
	INIT_FUNC_PTR(glGetError);
	INIT_FUNC_PTR(glGetFloatv);
	INIT_FUNC_PTR(glGetIntegerv);
	INIT_FUNC_PTR(glGetString);
	INIT_FUNC_PTR(glGetTexImage);
	INIT_FUNC_PTR(glGetTexLevelParameteriv);
	INIT_FUNC_PTR(glDepthRange);
	INIT_FUNC_PTR(glViewport);
	_info.featuresGL.VERSION_1_0 = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_VERSION_1_1()
{
	bool result = true;
	INIT_FUNC_PTR(glDrawArrays);
	INIT_FUNC_PTR(glPolygonOffset);
	INIT_FUNC_PTR(glCopyTexImage1D);
	INIT_FUNC_PTR(glCopyTexImage2D);
	INIT_FUNC_PTR(glCopyTexSubImage1D);
	INIT_FUNC_PTR(glCopyTexSubImage2D);
	INIT_FUNC_PTR(glTexSubImage1D);
	INIT_FUNC_PTR(glTexSubImage2D);
	INIT_FUNC_PTR(glBindTexture);
	INIT_FUNC_PTR(glDeleteTextures);
	INIT_FUNC_PTR(glGenTextures);
	_info.featuresGL.VERSION_1_1 = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_VERSION_1_2()
{
	bool result = true;
	INIT_FUNC_PTR(glTexSubImage3D);
	INIT_FUNC_PTR(glCopyTexSubImage3D);
	_info.featuresGL.VERSION_1_2 = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_VERSION_1_3()
{
	bool result = true;
	INIT_FUNC_PTR(glActiveTexture);
	INIT_FUNC_PTR(glSampleCoverage);
	INIT_FUNC_PTR(glCompressedTexSubImage3D);
	INIT_FUNC_PTR(glCompressedTexSubImage2D);
	INIT_FUNC_PTR(glCompressedTexSubImage1D);
	INIT_FUNC_PTR(glGetCompressedTexImage);
	_info.featuresGL.VERSION_1_3 = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_VERSION_1_4()
{
	bool result = true;
	INIT_FUNC_PTR(glBlendFuncSeparate);
	INIT_FUNC_PTR(glBlendColor);
	INIT_FUNC_PTR(glBlendEquation);
	_info.featuresGL.VERSION_1_4 = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_VERSION_1_5()
{
	bool result = true;
	INIT_FUNC_PTR(glGenQueries);
	INIT_FUNC_PTR(glDeleteQueries);
	INIT_FUNC_PTR(glBeginQuery);
	INIT_FUNC_PTR(glEndQuery);
	INIT_FUNC_PTR(glGetQueryObjectiv);
	INIT_FUNC_PTR(glGetQueryObjectuiv);
	INIT_FUNC_PTR(glBindBuffer);
	INIT_FUNC_PTR(glDeleteBuffers);
	INIT_FUNC_PTR(glGenBuffers);
	INIT_FUNC_PTR(glBufferSubData);
	INIT_FUNC_PTR(glGetBufferSubData);
	INIT_FUNC_PTR(glUnmapBuffer);
	_info.featuresGL.VERSION_1_5 = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_VERSION_2_0()
{
	bool result = true;
	INIT_FUNC_PTR(glBlendEquationSeparate);
	INIT_FUNC_PTR(glDrawBuffers);
	INIT_FUNC_PTR(glStencilOpSeparate);
	INIT_FUNC_PTR(glStencilFuncSeparate);
	INIT_FUNC_PTR(glStencilMaskSeparate);
	INIT_FUNC_PTR(glAttachShader);
	INIT_FUNC_PTR(glCompileShader);
	INIT_FUNC_PTR(glCreateProgram);
	INIT_FUNC_PTR(glCreateShader);
	INIT_FUNC_PTR(glDeleteProgram);
	INIT_FUNC_PTR(glDeleteShader);
	INIT_FUNC_PTR(glDetachShader);
	INIT_FUNC_PTR(glDisableVertexAttribArray);
	INIT_FUNC_PTR(glEnableVertexAttribArray);
	INIT_FUNC_PTR(glGetProgramiv);
	INIT_FUNC_PTR(glGetProgramInfoLog);
	INIT_FUNC_PTR(glGetShaderiv);
	INIT_FUNC_PTR(glGetShaderInfoLog);
	INIT_FUNC_PTR(glLinkProgram);
	INIT_FUNC_PTR(glShaderSource);
	INIT_FUNC_PTR(glValidateProgram);
	INIT_FUNC_PTR(glVertexAttribPointer);
	_info.featuresGL.VERSION_2_0 = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_VERSION_2_1()
{
	bool result = true;
	_info.featuresGL.VERSION_2_1 = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_VERSION_3_0()
{
	bool result = true;
	INIT_FUNC_PTR(glColorMaski);
	INIT_FUNC_PTR(glGetIntegeri_v);
	INIT_FUNC_PTR(glEnablei);
	INIT_FUNC_PTR(glDisablei);
	INIT_FUNC_PTR(glBeginTransformFeedback);
	INIT_FUNC_PTR(glEndTransformFeedback);
	INIT_FUNC_PTR(glBindBufferRange);
	INIT_FUNC_PTR(glBindBufferBase);
	INIT_FUNC_PTR(glTransformFeedbackVaryings);
	INIT_FUNC_PTR(glClampColor);
	INIT_FUNC_PTR(glBeginConditionalRender);
	INIT_FUNC_PTR(glEndConditionalRender);
	INIT_FUNC_PTR(glVertexAttribIPointer);
	INIT_FUNC_PTR(glClearBufferiv);
	INIT_FUNC_PTR(glClearBufferuiv);
	INIT_FUNC_PTR(glClearBufferfv);
	INIT_FUNC_PTR(glClearBufferfi);
	result = glextLoad_GL_ARB_depth_buffer_float() && result;
	result = glextLoad_GL_ARB_framebuffer_object() && result;
	result = glextLoad_GL_ARB_framebuffer_sRGB() && result;
	result = glextLoad_GL_ARB_half_float_vertex() && result;
	result = glextLoad_GL_ARB_map_buffer_range() && result;
	result = glextLoad_GL_ARB_texture_compression_rgtc() && result;
	result = glextLoad_GL_ARB_texture_rg() && result;
	result = glextLoad_GL_ARB_vertex_array_object() && result;
	_info.featuresGL.VERSION_3_0 = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_ARB_depth_buffer_float()
{
	_info.featuresGL.ARB_depth_buffer_float = IsExtSupported("GL_ARB_depth_buffer_float");
	return _info.featuresGL.ARB_depth_buffer_float;
}

bool GLRenderContext::glextLoad_GL_ARB_framebuffer_object()
{
	bool result = IsExtSupported("GL_ARB_framebuffer_object");
	if(!result)
		return false;
	INIT_FUNC_PTR(glBindRenderbuffer);
	INIT_FUNC_PTR(glDeleteRenderbuffers);
	INIT_FUNC_PTR(glGenRenderbuffers);
	INIT_FUNC_PTR(glBindFramebuffer);
	INIT_FUNC_PTR(glDeleteFramebuffers);
	INIT_FUNC_PTR(glGenFramebuffers);
	INIT_FUNC_PTR(glCheckFramebufferStatus);
	INIT_FUNC_PTR(glFramebufferTexture2D);
	INIT_FUNC_PTR(glFramebufferRenderbuffer);
	INIT_FUNC_PTR(glGenerateMipmap);
	INIT_FUNC_PTR(glBlitFramebuffer);
	INIT_FUNC_PTR(glRenderbufferStorageMultisample);
	INIT_FUNC_PTR(glFramebufferTextureLayer);
	_info.featuresGL.ARB_framebuffer_object = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_ARB_framebuffer_sRGB()
{
	_info.featuresGL.ARB_framebuffer_sRGB = IsExtSupported("GL_ARB_framebuffer_sRGB");
	return _info.featuresGL.ARB_framebuffer_sRGB;
}

bool GLRenderContext::glextLoad_GL_ARB_half_float_vertex()
{
	_info.featuresGL.ARB_half_float_vertex = IsExtSupported("GL_ARB_half_float_vertex");
	return _info.featuresGL.ARB_half_float_vertex;
}

bool GLRenderContext::glextLoad_GL_ARB_map_buffer_range()
{
	bool result = IsExtSupported("GL_ARB_map_buffer_range");
	if(!result)
		return false;
	INIT_FUNC_PTR(glMapBufferRange);
	INIT_FUNC_PTR(glFlushMappedBufferRange);
	_info.featuresGL.ARB_map_buffer_range = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_ARB_texture_compression_rgtc()
{
	_info.featuresGL.ARB_texture_compression_rgtc = IsExtSupported("GL_ARB_texture_compression_rgtc");
	return _info.featuresGL.ARB_texture_compression_rgtc;
}

bool GLRenderContext::glextLoad_GL_ARB_texture_rg()
{
	_info.featuresGL.ARB_texture_rg = IsExtSupported("GL_ARB_texture_rg");
	return _info.featuresGL.ARB_texture_rg;
}

bool GLRenderContext::glextLoad_GL_ARB_vertex_array_object()
{
	bool result = IsExtSupported("GL_ARB_vertex_array_object");
	if(!result)
		return false;
	INIT_FUNC_PTR(glBindVertexArray);
	INIT_FUNC_PTR(glGenVertexArrays);
	_info.featuresGL.ARB_vertex_array_object = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_VERSION_3_1()
{
	bool result = true;
	INIT_FUNC_PTR(glTexBuffer);
	INIT_FUNC_PTR(glPrimitiveRestartIndex);
	result = glextLoad_GL_ARB_copy_buffer() && result;
	result = glextLoad_GL_ARB_uniform_buffer_object() && result;
	_info.featuresGL.VERSION_3_1 = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_ARB_copy_buffer()
{
	bool result = IsExtSupported("GL_ARB_copy_buffer");
	if(!result)
		return false;
	INIT_FUNC_PTR(glCopyBufferSubData);
	_info.featuresGL.ARB_copy_buffer = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_ARB_uniform_buffer_object()
{
	bool result = IsExtSupported("GL_ARB_uniform_buffer_object");
	if(!result)
		return false;
	INIT_FUNC_PTR(glGetActiveUniformsiv);
	INIT_FUNC_PTR(glGetActiveUniformName);
	INIT_FUNC_PTR(glGetUniformBlockIndex);
	INIT_FUNC_PTR(glGetActiveUniformBlockiv);
	_info.featuresGL.ARB_uniform_buffer_object = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_VERSION_3_2()
{
	bool result = true;
	result = glextLoad_GL_ARB_depth_clamp() && result;
	result = glextLoad_GL_ARB_draw_elements_base_vertex() && result;
	result = glextLoad_GL_ARB_fragment_coord_conventions() && result;
	result = glextLoad_GL_ARB_provoking_vertex() && result;
	result = glextLoad_GL_ARB_seamless_cube_map() && result;
	result = glextLoad_GL_ARB_sync() && result;
	result = glextLoad_GL_ARB_texture_multisample() && result;
	_info.featuresGL.VERSION_3_2 = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_ARB_depth_clamp()
{
	_info.featuresGL.ARB_depth_clamp = IsExtSupported("GL_ARB_depth_clamp");
	return _info.featuresGL.ARB_depth_clamp;
}

bool GLRenderContext::glextLoad_GL_ARB_draw_elements_base_vertex()
{
	bool result = IsExtSupported("GL_ARB_draw_elements_base_vertex");
	if(!result)
		return false;
	INIT_FUNC_PTR(glDrawElementsBaseVertex);
	INIT_FUNC_PTR(glDrawRangeElementsBaseVertex);
	_info.featuresGL.ARB_draw_elements_base_vertex = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_ARB_fragment_coord_conventions()
{
	_info.featuresGL.ARB_fragment_coord_conventions = IsExtSupported("GL_ARB_fragment_coord_conventions");
	return _info.featuresGL.ARB_fragment_coord_conventions;
}

bool GLRenderContext::glextLoad_GL_ARB_provoking_vertex()
{
	bool result = IsExtSupported("GL_ARB_provoking_vertex");
	if(!result)
		return false;
	INIT_FUNC_PTR(glProvokingVertex);
	_info.featuresGL.ARB_provoking_vertex = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_ARB_seamless_cube_map()
{
	_info.featuresGL.ARB_seamless_cube_map = IsExtSupported("GL_ARB_seamless_cube_map");
	return _info.featuresGL.ARB_seamless_cube_map;
}

bool GLRenderContext::glextLoad_GL_ARB_sync()
{
	bool result = IsExtSupported("GL_ARB_sync");
	if(!result)
		return false;
	INIT_FUNC_PTR(glFenceSync);
	INIT_FUNC_PTR(glDeleteSync);
	INIT_FUNC_PTR(glClientWaitSync);
	INIT_FUNC_PTR(glWaitSync);
	INIT_FUNC_PTR(glGetInteger64v);
	_info.featuresGL.ARB_sync = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_ARB_texture_multisample()
{
	bool result = IsExtSupported("GL_ARB_texture_multisample");
	if(!result)
		return false;
	INIT_FUNC_PTR(glFramebufferTexture);
	INIT_FUNC_PTR(glGetMultisamplefv);
	INIT_FUNC_PTR(glSampleMaski);
	_info.featuresGL.ARB_texture_multisample = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_VERSION_3_3()
{
	bool result = true;
	result = glextLoad_GL_ARB_blend_func_extended() && result;
	result = glextLoad_GL_ARB_explicit_attrib_location() && result;
	result = glextLoad_GL_ARB_occlusion_query2() && result;
	result = glextLoad_GL_ARB_sampler_objects() && result;
	result = glextLoad_GL_ARB_shader_bit_encoding() && result;
	result = glextLoad_GL_ARB_texture_rgb10_a2ui() && result;
	result = glextLoad_GL_ARB_texture_swizzle() && result;
	result = glextLoad_GL_ARB_timer_query() && result;
	result = glextLoad_GL_ARB_vertex_type_2_10_10_10_rev() && result;
	_info.featuresGL.VERSION_3_3 = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_ARB_blend_func_extended()
{
	_info.featuresGL.ARB_blend_func_extended = IsExtSupported("GL_ARB_blend_func_extended");
	return _info.featuresGL.ARB_blend_func_extended;
}

bool GLRenderContext::glextLoad_GL_ARB_explicit_attrib_location()
{
	_info.featuresGL.ARB_explicit_attrib_location = IsExtSupported("GL_ARB_explicit_attrib_location");
	return _info.featuresGL.ARB_explicit_attrib_location;
}

bool GLRenderContext::glextLoad_GL_ARB_occlusion_query2()
{
	_info.featuresGL.ARB_occlusion_query2 = IsExtSupported("GL_ARB_occlusion_query2");
	return _info.featuresGL.ARB_occlusion_query2;
}

bool GLRenderContext::glextLoad_GL_ARB_sampler_objects()
{
	bool result = IsExtSupported("GL_ARB_sampler_objects");
	if(!result)
		return false;
	INIT_FUNC_PTR(glGenSamplers);
	INIT_FUNC_PTR(glDeleteSamplers);
	INIT_FUNC_PTR(glBindSampler);
	INIT_FUNC_PTR(glSamplerParameteri);
	INIT_FUNC_PTR(glSamplerParameterf);
	INIT_FUNC_PTR(glSamplerParameterfv);
	_info.featuresGL.ARB_sampler_objects = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_ARB_shader_bit_encoding()
{
	_info.featuresGL.ARB_shader_bit_encoding = IsExtSupported("GL_ARB_shader_bit_encoding");
	return _info.featuresGL.ARB_shader_bit_encoding;
}

bool GLRenderContext::glextLoad_GL_ARB_texture_rgb10_a2ui()
{
	_info.featuresGL.ARB_texture_rgb10_a2ui = IsExtSupported("GL_ARB_texture_rgb10_a2ui");
	return _info.featuresGL.ARB_texture_rgb10_a2ui;
}

bool GLRenderContext::glextLoad_GL_ARB_texture_swizzle()
{
	_info.featuresGL.ARB_texture_swizzle = IsExtSupported("GL_ARB_texture_swizzle");
	return _info.featuresGL.ARB_texture_swizzle;
}

bool GLRenderContext::glextLoad_GL_ARB_timer_query()
{
	bool result = IsExtSupported("GL_ARB_timer_query");
	if(!result)
		return false;
	INIT_FUNC_PTR(glQueryCounter);
	INIT_FUNC_PTR(glGetQueryObjectui64v);
	_info.featuresGL.ARB_timer_query = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_ARB_vertex_type_2_10_10_10_rev()
{
	bool result = IsExtSupported("GL_ARB_vertex_type_2_10_10_10_rev");
	if(!result)
		return false;
	INIT_FUNC_PTR(glVertexAttribDivisor);
	_info.featuresGL.ARB_vertex_type_2_10_10_10_rev = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_VERSION_4_0()
{
	bool result = true;
	INIT_FUNC_PTR(glMinSampleShading);
	INIT_FUNC_PTR(glBlendEquationi);
	INIT_FUNC_PTR(glBlendEquationSeparatei);
	INIT_FUNC_PTR(glBlendFunci);
	INIT_FUNC_PTR(glBlendFuncSeparatei);
	result = glextLoad_GL_ARB_draw_buffers_blend() && result;
	result = glextLoad_GL_ARB_draw_indirect() && result;
	result = glextLoad_GL_ARB_gpu_shader5() && result;
	result = glextLoad_GL_ARB_gpu_shader_fp64() && result;
	result = glextLoad_GL_ARB_shader_subroutine() && result;
	result = glextLoad_GL_ARB_tessellation_shader() && result;
	result = glextLoad_GL_ARB_texture_buffer_object_rgb32() && result;
	result = glextLoad_GL_ARB_texture_cube_map_array() && result;
	result = glextLoad_GL_ARB_texture_gather() && result;
	result = glextLoad_GL_ARB_texture_query_lod() && result;
	result = glextLoad_GL_ARB_transform_feedback2() && result;
	result = glextLoad_GL_ARB_transform_feedback3() && result;
	_info.featuresGL.VERSION_4_0 = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_ARB_draw_buffers_blend()
{
	_info.featuresGL.ARB_draw_buffers_blend = IsExtSupported("GL_ARB_draw_buffers_blend");
	return _info.featuresGL.ARB_draw_buffers_blend;
}

bool GLRenderContext::glextLoad_GL_ARB_draw_indirect()
{
	bool result = IsExtSupported("GL_ARB_draw_indirect");
	if(!result)
		return false;
	INIT_FUNC_PTR(glDrawArraysIndirect);
	INIT_FUNC_PTR(glDrawElementsIndirect);
	_info.featuresGL.ARB_draw_indirect = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_ARB_gpu_shader5()
{
	_info.featuresGL.ARB_gpu_shader5 = IsExtSupported("GL_ARB_gpu_shader5");
	return _info.featuresGL.ARB_gpu_shader5;
}

bool GLRenderContext::glextLoad_GL_ARB_gpu_shader_fp64()
{
	_info.featuresGL.ARB_gpu_shader_fp64 = IsExtSupported("GL_ARB_gpu_shader_fp64");
	return _info.featuresGL.ARB_gpu_shader_fp64;
}

bool GLRenderContext::glextLoad_GL_ARB_shader_subroutine()
{
	bool result = IsExtSupported("GL_ARB_shader_subroutine");
	if(!result)
		return false;
	INIT_FUNC_PTR(glGetSubroutineIndex);
	INIT_FUNC_PTR(glUniformSubroutinesuiv);
	_info.featuresGL.ARB_shader_subroutine = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_ARB_tessellation_shader()
{
	bool result = IsExtSupported("GL_ARB_tessellation_shader");
	if(!result)
		return false;
	INIT_FUNC_PTR(glPatchParameteri);
	INIT_FUNC_PTR(glPatchParameterfv);
	_info.featuresGL.ARB_tessellation_shader = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_ARB_texture_buffer_object_rgb32()
{
	_info.featuresGL.ARB_texture_buffer_object_rgb32 = IsExtSupported("GL_ARB_texture_buffer_object_rgb32");
	return _info.featuresGL.ARB_texture_buffer_object_rgb32;
}

bool GLRenderContext::glextLoad_GL_ARB_texture_cube_map_array()
{
	_info.featuresGL.ARB_texture_cube_map_array = IsExtSupported("GL_ARB_texture_cube_map_array");
	return _info.featuresGL.ARB_texture_cube_map_array;
}

bool GLRenderContext::glextLoad_GL_ARB_texture_gather()
{
	_info.featuresGL.ARB_texture_gather = IsExtSupported("GL_ARB_texture_gather");
	return _info.featuresGL.ARB_texture_gather;
}

bool GLRenderContext::glextLoad_GL_ARB_texture_query_lod()
{
	_info.featuresGL.ARB_texture_query_lod = IsExtSupported("GL_ARB_texture_query_lod");
	return _info.featuresGL.ARB_texture_query_lod;
}

bool GLRenderContext::glextLoad_GL_ARB_transform_feedback2()
{
	bool result = IsExtSupported("GL_ARB_transform_feedback2");
	if(!result)
		return false;
	INIT_FUNC_PTR(glBindTransformFeedback);
	INIT_FUNC_PTR(glDeleteTransformFeedbacks);
	INIT_FUNC_PTR(glGenTransformFeedbacks);
	INIT_FUNC_PTR(glPauseTransformFeedback);
	INIT_FUNC_PTR(glResumeTransformFeedback);
	_info.featuresGL.ARB_transform_feedback2 = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_ARB_transform_feedback3()
{
	bool result = IsExtSupported("GL_ARB_transform_feedback3");
	if(!result)
		return false;
	INIT_FUNC_PTR(glDrawTransformFeedbackStream);
	INIT_FUNC_PTR(glBeginQueryIndexed);
	_info.featuresGL.ARB_transform_feedback3 = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_VERSION_4_1()
{
	bool result = true;
	result = glextLoad_GL_ARB_ES2_compatibility() && result;
	result = glextLoad_GL_ARB_get_program_binary() && result;
	result = glextLoad_GL_ARB_separate_shader_objects() && result;
	result = glextLoad_GL_ARB_shader_precision() && result;
	result = glextLoad_GL_ARB_vertex_attrib_64bit() && result;
	result = glextLoad_GL_ARB_viewport_array() && result;
	_info.featuresGL.VERSION_4_1 = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_ARB_ES2_compatibility()
{
	_info.featuresGL.ARB_ES2_compatibility = IsExtSupported("GL_ARB_ES2_compatibility");
	return _info.featuresGL.ARB_ES2_compatibility;
}

bool GLRenderContext::glextLoad_GL_ARB_get_program_binary()
{
	bool result = IsExtSupported("GL_ARB_get_program_binary");
	if(!result)
		return false;
	INIT_FUNC_PTR(glGetProgramBinary);
	INIT_FUNC_PTR(glProgramBinary);
	INIT_FUNC_PTR(glProgramParameteri);
	_info.featuresGL.ARB_get_program_binary = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_ARB_separate_shader_objects()
{
	bool result = IsExtSupported("GL_ARB_separate_shader_objects");
	if(!result)
		return false;
	INIT_FUNC_PTR(glUseProgramStages);
	INIT_FUNC_PTR(glCreateShaderProgramv);
	INIT_FUNC_PTR(glBindProgramPipeline);
	INIT_FUNC_PTR(glGenProgramPipelines);
	INIT_FUNC_PTR(glGetProgramPipelineiv);
	INIT_FUNC_PTR(glValidateProgramPipeline);
	_info.featuresGL.ARB_separate_shader_objects = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_ARB_shader_precision()
{
	_info.featuresGL.ARB_shader_precision = IsExtSupported("GL_ARB_shader_precision");
	return _info.featuresGL.ARB_shader_precision;
}

bool GLRenderContext::glextLoad_GL_ARB_vertex_attrib_64bit()
{
	bool result = IsExtSupported("GL_ARB_vertex_attrib_64bit");
	if(!result)
		return false;
	INIT_FUNC_PTR(glVertexAttribLPointer);
	_info.featuresGL.ARB_vertex_attrib_64bit = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_ARB_viewport_array()
{
	bool result = IsExtSupported("GL_ARB_viewport_array");
	if(!result)
		return false;
	INIT_FUNC_PTR(glViewportIndexedf);
	INIT_FUNC_PTR(glScissorIndexed);
	INIT_FUNC_PTR(glDepthRangeIndexed);
	INIT_FUNC_PTR(glGetFloati_v);
	_info.featuresGL.ARB_viewport_array = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_VERSION_4_2()
{
	bool result = true;
	result = glextLoad_GL_ARB_base_instance() && result;
	result = glextLoad_GL_ARB_shading_language_420pack() && result;
	result = glextLoad_GL_ARB_transform_feedback_instanced() && result;
	result = glextLoad_GL_ARB_compressed_texture_pixel_storage() && result;
	result = glextLoad_GL_ARB_conservative_depth() && result;
	result = glextLoad_GL_ARB_internalformat_query() && result;
	result = glextLoad_GL_ARB_map_buffer_alignment() && result;
	result = glextLoad_GL_ARB_shader_atomic_counters() && result;
	result = glextLoad_GL_ARB_shader_image_load_store() && result;
	result = glextLoad_GL_ARB_shading_language_packing() && result;
	result = glextLoad_GL_ARB_texture_compression_bptc() && result;
	result = glextLoad_GL_ARB_texture_storage() && result;
	_info.featuresGL.VERSION_4_2 = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_ARB_base_instance()
{
	bool result = IsExtSupported("GL_ARB_base_instance");
	if(!result)
		return false;
	INIT_FUNC_PTR(glDrawArraysInstancedBaseInstance);
	INIT_FUNC_PTR(glDrawElementsInstancedBaseVertexBaseInstance);
	_info.featuresGL.ARB_base_instance = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_ARB_shading_language_420pack()
{
	_info.featuresGL.ARB_shading_language_420pack = IsExtSupported("GL_ARB_shading_language_420pack");
	return _info.featuresGL.ARB_shading_language_420pack;
}

bool GLRenderContext::glextLoad_GL_ARB_transform_feedback_instanced()
{
	bool result = IsExtSupported("GL_ARB_transform_feedback_instanced");
	if(!result)
		return false;
	INIT_FUNC_PTR(glDrawTransformFeedbackStreamInstanced);
	_info.featuresGL.ARB_transform_feedback_instanced = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_ARB_compressed_texture_pixel_storage()
{
	_info.featuresGL.ARB_compressed_texture_pixel_storage = IsExtSupported("GL_ARB_compressed_texture_pixel_storage");
	return _info.featuresGL.ARB_compressed_texture_pixel_storage;
}

bool GLRenderContext::glextLoad_GL_ARB_conservative_depth()
{
	_info.featuresGL.ARB_conservative_depth = IsExtSupported("GL_ARB_conservative_depth");
	return _info.featuresGL.ARB_conservative_depth;
}

bool GLRenderContext::glextLoad_GL_ARB_internalformat_query()
{
	bool result = IsExtSupported("GL_ARB_internalformat_query");
	if(!result)
		return false;
	INIT_FUNC_PTR(glGetInternalformativ);
	_info.featuresGL.ARB_internalformat_query = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_ARB_map_buffer_alignment()
{
	_info.featuresGL.ARB_map_buffer_alignment = IsExtSupported("GL_ARB_map_buffer_alignment");
	return _info.featuresGL.ARB_map_buffer_alignment;
}

bool GLRenderContext::glextLoad_GL_ARB_shader_atomic_counters()
{
	_info.featuresGL.ARB_shader_atomic_counters = IsExtSupported("GL_ARB_shader_atomic_counters");
	return _info.featuresGL.ARB_shader_atomic_counters;
}

bool GLRenderContext::glextLoad_GL_ARB_shader_image_load_store()
{
	bool result = IsExtSupported("GL_ARB_shader_image_load_store");
	if(!result)
		return false;
	INIT_FUNC_PTR(glBindImageTexture);
	INIT_FUNC_PTR(glMemoryBarrier);
	_info.featuresGL.ARB_shader_image_load_store = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_ARB_shading_language_packing()
{
	_info.featuresGL.ARB_shading_language_packing = IsExtSupported("GL_ARB_shading_language_packing");
	return _info.featuresGL.ARB_shading_language_packing;
}

bool GLRenderContext::glextLoad_GL_ARB_texture_compression_bptc()
{
	_info.featuresGL.ARB_texture_compression_bptc = IsExtSupported("GL_ARB_texture_compression_bptc");
	return _info.featuresGL.ARB_texture_compression_bptc;
}

bool GLRenderContext::glextLoad_GL_ARB_texture_storage()
{
	bool result = IsExtSupported("GL_ARB_texture_storage");
	if(!result)
		return false;
	INIT_FUNC_PTR(glTexStorage1D);
	INIT_FUNC_PTR(glTexStorage2D);
	INIT_FUNC_PTR(glTexStorage3D);
	_info.featuresGL.ARB_texture_storage = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_VERSION_4_3()
{
	bool result = true;
	result = glextLoad_GL_ARB_arrays_of_arrays() && result;
	result = glextLoad_GL_ARB_fragment_layer_viewport() && result;
	result = glextLoad_GL_ARB_shader_image_size() && result;
	result = glextLoad_GL_ARB_ES3_compatibility() && result;
	result = glextLoad_GL_ARB_clear_buffer_object() && result;
	result = glextLoad_GL_ARB_compute_shader() && result;
	result = glextLoad_GL_ARB_copy_image() && result;
	result = glextLoad_GL_KHR_debug() && result;
	result = glextLoad_GL_ARB_explicit_uniform_location() && result;
	result = glextLoad_GL_ARB_framebuffer_no_attachments() && result;
	result = glextLoad_GL_ARB_internalformat_query2() && result;
	result = glextLoad_GL_ARB_invalidate_subdata() && result;
	result = glextLoad_GL_ARB_multi_draw_indirect() && result;
	result = glextLoad_GL_ARB_program_interface_query() && result;
	result = glextLoad_GL_ARB_robust_buffer_access_behavior() && result;
	result = glextLoad_GL_ARB_shader_storage_buffer_object() && result;
	result = glextLoad_GL_ARB_stencil_texturing() && result;
	result = glextLoad_GL_ARB_texture_buffer_range() && result;
	result = glextLoad_GL_ARB_texture_query_levels() && result;
	result = glextLoad_GL_ARB_texture_storage_multisample() && result;
	result = glextLoad_GL_ARB_texture_view() && result;
	result = glextLoad_GL_ARB_vertex_attrib_binding() && result;
	_info.featuresGL.VERSION_4_3 = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_ARB_arrays_of_arrays()
{
	_info.featuresGL.ARB_arrays_of_arrays = IsExtSupported("GL_ARB_arrays_of_arrays");
	return _info.featuresGL.ARB_arrays_of_arrays;
}

bool GLRenderContext::glextLoad_GL_ARB_fragment_layer_viewport()
{
	_info.featuresGL.ARB_fragment_layer_viewport = IsExtSupported("GL_ARB_fragment_layer_viewport");
	return _info.featuresGL.ARB_fragment_layer_viewport;
}

bool GLRenderContext::glextLoad_GL_ARB_shader_image_size()
{
	_info.featuresGL.ARB_shader_image_size = IsExtSupported("GL_ARB_shader_image_size");
	return _info.featuresGL.ARB_shader_image_size;
}

bool GLRenderContext::glextLoad_GL_ARB_ES3_compatibility()
{
	_info.featuresGL.ARB_ES3_compatibility = IsExtSupported("GL_ARB_ES3_compatibility");
	return _info.featuresGL.ARB_ES3_compatibility;
}

bool GLRenderContext::glextLoad_GL_ARB_clear_buffer_object()
{
	bool result = IsExtSupported("GL_ARB_clear_buffer_object");
	if(!result)
		return false;
	INIT_FUNC_PTR(glClearBufferData);
	INIT_FUNC_PTR(glClearBufferSubData);
	_info.featuresGL.ARB_clear_buffer_object = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_ARB_compute_shader()
{
	bool result = IsExtSupported("GL_ARB_compute_shader");
	if(!result)
		return false;
	INIT_FUNC_PTR(glDispatchCompute);
	INIT_FUNC_PTR(glDispatchComputeIndirect);
	_info.featuresGL.ARB_compute_shader = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_ARB_copy_image()
{
	bool result = IsExtSupported("GL_ARB_copy_image");
	if(!result)
		return false;
	INIT_FUNC_PTR(glCopyImageSubData);
	_info.featuresGL.ARB_copy_image = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_KHR_debug()
{
	bool result = IsExtSupported("GL_KHR_debug");
	if(!result)
		return false;
	INIT_FUNC_PTR(glDebugMessageControl);
	INIT_FUNC_PTR(glDebugMessageInsert);
	INIT_FUNC_PTR(glDebugMessageCallback);
	INIT_FUNC_PTR(glPushDebugGroup);
	INIT_FUNC_PTR(glPopDebugGroup);
	INIT_FUNC_PTR(glObjectLabel);
	INIT_FUNC_PTR(glObjectPtrLabel);
	_info.featuresGL.KHR_debug = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_ARB_explicit_uniform_location()
{
	_info.featuresGL.ARB_explicit_uniform_location = IsExtSupported("GL_ARB_explicit_uniform_location");
	return _info.featuresGL.ARB_explicit_uniform_location;
}

bool GLRenderContext::glextLoad_GL_ARB_framebuffer_no_attachments()
{
	bool result = IsExtSupported("GL_ARB_framebuffer_no_attachments");
	if(!result)
		return false;
	INIT_FUNC_PTR(glFramebufferParameteri);
	_info.featuresGL.ARB_framebuffer_no_attachments = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_ARB_internalformat_query2()
{
	bool result = IsExtSupported("GL_ARB_internalformat_query2");
	if(!result)
		return false;
	INIT_FUNC_PTR(glGetInternalformati64v);
	_info.featuresGL.ARB_internalformat_query2 = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_ARB_invalidate_subdata()
{
	bool result = IsExtSupported("GL_ARB_invalidate_subdata");
	if(!result)
		return false;
	INIT_FUNC_PTR(glInvalidateTexSubImage);
	INIT_FUNC_PTR(glInvalidateTexImage);
	INIT_FUNC_PTR(glInvalidateBufferSubData);
	INIT_FUNC_PTR(glInvalidateBufferData);
	INIT_FUNC_PTR(glInvalidateFramebuffer);
	INIT_FUNC_PTR(glInvalidateSubFramebuffer);
	_info.featuresGL.ARB_invalidate_subdata = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_ARB_multi_draw_indirect()
{
	bool result = IsExtSupported("GL_ARB_multi_draw_indirect");
	if(!result)
		return false;
	INIT_FUNC_PTR(glMultiDrawArraysIndirect);
	INIT_FUNC_PTR(glMultiDrawElementsIndirect);
	_info.featuresGL.ARB_multi_draw_indirect = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_ARB_program_interface_query()
{
	_info.featuresGL.ARB_program_interface_query = IsExtSupported("GL_ARB_program_interface_query");
	return _info.featuresGL.ARB_program_interface_query;
}

bool GLRenderContext::glextLoad_GL_ARB_robust_buffer_access_behavior()
{
	_info.featuresGL.ARB_robust_buffer_access_behavior = IsExtSupported("GL_ARB_robust_buffer_access_behavior");
	return _info.featuresGL.ARB_robust_buffer_access_behavior;
}

bool GLRenderContext::glextLoad_GL_ARB_shader_storage_buffer_object()
{
	_info.featuresGL.ARB_shader_storage_buffer_object = IsExtSupported("GL_ARB_shader_storage_buffer_object");
	return _info.featuresGL.ARB_shader_storage_buffer_object;
}

bool GLRenderContext::glextLoad_GL_ARB_stencil_texturing()
{
	_info.featuresGL.ARB_stencil_texturing = IsExtSupported("GL_ARB_stencil_texturing");
	return _info.featuresGL.ARB_stencil_texturing;
}

bool GLRenderContext::glextLoad_GL_ARB_texture_buffer_range()
{
	bool result = IsExtSupported("GL_ARB_texture_buffer_range");
	if(!result)
		return false;
	INIT_FUNC_PTR(glTexBufferRange);
	_info.featuresGL.ARB_texture_buffer_range = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_ARB_texture_query_levels()
{
	_info.featuresGL.ARB_texture_query_levels = IsExtSupported("GL_ARB_texture_query_levels");
	return _info.featuresGL.ARB_texture_query_levels;
}

bool GLRenderContext::glextLoad_GL_ARB_texture_storage_multisample()
{
	bool result = IsExtSupported("GL_ARB_texture_storage_multisample");
	if(!result)
		return false;
	INIT_FUNC_PTR(glTexStorage2DMultisample);
	INIT_FUNC_PTR(glTexStorage3DMultisample);
	_info.featuresGL.ARB_texture_storage_multisample = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_ARB_texture_view()
{
	bool result = IsExtSupported("GL_ARB_texture_view");
	if(!result)
		return false;
	INIT_FUNC_PTR(glTextureView);
	_info.featuresGL.ARB_texture_view = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_ARB_vertex_attrib_binding()
{
	bool result = IsExtSupported("GL_ARB_vertex_attrib_binding");
	if(!result)
		return false;
	INIT_FUNC_PTR(glBindVertexBuffer);
	INIT_FUNC_PTR(glVertexAttribFormat);
	INIT_FUNC_PTR(glVertexAttribIFormat);
	INIT_FUNC_PTR(glVertexAttribLFormat);
	INIT_FUNC_PTR(glVertexAttribBinding);
	INIT_FUNC_PTR(glVertexBindingDivisor);
	_info.featuresGL.ARB_vertex_attrib_binding = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_VERSION_4_4()
{
	bool result = true;
	result = glextLoad_GL_ARB_buffer_storage() && result;
	result = glextLoad_GL_ARB_clear_texture() && result;
	result = glextLoad_GL_ARB_enhanced_layouts() && result;
	result = glextLoad_GL_ARB_multi_bind() && result;
	result = glextLoad_GL_ARB_query_buffer_object() && result;
	result = glextLoad_GL_ARB_texture_mirror_clamp_to_edge() && result;
	result = glextLoad_GL_ARB_texture_stencil8() && result;
	result = glextLoad_GL_ARB_vertex_type_10f_11f_11f_rev() && result;
	_info.featuresGL.VERSION_4_4 = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_ARB_buffer_storage()
{
	bool result = IsExtSupported("GL_ARB_buffer_storage");
	if(!result)
		return false;
	INIT_FUNC_PTR(glBufferStorage);
	_info.featuresGL.ARB_buffer_storage = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_ARB_clear_texture()
{
	bool result = IsExtSupported("GL_ARB_clear_texture");
	if(!result)
		return false;
	INIT_FUNC_PTR(glClearTexImage);
	INIT_FUNC_PTR(glClearTexSubImage);
	_info.featuresGL.ARB_clear_texture = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_ARB_enhanced_layouts()
{
	_info.featuresGL.ARB_enhanced_layouts = IsExtSupported("GL_ARB_enhanced_layouts");
	return _info.featuresGL.ARB_enhanced_layouts;
}

bool GLRenderContext::glextLoad_GL_ARB_multi_bind()
{
	_info.featuresGL.ARB_multi_bind = IsExtSupported("GL_ARB_multi_bind");
	return _info.featuresGL.ARB_multi_bind;
}

bool GLRenderContext::glextLoad_GL_ARB_query_buffer_object()
{
	_info.featuresGL.ARB_query_buffer_object = IsExtSupported("GL_ARB_query_buffer_object");
	return _info.featuresGL.ARB_query_buffer_object;
}

bool GLRenderContext::glextLoad_GL_ARB_texture_mirror_clamp_to_edge()
{
	_info.featuresGL.ARB_texture_mirror_clamp_to_edge = IsExtSupported("GL_ARB_texture_mirror_clamp_to_edge");
	return _info.featuresGL.ARB_texture_mirror_clamp_to_edge;
}

bool GLRenderContext::glextLoad_GL_ARB_texture_stencil8()
{
	_info.featuresGL.ARB_texture_stencil8 = IsExtSupported("GL_ARB_texture_stencil8");
	return _info.featuresGL.ARB_texture_stencil8;
}

bool GLRenderContext::glextLoad_GL_ARB_vertex_type_10f_11f_11f_rev()
{
	_info.featuresGL.ARB_vertex_type_10f_11f_11f_rev = IsExtSupported("GL_ARB_vertex_type_10f_11f_11f_rev");
	return _info.featuresGL.ARB_vertex_type_10f_11f_11f_rev;
}

bool GLRenderContext::glextLoad_GL_VERSION_4_5()
{
	bool result = true;
	result = glextLoad_GL_ARB_clip_control() && result;
	result = glextLoad_GL_ARB_conditional_render_inverted() && result;
	result = glextLoad_GL_ARB_cull_distance() && result;
	result = glextLoad_GL_ARB_direct_state_access() && result;
	result = glextLoad_GL_ARB_ES3_1_compatibility() && result;
	result = glextLoad_GL_ARB_get_texture_sub_image() && result;
	result = glextLoad_GL_ARB_robustness() && result;
	result = glextLoad_GL_ARB_texture_barrier() && result;
	result = glextLoad_GL_KHR_context_flush_control() && result;
	_info.featuresGL.VERSION_4_5 = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_ARB_clip_control()
{
	bool result = IsExtSupported("GL_ARB_clip_control");
	if(!result)
		return false;
	INIT_FUNC_PTR(glClipControl);
	_info.featuresGL.ARB_clip_control = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_ARB_conditional_render_inverted()
{
	_info.featuresGL.ARB_conditional_render_inverted = IsExtSupported("GL_ARB_conditional_render_inverted");
	return _info.featuresGL.ARB_conditional_render_inverted;
}

bool GLRenderContext::glextLoad_GL_ARB_cull_distance()
{
	_info.featuresGL.ARB_cull_distance = IsExtSupported("GL_ARB_cull_distance");
	return _info.featuresGL.ARB_cull_distance;
}

bool GLRenderContext::glextLoad_GL_ARB_direct_state_access()
{
	_info.featuresGL.ARB_direct_state_access = IsExtSupported("GL_ARB_direct_state_access");
	return _info.featuresGL.ARB_direct_state_access;
}

bool GLRenderContext::glextLoad_GL_ARB_ES3_1_compatibility()
{
	bool result = IsExtSupported("GL_ARB_ES3_1_compatibility");
	if(!result)
		return false;
	INIT_FUNC_PTR(glMemoryBarrierByRegion);
	_info.featuresGL.ARB_ES3_1_compatibility = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_ARB_get_texture_sub_image()
{
	bool result = IsExtSupported("GL_ARB_get_texture_sub_image");
	if(!result)
		return false;
	INIT_FUNC_PTR(glGetTextureSubImage);
	INIT_FUNC_PTR(glGetCompressedTextureSubImage);
	_info.featuresGL.ARB_get_texture_sub_image = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_ARB_robustness()
{
	_info.featuresGL.ARB_robustness = IsExtSupported("GL_ARB_robustness");
	return _info.featuresGL.ARB_robustness;
}

bool GLRenderContext::glextLoad_GL_ARB_texture_barrier()
{
	bool result = IsExtSupported("GL_ARB_texture_barrier");
	if(!result)
		return false;
	INIT_FUNC_PTR(glTextureBarrier);
	_info.featuresGL.ARB_texture_barrier = result;
	return result;
}

bool GLRenderContext::glextLoad_GL_KHR_context_flush_control()
{
	_info.featuresGL.KHR_context_flush_control = IsExtSupported("GL_KHR_context_flush_control");
	return _info.featuresGL.KHR_context_flush_control;
}

bool GLRenderContext::glextLoad_GL_EXT_texture_filter_anisotropic()
{
	_info.featuresGL.EXT_texture_filter_anisotropic = IsExtSupported("GL_EXT_texture_filter_anisotropic");
	return _info.featuresGL.EXT_texture_filter_anisotropic;
}

bool GLRenderContext::glextLoad_GL_EXT_texture_sRGB()
{
	_info.featuresGL.EXT_texture_sRGB = IsExtSupported("GL_EXT_texture_sRGB");
	return _info.featuresGL.EXT_texture_sRGB;
}

bool GLRenderContext::glextLoad_GL_EXT_texture_snorm()
{
	_info.featuresGL.EXT_texture_snorm = IsExtSupported("GL_EXT_texture_snorm");
	return _info.featuresGL.EXT_texture_snorm;
}
