

#include <cassert>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <cstdio>
#include "GLRenderContext.h"


// Inclide definitions of extension loading functions.
// ===========================================================================

#define INIT_FUNC_PTR(func) \
	if (!InitFuncPtr(ptr_##func, #func)) { \
		result = false; \
		DebugMessage( \
			DebugMessageSource::ThirdParty, \
			DebugMessageType::Error, \
			DebugMessageSeverity::Notification, \
			ErrorMessageId::ExtensionMissingEntries, \
			#func); }



template <class _FUNCT>
inline
bool InitFuncPtr(_FUNCT& func_ptr, const char* func_name)
{
#if defined(_WIN32)
	func_ptr = (_FUNCT)wglGetProcAddress(func_name);

	if (func_ptr == nullptr)
	{
		static HMODULE hMod = LoadLibraryA("opengl32.dll");

		if (hMod == NULL)
			return false;

		func_ptr = reinterpret_cast<_FUNCT>(GetProcAddress(hMod, func_name));
	}
#elif defined(__linux__)
	func_ptr = (_FUNCT)glXGetProcAddress((const GLubyte*)func_name);
#endif
	return func_ptr != nullptr;
}


namespace gls::internals
{

#if defined(_WIN32)

	#include "extensions/glext_windows_load_def.inc"
	#include "extensions/wglext_load_def.inc"

#elif defined(__linux__)

	#include "extensions/glext_linux_load_def.inc"
	#include "extensions/glxext_load_def.inc"

#endif

// ===========================================================================

const PixelStore __defaultPixelStore;


void __SetPixelPackState(GLState* gl_state, const PixelStore* pixel_store)
{
	const PixelStore* ps = pixel_store? pixel_store: &__defaultPixelStore;
	PixelStore* cur_ps = &gl_state->pixelStorePack;

	if (ps->swapBytes != cur_ps->swapBytes)
	{
		glPixelStorei(GL_PACK_SWAP_BYTES, (GLint)ps->swapBytes);
		cur_ps->swapBytes = ps->swapBytes;
	}

	if (ps->lsbFirst != cur_ps->lsbFirst)
	{
		glPixelStorei(GL_PACK_LSB_FIRST, (GLint)ps->lsbFirst);
		cur_ps->lsbFirst = ps->lsbFirst;
	}

	if (ps->rowLength != cur_ps->rowLength)
	{
		glPixelStorei(GL_PACK_ROW_LENGTH, ps->rowLength);
		cur_ps->rowLength = ps->rowLength;
	}

	if (ps->skipRows != cur_ps->skipRows)
	{
		glPixelStorei(GL_PACK_SKIP_ROWS, ps->skipRows);
		cur_ps->skipRows = ps->skipRows;
	}

	if (ps->skipPixels != cur_ps->skipPixels)
	{
		glPixelStorei(GL_PACK_SKIP_PIXELS, ps->skipPixels);
		cur_ps->skipPixels = ps->skipPixels;
	}

	if (ps->alignment != cur_ps->alignment)
	{
		glPixelStorei(GL_PACK_ALIGNMENT, ps->alignment);
		cur_ps->alignment = ps->alignment;
	}

	if (ps->imageHeight != cur_ps->imageHeight)
	{
		glPixelStorei(GL_PACK_IMAGE_HEIGHT, ps->imageHeight);
		cur_ps->imageHeight = ps->imageHeight;
	}

	if (ps->skipImages != cur_ps->skipImages)
	{
		glPixelStorei(GL_PACK_SKIP_IMAGES, ps->skipImages);
		cur_ps->skipImages = ps->skipImages;
	}

	if (ps->compressedBlockWidth != cur_ps->compressedBlockWidth)
	{
		glPixelStorei(GL_PACK_COMPRESSED_BLOCK_WIDTH, ps->compressedBlockWidth);
		cur_ps->compressedBlockWidth = ps->compressedBlockWidth;
	}

	if (ps->compressedBlockHeight != cur_ps->compressedBlockHeight)
	{
		glPixelStorei(GL_PACK_COMPRESSED_BLOCK_HEIGHT, ps->compressedBlockHeight);
		cur_ps->compressedBlockHeight = ps->compressedBlockHeight;
	}

	if (ps->compressedBlockDepth != cur_ps->compressedBlockDepth)
	{
		glPixelStorei(GL_PACK_COMPRESSED_BLOCK_DEPTH, ps->compressedBlockDepth);
		cur_ps->compressedBlockDepth = ps->compressedBlockDepth;
	}

	if (ps->compressedBlockSize != cur_ps->compressedBlockSize)
	{
		glPixelStorei(GL_PACK_COMPRESSED_BLOCK_SIZE, ps->compressedBlockSize);
		cur_ps->compressedBlockSize = ps->compressedBlockSize;
	}
}

void __SetPixelUnpackState(GLState* gl_state, const PixelStore* pixel_store)
{
	const PixelStore* ps = pixel_store? pixel_store: &__defaultPixelStore;
	PixelStore* cur_ps = &gl_state->pixelStoreUnpack;

	if (ps->swapBytes != cur_ps->swapBytes)
	{
		glPixelStorei(GL_UNPACK_SWAP_BYTES, (GLint)ps->swapBytes);
		cur_ps->swapBytes = ps->swapBytes;
	}

	if (ps->lsbFirst != cur_ps->lsbFirst)
	{
		glPixelStorei(GL_UNPACK_LSB_FIRST, (GLint)ps->lsbFirst);
		cur_ps->lsbFirst = ps->lsbFirst;
	}

	if (ps->rowLength != cur_ps->rowLength)
	{
		glPixelStorei(GL_UNPACK_ROW_LENGTH, ps->rowLength);
		cur_ps->rowLength = ps->rowLength;
	}

	if (ps->skipRows != cur_ps->skipRows)
	{
		glPixelStorei(GL_UNPACK_SKIP_ROWS, ps->skipRows);
		cur_ps->skipRows = ps->skipRows;
	}

	if (ps->skipPixels != cur_ps->skipPixels)
	{
		glPixelStorei(GL_UNPACK_SKIP_PIXELS, ps->skipPixels);
		cur_ps->skipPixels = ps->skipPixels;
	}

	if (ps->alignment != cur_ps->alignment)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, ps->alignment);
		cur_ps->alignment = ps->alignment;
	}

	if (ps->imageHeight != cur_ps->imageHeight)
	{
		glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, ps->imageHeight);
		cur_ps->imageHeight = ps->imageHeight;
	}

	if (ps->skipImages != cur_ps->skipImages)
	{
		glPixelStorei(GL_UNPACK_SKIP_IMAGES, ps->skipImages);
		cur_ps->skipImages = ps->skipImages;
	}

	if (ps->compressedBlockWidth != cur_ps->compressedBlockWidth)
	{
		glPixelStorei(GL_UNPACK_COMPRESSED_BLOCK_WIDTH, ps->compressedBlockWidth);
		cur_ps->compressedBlockWidth = ps->compressedBlockWidth;
	}

	if (ps->compressedBlockHeight != cur_ps->compressedBlockHeight)
	{
		glPixelStorei(GL_UNPACK_COMPRESSED_BLOCK_HEIGHT, ps->compressedBlockHeight);
		cur_ps->compressedBlockHeight = ps->compressedBlockHeight;
	}

	if (ps->compressedBlockDepth != cur_ps->compressedBlockDepth)
	{
		glPixelStorei(GL_UNPACK_COMPRESSED_BLOCK_DEPTH, ps->compressedBlockDepth);
		cur_ps->compressedBlockDepth = ps->compressedBlockDepth;
	}

	if (ps->compressedBlockSize != cur_ps->compressedBlockSize)
	{
		glPixelStorei(GL_UNPACK_COMPRESSED_BLOCK_SIZE, ps->compressedBlockSize);
		cur_ps->compressedBlockSize = ps->compressedBlockSize;
	}
}


GLRenderContext::GLRenderContext(IDebugLogger* logger)
{
	Clear();
	_logger = logger? logger : &_nullLogger;
}

GLRenderContext::~GLRenderContext()
{
	
}

bool GLRenderContext::InitCommon(uint version)
{
	if (!LoadOpenGLExtensions(version))
	{
		Destroy();
		DebugMessage(DebugMessageSource::ThirdParty, DebugMessageType::Error, DebugMessageSeverity::High, ErrorMessageId::CreateContext, version, "not all required extensions are available");
		return false;
	}

	GetContextInfo();

	if (!_info.featuresGL.ARB_vertex_attrib_binding)
	{
		// We use these only if we don't have the ARB_vertex_attrib_binding extension to keep track of the current streams (buffer bindings) and attributes.
		_vertexStreams = new VertexStream[_info.maxVertexAttribBindings] { };
		_vertexAttribs = new VertexAttrib[_info.maxVertexAttribs] { };
	}

	_vertAttribEnabledState = new VertexAttribEnabledState[_info.maxVertexAttribs] { };
	_glState.imageUnits = new GLState::SamplerState[_info.maxCombinedTextureImageUnits] { };

	// OpenGL 3.0 and above requires a vertex array object to be bound.
	// We'll use a single VAO.
	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// We'll use a single program pipeline.
	glGenProgramPipelines(1, &_pipeline);
	glBindProgramPipeline(_pipeline);

	_initialized = true;

	return true;
}

void GLRenderContext::DeinitCommon()
{
	delete[] _vertexStreams;
	delete[] _vertexAttribs;
	delete[] _vertAttribEnabledState;
	delete[] _glState.imageUnits;
	Clear();
}

void GLRenderContext::GetContextInfo()
{
	_info.renderer = (const char*)glGetString(GL_RENDERER);
	_info.vendor = (const char*)glGetString(GL_VENDOR);
	_info.shadingLanguageVersion = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
	const char* version = (const char*)glGetString(GL_VERSION);
	_info.versionString = version;
	_info.versionMajor = atoi(version);
	_info.versionMinor = 0;
	const char* minor_ptr = strchr(version, '.');
	if (minor_ptr)
		_info.versionMinor = atoi(minor_ptr + 1);
	int ver_num = _info.versionMajor * 100 + _info.versionMinor * 10;

	glGetIntegerv(GL_MAX_ELEMENTS_VERTICES, &_info.maxRecomendedVertexCount);
	glGetIntegerv(GL_MAX_ELEMENTS_INDICES, &_info.maxRecomendedIndexCount);

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &_info.maxTextureSize);
	glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &_info.max3DTextureSize);
	glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE, &_info.maxCubeMapSize);
	glGetIntegerv(GL_MAX_RECTANGLE_TEXTURE_SIZE, &_info.maxRectangleTextureSize);
	glGetFloatv(GL_MAX_TEXTURE_LOD_BIAS, &_info.maxTextureLODBias);
	int vport_dims[2];
	glGetIntegerv(GL_MAX_VIEWPORT_DIMS, vport_dims);
	_info.maxViewportWidth = vport_dims[0];
	_info.maxViewportHeight = vport_dims[1];
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &_info.maxVertexAttribs);
	_info.maxVertexAttribBindings = _info.maxVertexAttribs; // maxVertexAttribBindings will be overwritten below if version is >= 430
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &_info.maxCombinedTextureImageUnits);
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &_info.maxTextureAnisotropy);
	glGetIntegerv(GL_MAX_DRAW_BUFFERS, &_info.maxDrawBuffers);
	glGetIntegerv(GL_MAX_TEXTURE_BUFFER_SIZE, &_info.maxTextureBufferSize);
	glGetIntegerv(GL_MAX_ELEMENTS_VERTICES, &_info.maxElementVertices);
	glGetIntegerv(GL_MAX_ELEMENTS_INDICES, &_info.maxElementIndices);
	glGetIntegerv(GL_SAMPLE_BUFFERS, &_info.numSampleBuffers);
	glGetIntegerv(GL_SAMPLES, &_info.numSamples);

	if (ver_num >= 300)
	{
		glGetIntegerv(GL_MAX_SAMPLES, &_info.maxSamples);
	}

	if (ver_num >= 310)
	{
		glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &_info.maxUniformBlockSize);
		glGetIntegerv(GL_MAX_COMBINED_UNIFORM_BLOCKS, &_info.maxCombinedUniformBlocks);
		glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &_info.uniformBufferOffsetAlignment);
	}

	if (ver_num >= 320)
	{
		glGetInteger64v(GL_MAX_SERVER_WAIT_TIMEOUT, &_info.maxServerWaitTimeout);
		glGetIntegerv(GL_MAX_SAMPLE_MASK_WORDS, &_info.maxSampleMaskWords);
		glGetIntegerv(GL_MAX_COLOR_TEXTURE_SAMPLES, &_info.maxColorTextureSamples);
		glGetIntegerv(GL_MAX_DEPTH_TEXTURE_SAMPLES, &_info.maxDepthTextureSamples);
		glGetIntegerv(GL_MAX_INTEGER_SAMPLES, &_info.maxIntegerSamples);
	}

	if (ver_num >= 400)
	{
		glGetIntegerv(GL_MAX_GEOMETRY_SHADER_INVOCATIONS, &_info.maxGeometryShaderInvocations);
		glGetIntegerv(GL_MIN_FRAGMENT_INTERPOLATION_OFFSET, &_info.minFragmentInterpolationOffset);
		glGetIntegerv(GL_MAX_FRAGMENT_INTERPOLATION_OFFSET, &_info.maxFragmentInterpolationOffset);
		glGetIntegerv(GL_FRAGMENT_INTERPOLATION_OFFSET_BITS, &_info.fragmentInterpolationOffsetBits);
		glGetIntegerv(GL_MAX_VERTEX_STREAMS, &_info.maxVertexStreams);
		glGetFloatv(GL_MIN_SAMPLE_SHADING_VALUE, &_info.minSampleShadingValue);
		glGetIntegerv(GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET, &_info.minTexGatherOffset);
		glGetIntegerv(GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET, &_info.maxTexGatherOffset);
		//glGetIntegerv(GL_MAX_PROGRAM_TEXTURE_GATHER_COMPONENTS, &_info.maxTexGatherComponents);
		glGetIntegerv(GL_MAX_TRANSFORM_FEEDBACK_BUFFERS, &_info.maxTransformFeedbackBuffers);
		glGetIntegerv(GL_MAX_SUBROUTINES, &_info.maxSubroutines);
		glGetIntegerv(GL_MAX_SUBROUTINE_UNIFORM_LOCATIONS, &_info.maxSubroutineUniformLocations);
		glGetIntegerv(GL_MAX_PATCH_VERTICES, &_info.maxPatchVertices);
		glGetIntegerv(GL_MAX_TESS_GEN_LEVEL, &_info.maxTessGenLevel);
		glGetIntegerv(GL_MAX_TESS_CONTROL_UNIFORM_COMPONENTS, &_info.maxTessControlUniformComponents);
		glGetIntegerv(GL_MAX_TESS_EVALUATION_UNIFORM_COMPONENTS, &_info.maxTessEvaluationUniformComponents);
		glGetIntegerv(GL_MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS, &_info.maxTessControlTextureImageUnits);
		glGetIntegerv(GL_MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS, &_info.maxTessEvaluationTextureImageUnits);
		glGetIntegerv(GL_MAX_TESS_CONTROL_OUTPUT_COMPONENTS, &_info.maxTessControlOutputComponents);
		glGetIntegerv(GL_MAX_TESS_PATCH_COMPONENTS, &_info.maxTessPatchComponents);
		glGetIntegerv(GL_MAX_TESS_CONTROL_TOTAL_OUTPUT_COMPONENTS, &_info.maxTessControlTotalOutputComponents);
		glGetIntegerv(GL_MAX_TESS_EVALUATION_OUTPUT_COMPONENTS, &_info.maxTessEvaluationOutputComponents);
		glGetIntegerv(GL_MAX_TESS_CONTROL_UNIFORM_BLOCKS, &_info.maxTessUniformBlocks);
		glGetIntegerv(GL_MAX_TESS_EVALUATION_UNIFORM_BLOCKS, &_info.maxTessEvaluationUniformBlocks);
		glGetIntegerv(GL_MAX_TESS_CONTROL_INPUT_COMPONENTS, &_info.maxTessControlInputComponents);
		glGetIntegerv(GL_MAX_TESS_EVALUATION_INPUT_COMPONENTS, &_info.maxTessEvaluationInputComponents);
		glGetIntegerv(GL_MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS, &_info.maxCombinedTessControlUniformComponents);
		glGetIntegerv(GL_MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS, &_info.maxCombinedTessEvaluationUniformComponents);
	}

	// Mandatory 410 implementation dependent state.
	glGetIntegerv(GL_MAX_VIEWPORTS, &_info.maxViewports);
	glGetIntegerv(GL_VIEWPORT_SUBPIXEL_BITS, &_info.viewportSubpixelBits);
	glGetFloatv(GL_VIEWPORT_BOUNDS_RANGE, _info.viewportBoundsRange);

	if (ver_num >= 420)
	{
		glGetIntegerv(GL_MIN_MAP_BUFFER_ALIGNMENT, &_info.minMapBufferAlignment);
		glGetIntegerv(GL_MAX_VERTEX_ATOMIC_COUNTER_BUFFERS, &_info.maxVertexAtomicCounterBuffers);
		glGetIntegerv(GL_MAX_TESS_CONTROL_ATOMIC_COUNTER_BUFFERS, &_info.maxTessControlAtomicCounterBuffers);
		glGetIntegerv(GL_MAX_TESS_EVALUATION_ATOMIC_COUNTER_BUFFERS, &_info.maxTessEvaluationAtomicCounterBuffers);
		glGetIntegerv(GL_MAX_GEOMETRY_ATOMIC_COUNTER_BUFFERS, &_info.maxGeometryAtomicCounterBuffers);
		glGetIntegerv(GL_MAX_FRAGMENT_ATOMIC_COUNTER_BUFFERS, &_info.maxFragmentAtomicCounterBuffers);
		glGetIntegerv(GL_MAX_COMBINED_ATOMIC_COUNTER_BUFFERS, &_info.maxCombinedAtomicCounterBuffers);
		glGetIntegerv(GL_MAX_VERTEX_ATOMIC_COUNTERS, &_info.maxVertexAtomicCounters);
		glGetIntegerv(GL_MAX_TESS_CONTROL_ATOMIC_COUNTERS, &_info.maxTessControlAtomicCounters);
		glGetIntegerv(GL_MAX_TESS_EVALUATION_ATOMIC_COUNTERS, &_info.maxTessEvaluationAtomicCounters);
		glGetIntegerv(GL_MAX_GEOMETRY_ATOMIC_COUNTERS, &_info.maxGeometryAtomicCounters);
		glGetIntegerv(GL_MAX_FRAGMENT_ATOMIC_COUNTERS, &_info.maxFragmentAtomicCounters);
		glGetIntegerv(GL_MAX_COMBINED_ATOMIC_COUNTERS, &_info.maxCombinedAtomicCounters);
		glGetIntegerv(GL_MAX_ATOMIC_COUNTER_BUFFER_SIZE, &_info.maxAtomicCounterBufferSize);
		glGetIntegerv(GL_MAX_ATOMIC_COUNTER_BUFFER_BINDINGS, &_info.maxAtomicCounterBufferBindings);
		glGetIntegerv(GL_MAX_IMAGE_UNITS, &_info.maxImageUnits);
		glGetIntegerv(GL_MAX_COMBINED_IMAGE_UNITS_AND_FRAGMENT_OUTPUTS, &_info.maxCombinedShaderOutputResources);
		glGetIntegerv(GL_MAX_IMAGE_SAMPLES, &_info.maxImageSamplers);
		glGetIntegerv(GL_MAX_VERTEX_IMAGE_UNIFORMS, &_info.maxVertexImageUniforms);
		glGetIntegerv(GL_MAX_TESS_CONTROL_IMAGE_UNIFORMS, &_info.maxTessControlImageUniforms);
		glGetIntegerv(GL_MAX_TESS_EVALUATION_IMAGE_UNIFORMS, &_info.maxTessEvaluationImageUniforms);
		glGetIntegerv(GL_MAX_GEOMETRY_IMAGE_UNIFORMS, &_info.maxGeometryImageUniforms);
		glGetIntegerv(GL_MAX_FRAGMENT_IMAGE_UNIFORMS, &_info.maxFragmentImageUniforms);
		glGetIntegerv(GL_MAX_COMBINED_IMAGE_UNIFORMS, &_info.maxCombinedImageUniforms);
	}

	if (ver_num >= 430)
	{
		glGetIntegerv(GL_MAX_COMPUTE_UNIFORM_BLOCKS, &_info.maxComputeUniformBlocks);
		glGetIntegerv(GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS, &_info.maxComputeTextureImageUnits);
		glGetIntegerv(GL_MAX_COMPUTE_IMAGE_UNIFORMS, &_info.maxComputeImageUniforms);
		glGetIntegerv(GL_MAX_COMPUTE_SHARED_MEMORY_SIZE, &_info.maxComputeSharedMemorySize);
		glGetIntegerv(GL_MAX_COMPUTE_UNIFORM_COMPONENTS, &_info.maxComputeUniformComponents);
		glGetIntegerv(GL_MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS, &_info.maxComputeAtomicCounterBuffers);
		glGetIntegerv(GL_MAX_COMPUTE_ATOMIC_COUNTERS, &_info.maxComputeAtomicCounters);
		glGetIntegerv(GL_MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS, &_info.maxCombinedComputeUniformComponents);
		//glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &_info.maxComputeWorkGroupInvocations);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &_info.maxComputeWorkGroupCountX);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &_info.maxComputeWorkGroupCountY);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &_info.maxComputeWorkGroupCountZ);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &_info.maxComputeWorkGroupSizeX);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &_info.maxComputeWorkGroupSizeY);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &_info.maxComputeWorkGroupSizeZ);
		glGetIntegerv(GL_MAX_FRAMEBUFFER_WIDTH, &_info.maxFramebufferWidth);
		glGetIntegerv(GL_MAX_FRAMEBUFFER_HEIGHT, &_info.maxFramebufferHeight);
		glGetIntegerv(GL_MAX_FRAMEBUFFER_LAYERS, &_info.maxFramebufferLayers);
		glGetIntegerv(GL_MAX_FRAMEBUFFER_SAMPLES, &_info.maxFramebufferSamples);
		glGetIntegerv(GL_MAX_VERTEX_SHADER_STORAGE_BLOCKS, &_info.maxVertexShaderStorageBlocks);
		glGetIntegerv(GL_MAX_GEOMETRY_SHADER_STORAGE_BLOCKS, &_info.maxGeometryShaderStorageBlocks);
		glGetIntegerv(GL_MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS, &_info.maxTessControlShaderStorageBlocks);
		glGetIntegerv(GL_MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS, &_info.maxTessEvaluationShaderStorageBlocks);
		glGetIntegerv(GL_MAX_FRAGMENT_SHADER_STORAGE_BLOCKS, &_info.maxFragmentShaderStorageBlocks);
		glGetIntegerv(GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS, &_info.maxComputeShaderStorageBlocks);
		glGetIntegerv(GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS, &_info.maxCombinedShaderStorageBlocks);
		glGetIntegerv(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS, &_info.maxShaderStorageBufferBindings);
		glGetIntegerv(GL_MAX_SHADER_STORAGE_BLOCK_SIZE, &_info.maxShaderStorageBlockSize);
		glGetIntegerv(GL_SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT, &_info.shaderStorageBufferOffsetAlignment);
		glGetIntegerv(GL_TEXTURE_BUFFER_OFFSET_ALIGNMENT, &_info.textureBufferOffsetAlignment);
		glGetIntegerv(GL_MAX_VERTEX_ATTRIB_BINDINGS, &_info.maxVertexAttribBindings);
		glGetIntegerv(GL_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET, &_info.maxVertexAttribRelativeOffset);
	}
	// Mandatory 430 implementation dependent state.
	glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, &_info.maxUniformLocations);
	glGetIntegerv(GL_MAX_DEBUG_MESSAGE_LENGTH, &_info.maxDebugMessageLength);
	glGetIntegerv(GL_MAX_DEBUG_LOGGED_MESSAGES, &_info.maxDebugLoggedMessages);
	glGetIntegerv(GL_MAX_DEBUG_GROUP_STACK_DEPTH, &_info.maxDebugGroupStackDepth);
	glGetIntegerv(GL_MAX_LABEL_LENGTH, &_info.maxLabelLength);
	
	if (ver_num >= 450)
	{
		glGetIntegerv(GL_MAX_CULL_DISTANCES, &_info.maxCullDistances);
		glGetIntegerv(GL_MAX_COMBINED_CLIP_AND_CULL_DISTANCES, &_info.maxCombinedClipAndCullDistances);
	}
}

bool GLRenderContext::LoadOpenGLExtensions(uint version)
{
	bool result = true;

	LOAD_EXTENSION_REQ(GL_VERSION_1_0);
	LOAD_EXTENSION_REQ(GL_VERSION_1_1);
	LOAD_EXTENSION_REQ(GL_VERSION_1_2);
	LOAD_EXTENSION_REQ(GL_VERSION_1_3);
	LOAD_EXTENSION_REQ(GL_VERSION_1_4);
	LOAD_EXTENSION_REQ(GL_VERSION_1_5);
	LOAD_EXTENSION_REQ(GL_VERSION_2_0);
	LOAD_EXTENSION_REQ(GL_VERSION_2_1);
	LOAD_EXTENSION_REQ(GL_VERSION_3_0);
	LOAD_EXTENSION_REQ(GL_VERSION_3_1);
	LOAD_EXTENSION_REQ(GL_VERSION_3_2);
	LOAD_EXTENSION_REQ(GL_VERSION_3_3);
	LOAD_EXTENSION_REQ(GL_EXT_texture_sRGB);
	LOAD_EXTENSION_REQ(GL_EXT_texture_filter_anisotropic);
//	LOAD_EXTENSION(GL_EXT_texture_snorm);

	if (version >= 400)
	{
		LOAD_EXTENSION_REQ(GL_VERSION_4_0);
	}

	if (version >= 410)
	{
		LOAD_EXTENSION_REQ(GL_VERSION_4_1);
	}
	else
	{
		LOAD_EXTENSION_REQ(GL_ARB_separate_shader_objects);
		LOAD_EXTENSION_REQ(GL_ARB_viewport_array);
	}

	if (version >= 420)
	{
		LOAD_EXTENSION_REQ(GL_VERSION_4_2);
	}
	else
	{
		LOAD_EXTENSION_REQ(GL_ARB_base_instance);
		LOAD_EXTENSION_REQ(GL_ARB_texture_storage);
	}

	if (version >= 430)
	{
		LOAD_EXTENSION_REQ(GL_VERSION_4_3);
	}
	else
	{
		LOAD_EXTENSION(GL_ARB_explicit_uniform_location);
		LOAD_EXTENSION(GL_KHR_debug);
	}

	if (version >= 440)
	{
		LOAD_EXTENSION_REQ(GL_VERSION_4_4);
	}
	else
	{
		LOAD_EXTENSION_REQ(GL_ARB_buffer_storage);
	}

	if (version >= 450)
	{
		LOAD_EXTENSION_REQ(GL_VERSION_4_5);
	}

	result = LoadPlatformOpenGLExtensions() && result;

	return result;
}

void GLRenderContext::Clear()
{
#if defined(_WIN32)
	_hwnd = 0;
	_hglrc = 0;
	_hdc = 0;
	_instanceHandle = 0;
#elif defined(__linux__)
	_display = nullptr;
	_window = 0;
	_context = 0;
#endif
	_info = { };
	_initialized = false;

	_glState = { };
	_glState.pixelStorePack = __defaultPixelStore;
	_glState.pixelStoreUnpack = __defaultPixelStore;
	_vertexStreams = nullptr;
	_vertexFormat = nullptr;
	_vertexAttribs = nullptr;
	_vertAttribEnabledState = nullptr;
	_indexType = DataType::UnsignedShort;
	_pipeline = 0;
	_unit0Texture = nullptr;
	_framebuffer = nullptr;
	_logger = nullptr;
}

const ContextInfo& GLRenderContext::GetInfo() const
{
	return _info;
}

void GLRenderContext::VertexSource(int stream, IBuffer* buffer, sizei stride, intptr offset, uint divisor)
{
	if (_info.featuresGL.ARB_vertex_attrib_binding)
	{
		glBindVertexBuffer(stream, dyn_cast_ptr<GLBuffer*>(buffer)->GetID(), offset, stride);

		// Set the divisor for this stream.
		glVertexBindingDivisor(stream, divisor);
	}
	else
	{
		assert(stream >= 0 && stream < _info.maxVertexAttribBindings);
		_vertexStreams[stream].buffer = dyn_cast_ptr<GLBuffer*>(buffer);
		_vertexStreams[stream].stride = stride;
		_vertexStreams[stream].offset = offset;
		_vertexStreams[stream].divisor = divisor;
	}
}

void GLRenderContext::IndexSource(IBuffer* buffer, DataType index_type)
{
	GLBuffer* buf = dyn_cast_ptr<GLBuffer*>(buffer);
	GLuint bufId = (buf != nullptr) ? buf->GetID() : 0;

	if (bufId != _glState.indexBuf)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufId);
		_glState.indexBuf = bufId;
	}

	_indexType = index_type;
}

void GLRenderContext::ActiveVertexFormat(IVertexFormat* format)
{
	_vertexFormat = static_cast<GLVertexFormat*>(format);

	if (_info.featuresGL.ARB_vertex_attrib_binding)
	{
		if (_vertexFormat != nullptr)
		{
			int descCount = _vertexFormat->GetDescriptorCount();
			const VertexAttribDesc* descriptors = _vertexFormat->GetDescriptors();

			for (int i = 0; i < descCount; ++i)
			{
				const VertexAttribDesc& desc = descriptors[i];
				assert((int)desc.attribute < _info.maxVertexAttribs);
				assert(desc.stream >= 0 && desc.stream < _info.maxVertexAttribBindings);

				if (desc.type == DataType::Double)
				{
					glVertexAttribLFormat(desc.attribute, desc.numComponents, GetGLEnum(desc.type), (GLuint)desc.offset);
				}
				else
				{
					if (desc.integer)
						glVertexAttribIFormat(desc.attribute, desc.numComponents, GetGLEnum(desc.type), (GLuint)desc.offset);
					else
						glVertexAttribFormat(desc.attribute, desc.numComponents, GetGLEnum(desc.type), desc.normalized, (GLuint)desc.offset);
				}

				// Bind this attribute index to the index of the stream from which it gets the data.
				glVertexAttribBinding(desc.attribute, desc.stream);

				_vertAttribEnabledState[desc.attribute].newState = true;	// Set this flag for the next loop.
			}
		}

		for (int i = 0; i < _info.maxVertexAttribs; ++i)
		{
			if (_vertAttribEnabledState[i].newState == true)
			{
				if (_vertAttribEnabledState[i].currentState == false)
				{
					glEnableVertexAttribArray(i);
					_vertAttribEnabledState[i].currentState = true;
				}
				_vertAttribEnabledState[i].newState = false;	// Reset this flag after changing the state.
			}
			else
			{
				if (_vertAttribEnabledState[i].currentState == true)
				{
					glDisableVertexAttribArray(i);
					_vertAttribEnabledState[i].currentState = false;
				}
			}
		}
	}
}

void GLRenderContext::EnablePrimitiveRestart(bool enable)
{
	if (enable)
		glEnable(GL_PRIMITIVE_RESTART);
	else
		glDisable(GL_PRIMITIVE_RESTART);
}

void GLRenderContext::EnablePrimitiveRestartFixedIndex(bool enable)
{
	if (enable)
		glEnable(GL_PRIMITIVE_RESTART_FIXED_INDEX);
	else
		glDisable(GL_PRIMITIVE_RESTART_FIXED_INDEX);
}

void GLRenderContext::PrimitiveRestartIndex(uint index)
{
	glPrimitiveRestartIndex(index);
}

void GLRenderContext::ProvokingVertex(VertexConvention vertex_convention)
{
	glProvokingVertex(GetGLEnum(vertex_convention));
}

void GLRenderContext::PatchVertexCount(int count)
{
	glPatchParameteri(GL_PATCH_VERTICES, count);
}

void GLRenderContext::PatchDefaultOuterLevels(const float values[4])
{
	glPatchParameterfv(GL_PATCH_DEFAULT_OUTER_LEVEL, values);
}

void GLRenderContext::PatchDefaultInnerLevels(const float values[2])
{
	glPatchParameterfv(GL_PATCH_DEFAULT_INNER_LEVEL, values);
}

void GLRenderContext::BeginConditionalRender(IQuery* query, ConditionalRenderQueryMode mode)
{
	glBeginConditionalRender(static_cast<GLQuery*>(query)->GetID(), GetGLEnum(mode));
}

void GLRenderContext::EndConditionalRender()
{
	glEndConditionalRender();
}

/*
	primitive must be one of PRIM_POINTS, PRIM_LINES or PRIM_TRIANGLES
*/
void GLRenderContext::BeginTransformFeedback(PrimitiveType primitive, ITransformFeedback* transform_feedback)
{
	GLuint tf_id = dyn_cast_ptr<GLTransformFeedback*>(transform_feedback)->GetID();
	if (tf_id != _glState.transformFeedback)
	{
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, tf_id);
		_glState.transformFeedback = tf_id;
	}

	glBeginTransformFeedback(GetGLEnum(primitive));
}

void GLRenderContext::EndTransformFeedback()
{
	glEndTransformFeedback();
}

void GLRenderContext::PauseTransformFeedback()
{
	glPauseTransformFeedback();
}

void GLRenderContext::ResumeTransformFeedback()
{
	glResumeTransformFeedback();
}

void GLRenderContext::Viewport(int x, int y, int width, int height)
{
	glViewport(x, y, width, height);
}

void GLRenderContext::ViewportIndexed(uint index, float x, float y, float width, float height)
{
	glViewportIndexedf(index, x, y, width, height);
}

void GLRenderContext::ClipControl(ClipOrigin origin, ClipDepth depth)
{
	glClipControl(GetGLEnum(origin), GetGLEnum(depth));
}

void GLRenderContext::EnableFaceCulling(bool enable)
{
	if (enable)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
}

void GLRenderContext::CullFace(PolygonFace face)
{
	glCullFace(GetGLEnum(face));
}

void GLRenderContext::FrontFace(VertexWinding orient)
{
	glFrontFace(GetGLEnum(orient));
}

void GLRenderContext::RasterizationMode(RasterMode mode)
{
	glPolygonMode(GL_FRONT_AND_BACK, GetGLEnum(mode));
}

void GLRenderContext::EnableRasterizerDiscard(bool enable)
{
	if (enable)
		glEnable(GL_RASTERIZER_DISCARD);
	else
		glDisable(GL_RASTERIZER_DISCARD);
}

void GLRenderContext::LineWidth(float width)
{
	glLineWidth(width);
}

void GLRenderContext::EnableLineAntialiasing(bool enable)
{
	if (enable)
		glEnable(GL_LINE_SMOOTH);
	else
		glDisable(GL_LINE_SMOOTH);
}

void GLRenderContext::EnableMultisampling(bool enable)
{
	if (enable)
		glEnable(GL_MULTISAMPLE);
	else
		glDisable(GL_MULTISAMPLE);
}

void GLRenderContext::EnableSampleAlphaToCoverage(bool enable)
{
	if (enable)
		glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
	else
		glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
}

void GLRenderContext::EnableSampleAlphaToOne(bool enable)
{
	if (enable)
		glEnable(GL_SAMPLE_ALPHA_TO_ONE);
	else
		glDisable(GL_SAMPLE_ALPHA_TO_ONE);
}

void GLRenderContext::EnableSampleCoverage(bool enable)
{
	if (enable)
		glEnable(GL_SAMPLE_COVERAGE);
	else
		glDisable(GL_SAMPLE_COVERAGE);
}

void GLRenderContext::EnableSampleShading(bool enable)
{
	if (enable)
		glEnable(GL_SAMPLE_SHADING);
	else
		glDisable(GL_SAMPLE_SHADING);
}

void GLRenderContext::SampleCoverage(float value, bool invert)
{
	glSampleCoverage(value, invert);
}

void GLRenderContext::SampleMask(uint index, uint mask)
{
	assert(index < (uint)_info.maxSampleMaskWords);
	glSampleMaski(index, mask);
}

void GLRenderContext::GetSamplePosition(uint index, float position[2])
{
	assert(index < (uint)_info.numSamples);
	glGetMultisamplefv(GL_SAMPLE_POSITION, index, position);
}

void GLRenderContext::MinSampleShading(float value)
{
	glMinSampleShading(value);
}

void GLRenderContext::EnableScissorTest(bool enable)
{
	if (enable)
		glEnable(GL_SCISSOR_TEST);
	else
		glDisable(GL_SCISSOR_TEST);
}

void GLRenderContext::EnableScissorTestIndexed(uint index, bool enable)
{
	if (enable)
		glEnablei(GL_SCISSOR_TEST, index);
	else
		glDisablei(GL_SCISSOR_TEST, index);
}

void GLRenderContext::Scissor(int x, int y, int width, int height)
{
	glScissor(x, y, width, height);
}

void GLRenderContext::ScissorIndexed(uint index, int x, int y, int width, int height)
{
	glScissorIndexed(index, x, y, width, height);
}

void GLRenderContext::EnableDepthTest(bool enable)
{
	if (enable)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
}

void GLRenderContext::DepthTestFunc(CompareFunc func)
{
	glDepthFunc(GetGLEnum(func));
}

void GLRenderContext::DepthRange(float dnear, float dfar)
{
	glDepthRange(dnear, dfar);
}

void GLRenderContext::DepthRangeIndexed(uint index, float dnear, float dfar)
{
	glDepthRangeIndexed(index, dnear, dfar);
}

void GLRenderContext::DepthOffset(float factor, float units)
{
	glPolygonOffset(factor, units);
}

void GLRenderContext::EnableDepthClamp(bool enable)
{
	if (enable)
		glEnable(GL_DEPTH_CLAMP);
	else
		glDisable(GL_DEPTH_CLAMP);
}

void GLRenderContext::EnableStencilTest(bool enable)
{
	if (enable)
		glEnable(GL_STENCIL_TEST);
	else
		glDisable(GL_STENCIL_TEST);
}

void GLRenderContext::StencilTestFunc(PolygonFace face, CompareFunc func, int ref, uint mask)
{
	glStencilFuncSeparate(GetGLEnum(face), GetGLEnum(func), ref, mask);
}

void GLRenderContext::StencilOperation(PolygonFace face, StencilOp stencil_fail, StencilOp depth_fail, StencilOp depth_pass)
{
	glStencilOpSeparate(GetGLEnum(face), GetGLEnum(stencil_fail), GetGLEnum(depth_fail), GetGLEnum(depth_pass));
}

void GLRenderContext::EnableBlending(bool enable)
{
	if (enable)
		glEnable(GL_BLEND);
	else
		glDisable(GL_BLEND);
}

void GLRenderContext::EnableBlending(uint buffer, bool enable)
{
	if (enable)
		glEnablei(GL_BLEND, buffer);
	else
		glDisablei(GL_BLEND, buffer);
}

void GLRenderContext::BlendingColor(const float color[4])
{
	glBlendColor(color[0], color[1], color[2], color[3]);
}

void GLRenderContext::BlendingFunc(BlendFunc src_factor, BlendFunc dest_factor)
{
	glBlendFunc(GetGLEnum(src_factor), GetGLEnum(dest_factor));
}

void GLRenderContext::BlendingFunc(BlendFunc src_rgb_factor, BlendFunc dest_rgb_factor, BlendFunc src_alpha_factor, BlendFunc dest_alpha_factor)
{
	glBlendFuncSeparate(GetGLEnum(src_rgb_factor), GetGLEnum(dest_rgb_factor), GetGLEnum(src_alpha_factor), GetGLEnum(dest_alpha_factor));
}

void GLRenderContext::BlendingFunc(uint buffer, BlendFunc src_factor, BlendFunc dest_factor)
{
	glBlendFunci(buffer, GetGLEnum(src_factor), GetGLEnum(dest_factor));
}

void GLRenderContext::BlendingFunc(uint buffer, BlendFunc src_rgb_factor, BlendFunc dest_rgb_factor, BlendFunc src_alpha_factor, BlendFunc dest_alpha_factor)
{
	glBlendFuncSeparatei(buffer, GetGLEnum(src_rgb_factor), GetGLEnum(dest_rgb_factor), GetGLEnum(src_alpha_factor), GetGLEnum(dest_alpha_factor));
}

void GLRenderContext::BlendingOperation(BlendOp op)
{
	glBlendEquation(GetGLEnum(op));
}

void GLRenderContext::BlendingOperation(BlendOp op_rgb, BlendOp op_alpha)
{
	glBlendEquationSeparate(GetGLEnum(op_rgb), GetGLEnum(op_alpha));
}

void GLRenderContext::BlendingOperation(uint buffer, BlendOp op)
{
	glBlendEquationi(buffer, GetGLEnum(op));
}

void GLRenderContext::BlendingOperation(uint buffer, BlendOp op_rgb, BlendOp op_alpha)
{
	glBlendEquationSeparatei(buffer, GetGLEnum(op_rgb), GetGLEnum(op_alpha));
}

void GLRenderContext::EnableLogicOperation(bool enable)
{
	if (enable)
		glEnable(GL_COLOR_LOGIC_OP);
	else
		glDisable(GL_COLOR_LOGIC_OP);
}

void GLRenderContext::LogicOperation(LogicOp op)
{
	glLogicOp(GetGLEnum(op));
}

/*
	if fbuf is 0, main framebuffer is used
*/
void GLRenderContext::SetFramebuffer(IFramebuffer* fbuf)
{
	GLFramebuffer* framebuffer = dyn_cast_ptr<GLFramebuffer*>(fbuf);
	GLuint fbufId = framebuffer ? framebuffer->GetID() : 0;

	if (fbufId != _glState.drawFbuf)
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbufId);
		_glState.drawFbuf = fbufId;
	}

	// Remember currently set framebuffer, in case a different one is bound before drawing.
	// If so, DelayedDrawingStateSetup() will bind it again before the draw call.
	_framebuffer = framebuffer;
}

void GLRenderContext::ActiveColorBuffers(IFramebuffer* fbuf, const ColorBuffer* buffers, sizei count)
{
	if (count < 0 || count > _info.maxDrawBuffers)
	{
		assert(false);
		return;
	}

	GLuint id = fbuf ? dyn_cast_ptr<GLFramebuffer*>(fbuf)->GetID() : 0;
	if (id != _glState.drawFbuf)
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, id);
		_glState.drawFbuf = id;
	}

	if (count > 0 && buffers != nullptr)
	{
		GLenum enums[32];
		for (sizei i = 0; i < count; ++i)
			enums[i] = GetGLEnum(buffers[i]);

		glDrawBuffers(count, enums);
	}
	else
	{
		GLenum enums[] = { GL_NONE };
		glDrawBuffers(1, enums);
	}
}

void GLRenderContext::EnableFramebufferSRGB(bool enable)
{
	if (enable)
		glEnable(GL_FRAMEBUFFER_SRGB);
	else
		glDisable(GL_FRAMEBUFFER_SRGB);
}

/*
	sets color writing for all draw buffers
*/
void GLRenderContext::EnableColorWrite(bool r, bool g, bool b, bool a)
{
	glColorMask(r, g, b, a);
}

/*
	sets color writing for specified draw buffer
*/
void GLRenderContext::EnableColorWrite(uint buffer, bool r, bool g, bool b, bool a)
{
	glColorMaski(buffer, r, g, b, a);
}

void GLRenderContext::EnableDepthWrite(bool enable)
{
	glDepthMask(enable);
}

void GLRenderContext::EnableStencilWrite(PolygonFace face, uint mask)
{
	glStencilMaskSeparate(GetGLEnum(face), mask);
}

void GLRenderContext::ClearColorBuffer(IFramebuffer* fbuf, uint buffer, const float color[4])
{
	assert(buffer < (uint)_info.maxDrawBuffers);

	GLuint id = fbuf ? dyn_cast_ptr<GLFramebuffer*>(fbuf)->GetID() : 0;
	if (id != _glState.drawFbuf)
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, id);
		_glState.drawFbuf = id;
	}

	glClearBufferfv(GL_COLOR, buffer, color);
}

void GLRenderContext::ClearColorBuffer(IFramebuffer* fbuf, uint buffer, const int color[4])
{
	assert(buffer < (uint)_info.maxDrawBuffers);

	GLuint id = fbuf ? dyn_cast_ptr<GLFramebuffer*>(fbuf)->GetID() : 0;
	if (id != _glState.drawFbuf)
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, id);
		_glState.drawFbuf = id;
	}

	glClearBufferiv(GL_COLOR, buffer, color);
}

void GLRenderContext::ClearColorBuffer(IFramebuffer* fbuf, uint buffer, const uint color[4])
{
	assert(buffer < (uint)_info.maxDrawBuffers);

	GLuint id = fbuf ? dyn_cast_ptr<GLFramebuffer*>(fbuf)->GetID() : 0;
	if (id != _glState.drawFbuf)
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, id);
		_glState.drawFbuf = id;
	}

	glClearBufferuiv(GL_COLOR, buffer, color);
}

void GLRenderContext::ClearDepthBuffer(IFramebuffer* fbuf, float depth)
{
	GLuint id = fbuf ? dyn_cast_ptr<GLFramebuffer*>(fbuf)->GetID() : 0;
	if (id != _glState.drawFbuf)
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, id);
		_glState.drawFbuf = id;
	}

	glClearBufferfv(GL_DEPTH, 0, &depth);
}

void GLRenderContext::ClearStencilBuffer(IFramebuffer* fbuf, int stencil)
{
	GLuint id = fbuf ? dyn_cast_ptr<GLFramebuffer*>(fbuf)->GetID() : 0;
	if (id != _glState.drawFbuf)
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, id);
		_glState.drawFbuf = id;
	}

	glClearBufferiv(GL_STENCIL, 0, &stencil);
}

void GLRenderContext::ClearDepthStencilBuffer(IFramebuffer* fbuf, float depth, int stencil)
{
	GLuint id = fbuf ? dyn_cast_ptr<GLFramebuffer*>(fbuf)->GetID() : 0;
	if (id != _glState.drawFbuf)
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, id);
		_glState.drawFbuf = id;
	}

	glClearBufferfi(GL_DEPTH_STENCIL, 0, depth, stencil);
}

/*
	Reads source framebuffer's pixels into provided system memory buffer.
	If pixel_store is 0, default values are used. If depth or stencil data is read,
	source_color_buf should be COLOR_BUFFER_NONE and color_clamp is ignored.
*/
void GLRenderContext::ReadPixels(IFramebuffer* source_fbuf, ColorBuffer source_color_buf, ColorReadClamp color_clamp, int x, int y, sizei width, sizei height, ImageFormat format, DataType type, const PixelStore* pixel_store, void* buffer)
{
	GLuint id = source_fbuf ? dyn_cast_ptr<GLFramebuffer*>(source_fbuf)->GetID() : 0;
	if (id != _glState.readFbuf)
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, id);
		_glState.readFbuf = id;
	}

	if (source_color_buf != ColorBuffer::None)
	{
		glReadBuffer(GetGLEnum(source_color_buf));
	}

	if (_glState.pixelPackBuf != 0)
	{
		glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
		_glState.pixelPackBuf = 0;
	}

	// apply pixel store states
	__SetPixelPackState(&_glState, pixel_store);

	glClampColor(GL_CLAMP_READ_COLOR, GetGLEnum(color_clamp));

	// read framebuffer pixels into buffer
	glReadPixels(x, y, width, height, GetGLEnum(format), GetGLEnum(type), buffer);
}

/*
	Reads source framebuffer's pixels into provided buffer object.
	If pixel_store is 0, default values are used. If depth or stencil data is read,
	source_color_buf should be COLOR_BUFFER_NONE and color_clamp is ignored.
*/
void GLRenderContext::ReadPixels(IFramebuffer* source_fbuf, ColorBuffer source_color_buf, ColorReadClamp color_clamp, int x, int y, sizei width, sizei height, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset)
{
	GLuint id = source_fbuf ? dyn_cast_ptr<GLFramebuffer*>(source_fbuf)->GetID() : 0;
	if (id != _glState.readFbuf)
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, id);
		_glState.readFbuf = id;
	}

	if (source_color_buf != ColorBuffer::None)
	{
		glReadBuffer(GetGLEnum(source_color_buf));
	}

	assert(buffer);
	id = dyn_cast_ptr<GLBuffer*>(buffer)->GetID();
	if (id != _glState.pixelPackBuf)
	{
		glBindBuffer(GL_PIXEL_PACK_BUFFER, id);
		_glState.pixelPackBuf = id;
	}

	// apply pixel store states
	__SetPixelPackState(&_glState, pixel_store);

	glClampColor(GL_CLAMP_READ_COLOR, GetGLEnum(color_clamp));

	// read framebuffer pixels into buffer
	glReadPixels(x, y, width, height, GetGLEnum(format), GetGLEnum(type), BUFFER_OFFSET(buffer_offset));
}

void GLRenderContext::BlitFramebuffer(IFramebuffer* source_fbuf, ColorBuffer source_color_buf, int src_x0, int src_y0, int src_x1, int src_y1, IFramebuffer* dest_fbuf, int dst_x0, int dst_y0, int dst_x1, int dst_y1, uint buffers, TexFilter filter)
{
	GLuint id = source_fbuf ? dyn_cast_ptr<GLFramebuffer*>(source_fbuf)->GetID() : 0;
	if (id != _glState.readFbuf)
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, id);
		_glState.readFbuf = id;
	}

	if ((buffers & COLOR_BUFFER_BIT) && (source_color_buf != ColorBuffer::None))
	{
		glReadBuffer(GetGLEnum(source_color_buf));
	}

	id = dest_fbuf ? dyn_cast_ptr<GLFramebuffer*>(dest_fbuf)->GetID() : 0;
	if (id != _glState.drawFbuf)
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, id);
		_glState.drawFbuf = id;
	}

	GLbitfield mask = 0;
	if (buffers & COLOR_BUFFER_BIT)
		mask |= GL_COLOR_BUFFER_BIT;
	if (buffers & DEPTH_BUFFER_BIT)
		mask |= GL_DEPTH_BUFFER_BIT;
	if (buffers & STENCIL_BUFFER_BIT)
		mask |= GL_STENCIL_BUFFER_BIT;

	glBlitFramebuffer(src_x0, src_y0, src_x1, src_y1, dst_x0, dst_y0, dst_x1, dst_y1, mask, GetGLEnum(filter));
}

void GLRenderContext::SetVertexShader(IVertexShader* shader)
{
	GLuint prog_id = shader ? dyn_cast_ptr<GLShader*>(shader)->GetID() : 0;
	glUseProgramStages(_pipeline, GL_VERTEX_SHADER_BIT, prog_id);
}

void GLRenderContext::SetTessControlShader(ITessControlShader* shader)
{
	GLuint prog_id = shader ? dyn_cast_ptr<GLShader*>(shader)->GetID() : 0;
	glUseProgramStages(_pipeline, GL_TESS_CONTROL_SHADER_BIT, prog_id);
}

void GLRenderContext::SetTessEvaluationShader(ITessEvaluationShader* shader)
{
	GLuint prog_id = shader ? dyn_cast_ptr<GLShader*>(shader)->GetID() : 0;
	glUseProgramStages(_pipeline, GL_TESS_EVALUATION_SHADER_BIT, prog_id);
}

void GLRenderContext::SetGeometryShader(IGeometryShader* shader)
{
	GLuint prog_id = shader ? dyn_cast_ptr<GLShader*>(shader)->GetID() : 0;
	glUseProgramStages(_pipeline, GL_GEOMETRY_SHADER_BIT, prog_id);
}

void GLRenderContext::SetFragmentShader(IFragmentShader* shader)
{
	GLuint prog_id = shader ? dyn_cast_ptr<GLShader*>(shader)->GetID() : 0;
	glUseProgramStages(_pipeline, GL_FRAGMENT_SHADER_BIT, prog_id);
}

void GLRenderContext::SetComputeShader(IComputeShader* shader)
{
	GLuint prog_id = shader ? dyn_cast_ptr<GLShader*>(shader)->GetID() : 0;
	glUseProgramStages(_pipeline, GL_COMPUTE_SHADER_BIT, prog_id);
}

void GLRenderContext::SetUniformBuffer(uint index, IBuffer* buffer)
{
	GLuint buf_id = dyn_cast_ptr<GLBuffer*>(buffer)->GetID();
	_glState.uniformBuf = buf_id;
	glBindBufferBase(GL_UNIFORM_BUFFER, index, buf_id);
}

void GLRenderContext::SetUniformBuffer(uint index, IBuffer* buffer, intptr offset, sizeiptr size)
{
	GLuint buf_id = dyn_cast_ptr<GLBuffer*>(buffer)->GetID();
	_glState.uniformBuf = buf_id;
	glBindBufferRange(GL_UNIFORM_BUFFER, index, buf_id, offset, size);
}

void GLRenderContext::SetAtomicCounterBuffer(uint index, IBuffer* buffer)
{
	GLuint buf_id = dyn_cast_ptr<GLBuffer*>(buffer)->GetID();
	_glState.atomicCounterBuf = buf_id;
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, index, buf_id);
}

void GLRenderContext::SetAtomicCounterBuffer(uint index, IBuffer* buffer, intptr offset, sizeiptr size)
{
	GLuint buf_id = dyn_cast_ptr<GLBuffer*>(buffer)->GetID();
	_glState.atomicCounterBuf = buf_id;
	glBindBufferRange(GL_ATOMIC_COUNTER_BUFFER, index, buf_id, offset, size);
}

void GLRenderContext::SetStorageBuffer(uint index, IBuffer* buffer)
{
	GLuint id = dyn_cast_ptr<GLBuffer*>(buffer)->GetID();
	_glState.shaderStorageBuf = id;
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, id);
}

void GLRenderContext::SetStorageBuffer(uint index, IBuffer* buffer, intptr offset, sizeiptr size)
{
	GLuint id = dyn_cast_ptr<GLBuffer*>(buffer)->GetID();
	_glState.shaderStorageBuf = id;
	glBindBufferRange(GL_SHADER_STORAGE_BUFFER, index, id, offset, size);
}

void GLRenderContext::UniformSubroutine(ShaderType shader_type, sizei count, const uint* indices)
{
	glUniformSubroutinesuiv(GetGLEnum(shader_type), count, indices);
}

void GLRenderContext::SetImageTexture(uint image_unit, ITexture* texture, int level, bool layered, int layer, BufferAccess access, PixelFormat format)
{
	glBindImageTexture(image_unit, dyn_cast_ptr<GLTexture*>(texture)->GetID(), level, layered, layer, GetGLEnum(access), GetGLEnum(format));
}

bool GLRenderContext::ValidateShaderPipeline()
{
	glValidateProgramPipeline(_pipeline);

	GLint status;
	glGetProgramPipelineiv(_pipeline, GL_VALIDATE_STATUS, &status);

	return (status == GL_TRUE);
}

void GLRenderContext::SetSamplerTexture(int sampler, ITexture* texture)
{
	assert(sampler >= 0 && sampler < _info.maxCombinedTextureImageUnits);

	GLTexture* newTexture = dyn_cast_ptr<GLTexture*>(texture);
	GLuint newTextureId = (newTexture != nullptr) ? newTexture->GetID() : 0;
	GLuint oldTextureId = _glState.imageUnits[sampler].texture;

	if (newTextureId != oldTextureId)
	{
		if (_glState.activeTexture != static_cast<GLuint>(sampler))
		{
			glActiveTexture(GL_TEXTURE0 + sampler);
			_glState.activeTexture = static_cast<GLuint>(sampler);
		}

		GLenum oldTarget = _glState.imageUnits[sampler].target;
		GLenum newTarget = (newTexture != nullptr) ? newTexture->GetTarget() : 0;

		// Clear the binding for a previously bound target.
		if (oldTextureId != 0 && oldTarget != 0 &&  oldTarget != newTarget)
			glBindTexture(oldTarget, 0);

		if (newTarget != 0)
			glBindTexture(newTarget, newTextureId);

		_glState.imageUnits[sampler].texture = newTextureId;
		_glState.imageUnits[sampler].target = newTarget;
	}

	// Remember texture for unit 0 also in an aditional variable, because GLTexture objects use this unit to bind textures for creation or update.
	// If a GLTexture did change the bound texture on unit 0, DelayedDrawingStateSetup() will restore the correct texture.
	// The user shouldn't use GLTexture's creation or update functions between a call to this function and drawing call anyway,
	// but correct behavior must be ensured in any case.
	if (sampler == 0)
		_unit0Texture = newTexture;
}

void GLRenderContext::SetSamplerState(int sampler, ISamplerState* state)
{
	assert(sampler >= 0 && sampler < _info.maxCombinedTextureImageUnits);

	GLSamplerState* newSampler = dyn_cast_ptr<GLSamplerState*>(state);
	GLuint newSamplerId = (newSampler != nullptr) ? newSampler->GetID() : 0;

	if (newSamplerId != _glState.imageUnits[sampler].sampler)
	{
		glBindSampler(sampler, newSamplerId);
		_glState.imageUnits[sampler].sampler = newSamplerId;
	}
}

void GLRenderContext::EnableSeamlessCubeMap(bool enable)
{
	if (enable)
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	else
		glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}

void GLRenderContext::Draw(PrimitiveType prim, int first, sizei count)
{
	DelayedDrawingStateSetup();
	glDrawArrays(GetGLEnum(prim), first, count);
}

void GLRenderContext::DrawInstanced(PrimitiveType prim, int first, sizei count, uint base_inst, sizei inst_count)
{
	DelayedDrawingStateSetup();
	glDrawArraysInstancedBaseInstance(GetGLEnum(prim), first, count, inst_count, base_inst);
}

// Same as DrawInstanced(), but parameters are sourced from the buffer (struct DrawIndirectData).
void GLRenderContext::DrawIndirect(PrimitiveType prim, IBuffer* buffer, intptr offset)
{
	DelayedDrawingStateSetup();

	assert(buffer != nullptr);
	GLuint buf_id = dyn_cast_ptr<GLBuffer*>(buffer)->GetID();
	if (buf_id != _glState.drawIndirectBuf)
	{
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, buf_id);
		_glState.drawIndirectBuf = buf_id;
	}

	glDrawArraysIndirect(GetGLEnum(prim), BUFFER_OFFSET(offset));
}

// Same as DrawIndirect, but buffer contains count DrawIndirectData structures.
// stride 0 means that structures are tightly packed.
void GLRenderContext::MultiDrawIndirect(PrimitiveType prim, IBuffer* buffer, intptr offset, sizei count, sizei stride)
{
	assert((stride & 3) == 0); // stride must be multiple of 4

	DelayedDrawingStateSetup();

	GLuint buf_id = dyn_cast_ptr<GLBuffer*>(buffer)->GetID();
	if (buf_id != _glState.drawIndirectBuf)
	{
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, buf_id);
		_glState.drawIndirectBuf = buf_id;
	}

	glMultiDrawArraysIndirect(GetGLEnum(prim), BUFFER_OFFSET(offset), count, stride);
}

/*
	Draws indexed primitives.
	index_start - an offset in bytes into the index buffer where the first index is to be taken.
	base_vertex - a constant that is added to each index.
	count - number of vertices to be rendered.
*/
void GLRenderContext::DrawIndexed(PrimitiveType prim, intptr index_start, int base_vertex, sizei count)
{
	DelayedDrawingStateSetup();
	glDrawElementsBaseVertex(GetGLEnum(prim), count, GetGLEnum(_indexType), BUFFER_OFFSET(index_start), base_vertex);
}

/*
	Draws a range of elements - it has constraint that all the indices must lie between start and end inclusive.
	index_start - an offset in bytes into the index buffer where the first index be taken.
	base_vertex - a constant that is added to each index.
	count - number of vertices to be rendered.
*/
void GLRenderContext::DrawIndexed(PrimitiveType prim, uint start, uint end, intptr index_start, int base_vertex, sizei count)
{
	DelayedDrawingStateSetup();
	glDrawRangeElementsBaseVertex(GetGLEnum(prim), start, end, count, GetGLEnum(_indexType), BUFFER_OFFSET(index_start), base_vertex);
}

void GLRenderContext::DrawIndexedInstanced(PrimitiveType prim, intptr index_start, int base_vertex, sizei count, uint base_inst, sizei inst_count)
{
	DelayedDrawingStateSetup();
	glDrawElementsInstancedBaseVertexBaseInstance(GetGLEnum(prim), count, GetGLEnum(_indexType), BUFFER_OFFSET(index_start), inst_count, base_vertex, base_inst);
}

// Same as DrawIndexedInstanced(), but parameters are sourced from the buffer (struct DrawIndexedIndirectData).
void GLRenderContext::DrawIndexedIndirect(PrimitiveType prim, IBuffer* buffer, intptr offset)
{
	DelayedDrawingStateSetup();

	GLuint buf_id = dyn_cast_ptr<GLBuffer*>(buffer)->GetID();
	if (buf_id != _glState.drawIndirectBuf)
	{
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, buf_id);
		_glState.drawIndirectBuf = buf_id;
	}

	glDrawElementsIndirect(GetGLEnum(prim), GetGLEnum(_indexType), BUFFER_OFFSET(offset));
}

// Same as DrawIndexedIndirect, but buffer contains count DrawIndexedIndirectData structures.
// stride 0 means that structures are tightly packed.
void GLRenderContext::MultiDrawIndexedIndirect(PrimitiveType prim, IBuffer* buffer, intptr offset, sizei count, sizei stride)
{
	assert((stride & 3) == 0); // stride must be multiple of 4

	DelayedDrawingStateSetup();

	GLuint buf_id = dyn_cast_ptr<GLBuffer*>(buffer)->GetID();
	if (buf_id != _glState.drawIndirectBuf)
	{
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, buf_id);
		_glState.drawIndirectBuf = buf_id;
	}

	glMultiDrawElementsIndirect(GetGLEnum(prim), GetGLEnum(_indexType), BUFFER_OFFSET(offset), count, stride);
}

void GLRenderContext::DrawTransformFeedback(PrimitiveType prim, ITransformFeedback* transform_feedback, uint stream)
{
	DelayedDrawingStateSetup();

	GLuint tf_id = dyn_cast_ptr<GLTransformFeedback*>(transform_feedback)->GetID();
	if (tf_id != _glState.transformFeedback)
	{
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, tf_id);
		_glState.transformFeedback = tf_id;
	}

	glDrawTransformFeedbackStream(GetGLEnum(prim), tf_id, stream);
}

void GLRenderContext::DrawTransformFeedbackInstanced(PrimitiveType prim, ITransformFeedback* transform_feedback, uint stream, sizei inst_count)
{
	DelayedDrawingStateSetup();

	GLuint tf_id = dyn_cast_ptr<GLTransformFeedback*>(transform_feedback)->GetID();
	if (tf_id != _glState.transformFeedback)
	{
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, tf_id);
		_glState.transformFeedback = tf_id;
	}

	glDrawTransformFeedbackStreamInstanced(GetGLEnum(prim), tf_id, stream, inst_count);
}

void GLRenderContext::DispatchCompute(uint num_groups_x, uint num_groups_y, uint num_groups_z)
{
	glDispatchCompute(num_groups_x, num_groups_y, num_groups_z);
}

void GLRenderContext::DispatchComputeIndirect(IBuffer* buffer, intptr offset)
{
	GLuint buf_id = dyn_cast_ptr<GLBuffer*>(buffer)->GetID();
	if (buf_id != _glState.dispatchIndirectBuf)
	{
		glBindBuffer(GL_DISPATCH_INDIRECT_BUFFER, buf_id);
		_glState.dispatchIndirectBuf = buf_id;
	}

	glDispatchComputeIndirect(offset);
}

void GLRenderContext::Flush()
{
	glFlush();
}

void GLRenderContext::Finish()
{
	glFinish();
}

// Flags are not used for now
SyncObject GLRenderContext::InsertFenceSync(FenceSyncCondition condition, uint flags)
{
	SyncObject sync = (SyncObject)glFenceSync(GetGLEnum(condition), 0);

	return sync;
}

void GLRenderContext::DeleteSync(SyncObject sync)
{
	glDeleteSync((GLsync)sync);
}

SyncWaitStatus GLRenderContext::ClientWaitSync(SyncObject sync, uint flags, uint64 timeout)
{
	GLbitfield bits = 0;
	if (flags & SYNC_FLUSH_COMMANDS_BIT)
		bits |= GL_SYNC_FLUSH_COMMANDS_BIT;
	GLenum status = glClientWaitSync((GLsync)sync, bits, timeout);

	return GetFromGLEnum<SyncWaitStatus>(status);
}

// Flags and timeout currently not used
void GLRenderContext::Wait(SyncObject sync, uint flags, uint64 timeout)
{
	glWaitSync((GLsync)sync, 0, GL_TIMEOUT_IGNORED);
}

void GLRenderContext::MemoryBarrier(uint flags)
{
	GLbitfield bits = 0;

	if (flags & BARRIER_VERTEX_ATTRIB_ARRAY_BIT)
		bits |= GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT;
	if (flags & BARRIER_INDEX_ARRAY_BIT)
		bits |= GL_ELEMENT_ARRAY_BARRIER_BIT;
	if (flags & BARRIER_UNIFORM_BIT)
		bits |= GL_UNIFORM_BARRIER_BIT;
	if (flags & BARRIER_TEXTURE_FETCH_BIT)
		bits |= GL_TEXTURE_FETCH_BARRIER_BIT;
	if (flags & BARRIER_SHADER_IMAGE_ACCESS_BIT)
		bits |= GL_SHADER_IMAGE_ACCESS_BARRIER_BIT;
	if (flags & BARRIER_COMMAND_BIT)
		bits |= GL_COMMAND_BARRIER_BIT;
	if (flags & BARRIER_PIXEL_BUFFER_BIT)
		bits |= GL_PIXEL_BUFFER_BARRIER_BIT;
	if (flags & BARRIER_TEXTURE_UPDATE_BIT)
		bits |= GL_TEXTURE_UPDATE_BARRIER_BIT;
	if (flags & BARRIER_BUFFER_UPDATE_BIT)
		bits |= GL_BUFFER_UPDATE_BARRIER_BIT;
	if (flags & BARRIER_FRAMEBUFFER_BIT)
		bits |= GL_FRAMEBUFFER_BARRIER_BIT;
	if (flags & BARRIER_TRANSFORM_FEEDBACK_BIT)
		bits |= GL_TRANSFORM_FEEDBACK_BARRIER_BIT;
	if (flags & BARRIER_ATOMIC_COUNTER_BIT)
		bits |= GL_ATOMIC_COUNTER_BARRIER_BIT;
	if (flags & BARRIER_SHADER_STORAGE_BIT)
		bits |= GL_SHADER_STORAGE_BARRIER_BIT;
	if (flags & BARRIER_QUERY_BUFFER_BIT)
		bits |= GL_QUERY_BUFFER_BARRIER_BIT;

	glMemoryBarrier(bits);
}

void GLRenderContext::MemoryBarrierByRegion(uint flags)
{
	GLbitfield bits = 0;

	if (flags & BARRIER_UNIFORM_BIT)
		bits |= GL_UNIFORM_BARRIER_BIT;
	if (flags & BARRIER_TEXTURE_FETCH_BIT)
		bits |= GL_TEXTURE_FETCH_BARRIER_BIT;
	if (flags & BARRIER_SHADER_IMAGE_ACCESS_BIT)
		bits |= GL_SHADER_IMAGE_ACCESS_BARRIER_BIT;
	if (flags & BARRIER_FRAMEBUFFER_BIT)
		bits |= GL_FRAMEBUFFER_BARRIER_BIT;
	if (flags & BARRIER_ATOMIC_COUNTER_BIT)
		bits |= GL_ATOMIC_COUNTER_BARRIER_BIT;
	if (flags & BARRIER_SHADER_STORAGE_BIT)
		bits |= GL_SHADER_STORAGE_BARRIER_BIT;

	glMemoryBarrierByRegion(bits);
}

void GLRenderContext::TextureBarrier()
{
	glTextureBarrier();
}

void GLRenderContext::CopyBufferData(IBuffer* source, intptr source_offset, IBuffer* dest, intptr dest_offset, sizeiptr size)
{
	GLuint srcId = dyn_cast_ptr<GLBuffer*>(source)->GetID();
	GLuint destId = dyn_cast_ptr<GLBuffer*>(dest)->GetID();

	if (srcId != _glState.copyReadBuffer)
	{
		glBindBuffer(GL_COPY_READ_BUFFER, srcId);
		_glState.copyReadBuffer = srcId;
	}

	if (destId != _glState.copyWriteBuffer)
	{
		glBindBuffer(GL_COPY_WRITE_BUFFER, destId);
		_glState.copyWriteBuffer = destId;
	}

	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, source_offset, dest_offset, size);
}

void GLRenderContext::CopyTextureData(
	ITexture* source, int source_level, int source_x, int source_y, int source_z, sizei width, sizei height, sizei depth,
	ITexture* dest, int dest_level, int dest_x, int dest_y, int dest_z)
{
	GLTexture* src_tex = dyn_cast_ptr<GLTexture*>(source);
	assert(src_tex->GetTextureType() != TextureType::TexBuffer);

	GLTexture* dst_tex = dyn_cast_ptr<GLTexture*>(dest);
	assert(dst_tex->GetTextureType() != TextureType::TexBuffer);

	glCopyImageSubData(
		src_tex->GetID(), src_tex->GetTarget(), source_level, source_x, source_y, source_z,
		dst_tex->GetID(), dst_tex->GetTarget(), dest_level, dest_x, dest_y, dest_z,
		width, height, depth);
}

void GLRenderContext::CopyRenderbufferData(IRenderbuffer* source, int source_x, int source_y, sizei width, sizei height, IRenderbuffer* dest, int dest_x, int dest_y)
{
	GLRenderbuffer* src_rbuf = dyn_cast_ptr<GLRenderbuffer*>(source);
	GLRenderbuffer* dst_rbuf = dyn_cast_ptr<GLRenderbuffer*>(dest);

	glCopyImageSubData(
		src_rbuf->GetID(), GL_RENDERBUFFER, 0, source_x, source_y, 0,
		dst_rbuf->GetID(), GL_RENDERBUFFER, 0, dest_x, dest_y, 0,
		width, height, 0);
}

void GLRenderContext::DelayedDrawingStateSetup()
{
	// Set current draw framebuffer, if the binding was changed by GLFramebuffer objects.
	GLuint fbufId = (_framebuffer != nullptr) ? _framebuffer->GetID() : 0;
	if (fbufId != _glState.drawFbuf)
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbufId);
		_glState.drawFbuf = fbufId;
	}

	if (!_info.featuresGL.ARB_vertex_attrib_binding)
	{
		// Setup vertex attribute pointers.
		if (_vertexFormat != nullptr)
		{
			int descCount = _vertexFormat->GetDescriptorCount();
			const VertexAttribDesc* descriptors = _vertexFormat->GetDescriptors();

			for (int i = 0; i < descCount; ++i)
			{
				const VertexAttribDesc& desc = descriptors[i];

				assert(desc.stream >= 0 && desc.stream < _info.maxVertexAttribBindings);
				assert(desc.attribute < (uint)_info.maxVertexAttribs);

				VertexStream& vstream = _vertexStreams[desc.stream];
				VertexAttrib& vattrib = _vertexAttribs[desc.attribute];

				if (vstream.buffer)
				{
					if (vstream.buffer != vattrib.buffer ||
						vstream.stride != vattrib.stride ||
						vstream.offset != vattrib.bufferBase ||
						desc.numComponents != vattrib.numComponents ||
						desc.type != vattrib.type ||
						desc.integer != vattrib.integer ||
						desc.normalized != vattrib.normalized ||
						desc.offset != vattrib.offset)
					{
						if (vstream.buffer->GetID() != _glState.vertexBuf)
						{
							glBindBuffer(GL_ARRAY_BUFFER, vstream.buffer->GetID());
							_glState.vertexBuf = vstream.buffer->GetID();
						}

						if (desc.type == DataType::Double)
						{
							glVertexAttribLPointer(desc.attribute, desc.numComponents, GL_DOUBLE, (GLsizei)vstream.stride, BUFFER_OFFSET(vstream.offset + desc.offset));
						}
						else
						{
							if (desc.integer)
								glVertexAttribIPointer(desc.attribute, desc.numComponents, GetGLEnum(desc.type), (GLsizei)vstream.stride, BUFFER_OFFSET(vstream.offset + desc.offset));
							else
								glVertexAttribPointer(desc.attribute, desc.numComponents, GetGLEnum(desc.type), desc.normalized, (GLsizei)vstream.stride, BUFFER_OFFSET(vstream.offset + desc.offset));
						}

						vattrib.buffer = vstream.buffer;
						vattrib.stride = vstream.stride;
						vattrib.bufferBase = vstream.offset;
						vattrib.numComponents = desc.numComponents;
						vattrib.type = desc.type;
						vattrib.integer = desc.integer;
						vattrib.normalized = desc.normalized;
						vattrib.offset = desc.offset;
					}

					if (vstream.divisor != vattrib.divisor)
					{
						glVertexAttribDivisor(desc.attribute, vstream.divisor);
						vattrib.divisor = vstream.divisor;
					}

					_vertAttribEnabledState[desc.attribute].newState = true;	// Set this flag for the next loop.
				}
			}
		}

		for (int i = 0; i < _info.maxVertexAttribs; ++i)
		{
			if (_vertAttribEnabledState[i].newState == true)
			{
				if (_vertAttribEnabledState[i].currentState == false)
				{
					glEnableVertexAttribArray(i);
					_vertAttribEnabledState[i].currentState = true;
				}
				_vertAttribEnabledState[i].newState = false;	// Reset this flag after changing the state.
			}
			else
			{
				if (_vertAttribEnabledState[i].currentState == true)
				{
					glDisableVertexAttribArray(i);
					_vertAttribEnabledState[i].currentState = false;
				}
			}
		}
	}

	// Bind the correct unit 0 texture if it was changed by GLTexture objects.
	GLuint newTextureId = (_unit0Texture != nullptr) ? _unit0Texture->GetID() : 0;
	GLuint oldTextureId = _glState.imageUnits[0].texture;

	if (newTextureId != oldTextureId)
	{
		GLenum oldTarget = _glState.imageUnits[0].target;
		GLenum newTarget = (_unit0Texture != nullptr) ? _unit0Texture->GetTarget() : 0;

		if (_glState.activeTexture != 0)
		{
			glActiveTexture(GL_TEXTURE0);
			_glState.activeTexture = 0;
		}

		// Clear the binding for a previously bound target.
		if (oldTextureId != 0 && oldTarget != 0 && oldTarget != newTarget)
			glBindTexture(oldTarget, 0);

		if (newTarget != 0)
			glBindTexture(newTarget, newTextureId);

		_glState.imageUnits[0].texture = newTextureId;
		_glState.imageUnits[0].target = newTarget;
	}
}

bool GLRenderContext::GetInternalFormatInfo(GLenum type, GLenum internal_format, InternalFormatInfo& info)
{
	GLint supported = GL_FALSE;
	glGetInternalformativ(type, internal_format, GL_INTERNALFORMAT_SUPPORTED, 1, &supported);
	if (!supported)
		return false;

	GLint temp;

	glGetInternalformativ(type, internal_format, GL_NUM_SAMPLE_COUNTS, 1, &info.numSampleCounts);

	int num_sample_counts = std::min(int(sizeof(info.samples) / sizeof(*info.samples)), info.numSampleCounts);
	memset(info.samples, 0, sizeof(info.samples));
	glGetInternalformativ(type, internal_format, GL_SAMPLES, num_sample_counts, info.samples);

	glGetInternalformativ(type, internal_format, GL_INTERNALFORMAT_PREFERRED, 1, &temp);
	info.preferredInternalFormat = GetFromGLEnum<PixelFormat>((GLenum)temp);

	glGetInternalformativ(type, internal_format, GL_INTERNALFORMAT_RED_SIZE, 1, &info.redSize);

	glGetInternalformativ(type, internal_format, GL_INTERNALFORMAT_GREEN_SIZE, 1, &info.greenSize);

	glGetInternalformativ(type, internal_format, GL_INTERNALFORMAT_BLUE_SIZE, 1, &info.blueSize);

	glGetInternalformativ(type, internal_format, GL_INTERNALFORMAT_ALPHA_SIZE, 1, &info.alphaSize);

	glGetInternalformativ(type, internal_format, GL_INTERNALFORMAT_DEPTH_SIZE, 1, &info.depthSize);

	glGetInternalformativ(type, internal_format, GL_INTERNALFORMAT_STENCIL_SIZE, 1, &info.stencilSize);

	glGetInternalformativ(type, internal_format, GL_INTERNALFORMAT_SHARED_SIZE, 1, &info.sharedSize);

	glGetInternalformativ(type, internal_format, GL_INTERNALFORMAT_RED_TYPE, 1, &temp);
	info.redType = GetFromGLEnum<TextureComponentType>((GLenum)temp);

	glGetInternalformativ(type, internal_format, GL_INTERNALFORMAT_GREEN_TYPE, 1, &temp);
	info.greenType = GetFromGLEnum<TextureComponentType>((GLenum)temp);

	glGetInternalformativ(type, internal_format, GL_INTERNALFORMAT_BLUE_TYPE, 1, &temp);
	info.blueType = GetFromGLEnum<TextureComponentType>((GLenum)temp);

	glGetInternalformativ(type, internal_format, GL_INTERNALFORMAT_ALPHA_TYPE, 1, &temp);
	info.alphaType = GetFromGLEnum<TextureComponentType>((GLenum)temp);

	glGetInternalformativ(type, internal_format, GL_INTERNALFORMAT_DEPTH_TYPE, 1, &temp);
	info.depthType = GetFromGLEnum<TextureComponentType>((GLenum)temp);

	glGetInternalformativ(type, internal_format, GL_INTERNALFORMAT_STENCIL_TYPE, 1, &temp);
	info.stencilType = GetFromGLEnum<TextureComponentType>((GLenum)temp);

	glGetInternalformativ(type, internal_format, GL_MAX_WIDTH, 1, &info.maxWidth);

	glGetInternalformativ(type, internal_format, GL_MAX_HEIGHT, 1, &info.maxHeight);

	glGetInternalformativ(type, internal_format, GL_MAX_DEPTH, 1, &info.maxDepth);

	glGetInternalformativ(type, internal_format, GL_MAX_LAYERS, 1, &info.maxLayers);

	glGetInternalformati64v(type, internal_format, GL_MAX_COMBINED_DIMENSIONS, 1, &info.maxCombinedDimensions);

	glGetInternalformativ(type, internal_format, GL_COLOR_COMPONENTS, 1, &temp);
	info.colorComponents = (temp == GL_TRUE);

	glGetInternalformativ(type, internal_format, GL_DEPTH_COMPONENTS, 1, &temp);
	info.depthComponents = (temp == GL_TRUE);

	glGetInternalformativ(type, internal_format, GL_STENCIL_COMPONENTS, 1, &temp);
	info.stencilComponents = (temp == GL_TRUE);

	glGetInternalformativ(type, internal_format, GL_COLOR_RENDERABLE, 1, &temp);
	info.colorRenderable = (temp == GL_TRUE);

	glGetInternalformativ(type, internal_format, GL_DEPTH_RENDERABLE, 1, &temp);
	info.depthRenderable = (temp == GL_TRUE);

	glGetInternalformativ(type, internal_format, GL_STENCIL_RENDERABLE, 1, &temp);
	info.stencilRenderable = (temp == GL_TRUE);

	glGetInternalformativ(type, internal_format, GL_FRAMEBUFFER_RENDERABLE, 1, &temp);
	info.framebufferRenderable = GetFromGLEnum<FeatureSupport>((GLenum)temp);

	glGetInternalformativ(type, internal_format, GL_FRAMEBUFFER_RENDERABLE_LAYERED, 1, &temp);
	info.framebufferRenderableLayered = GetFromGLEnum<FeatureSupport>((GLenum)temp);

	glGetInternalformativ(type, internal_format, GL_FRAMEBUFFER_BLEND, 1, &temp);
	info.framebufferBlend = GetFromGLEnum<FeatureSupport>((GLenum)temp);

	glGetInternalformativ(type, internal_format, GL_READ_PIXELS, 1, &temp);
	info.readPixels = GetFromGLEnum<FeatureSupport>((GLenum)temp);

	glGetInternalformativ(type, internal_format, GL_READ_PIXELS_FORMAT, 1, &temp);
	info.readPixelsFormat = GetFromGLEnum<ImageFormat>((GLenum)temp);

	glGetInternalformativ(type, internal_format, GL_READ_PIXELS_TYPE, 1, &temp);
	info.readPixelsType = GetFromGLEnum<DataType>((GLenum)temp);

	glGetInternalformativ(type, internal_format, GL_TEXTURE_IMAGE_FORMAT, 1, &temp);
	info.textureImageFormat = GetFromGLEnum<ImageFormat>((GLenum)temp);

	glGetInternalformativ(type, internal_format, GL_TEXTURE_IMAGE_TYPE, 1, &temp);
	info.textureImageType = GetFromGLEnum<DataType>((GLenum)temp);

	glGetInternalformativ(type, internal_format, GL_GET_TEXTURE_IMAGE_FORMAT, 1, &temp);
	info.getTextureImageFormat = GetFromGLEnum<ImageFormat>((GLenum)temp);

	glGetInternalformativ(type, internal_format, GL_GET_TEXTURE_IMAGE_TYPE, 1, &temp);
	info.getTextureImageType = GetFromGLEnum<DataType>((GLenum)temp);

	glGetInternalformativ(type, internal_format, GL_MIPMAP, 1, &temp);
	info.mipmap = (temp == GL_TRUE);

	glGetInternalformativ(type, internal_format, GL_MANUAL_GENERATE_MIPMAP, 1, &temp);
	info.manualGenerateMipmap = GetFromGLEnum<FeatureSupport>((GLenum)temp);

	glGetInternalformativ(type, internal_format, GL_COLOR_ENCODING, 1, &temp);
	info.colorEncoding = GetFromGLEnum<ColorEncoding>((GLenum)temp);

	glGetInternalformativ(type, internal_format, GL_SRGB_READ, 1, &temp);
	info.sRGBRead = GetFromGLEnum<FeatureSupport>((GLenum)temp);

	glGetInternalformativ(type, internal_format, GL_SRGB_WRITE, 1, &temp);
	info.sRGBWrite = GetFromGLEnum<FeatureSupport>((GLenum)temp);

	glGetInternalformativ(type, internal_format, GL_FILTER, 1, &temp);
	info.filter = GetFromGLEnum<FeatureSupport>((GLenum)temp);

	glGetInternalformativ(type, internal_format, GL_VERTEX_TEXTURE, 1, &temp);
	info.vertexTexture = GetFromGLEnum<FeatureSupport>((GLenum)temp);

	glGetInternalformativ(type, internal_format, GL_TESS_CONTROL_TEXTURE, 1, &temp);
	info.tessControlTexture = GetFromGLEnum<FeatureSupport>((GLenum)temp);

	glGetInternalformativ(type, internal_format, GL_TESS_EVALUATION_TEXTURE, 1, &temp);
	info.tessEvaluationTexture = GetFromGLEnum<FeatureSupport>((GLenum)temp);

	glGetInternalformativ(type, internal_format, GL_GEOMETRY_TEXTURE, 1, &temp);
	info.geometryTexture = GetFromGLEnum<FeatureSupport>((GLenum)temp);

	glGetInternalformativ(type, internal_format, GL_FRAGMENT_TEXTURE, 1, &temp);
	info.fragmentTexture = GetFromGLEnum<FeatureSupport>((GLenum)temp);

	glGetInternalformativ(type, internal_format, GL_COMPUTE_TEXTURE, 1, &temp);
	info.computeTexture = GetFromGLEnum<FeatureSupport>((GLenum)temp);

	glGetInternalformativ(type, internal_format, GL_TEXTURE_SHADOW, 1, &temp);
	info.textureShadow = GetFromGLEnum<FeatureSupport>((GLenum)temp);

	glGetInternalformativ(type, internal_format, GL_TEXTURE_GATHER, 1, &temp);
	info.textureGather = GetFromGLEnum<FeatureSupport>((GLenum)temp);

	glGetInternalformativ(type, internal_format, GL_TEXTURE_GATHER_SHADOW, 1, &temp);
	info.textureGatherShadow = GetFromGLEnum<FeatureSupport>((GLenum)temp);

	glGetInternalformativ(type, internal_format, GL_SHADER_IMAGE_LOAD, 1, &temp);
	info.shaderImageLoad = GetFromGLEnum<FeatureSupport>((GLenum)temp);

	glGetInternalformativ(type, internal_format, GL_SHADER_IMAGE_STORE, 1, &temp);
	info.shaderImageStore = GetFromGLEnum<FeatureSupport>((GLenum)temp);

	glGetInternalformativ(type, internal_format, GL_SHADER_IMAGE_ATOMIC, 1, &temp);
	info.shaderImageAtomic = GetFromGLEnum<FeatureSupport>((GLenum)temp);

	glGetInternalformativ(type, internal_format, GL_IMAGE_TEXEL_SIZE, 1, &info.imageTexelSize);

	glGetInternalformativ(type, internal_format, GL_IMAGE_COMPATIBILITY_CLASS, 1, &temp);
	info.imageCompatibilityClass = GetFromGLEnum<ImageClass>((GLenum)temp);

	glGetInternalformativ(type, internal_format, GL_IMAGE_PIXEL_FORMAT, 1, &temp);
	info.imagePixelFormat = GetFromGLEnum<ImageFormat>((GLenum)temp);

	glGetInternalformativ(type, internal_format, GL_IMAGE_PIXEL_TYPE, 1, &temp);
	info.imagePixelType = GetFromGLEnum<DataType>((GLenum)temp);

	glGetInternalformativ(type, internal_format, GL_IMAGE_FORMAT_COMPATIBILITY_TYPE, 1, &temp);
	info.imageFormatCompatibilityType = GetFromGLEnum<ImageFormatCompatibilityType>((GLenum)temp);

	glGetInternalformativ(type, internal_format, GL_SIMULTANEOUS_TEXTURE_AND_DEPTH_TEST, 1, &temp);
	info.simultaneousTextureAndDepthTest = GetFromGLEnum<FeatureSupport>((GLenum)temp);

	glGetInternalformativ(type, internal_format, GL_SIMULTANEOUS_TEXTURE_AND_STENCIL_TEST, 1, &temp);
	info.simultaneousTextureAndStencilTest = GetFromGLEnum<FeatureSupport>((GLenum)temp);

	glGetInternalformativ(type, internal_format, GL_SIMULTANEOUS_TEXTURE_AND_DEPTH_WRITE, 1, &temp);
	info.simultaneousTextureAndDepthWrite = GetFromGLEnum<FeatureSupport>((GLenum)temp);

	glGetInternalformativ(type, internal_format, GL_SIMULTANEOUS_TEXTURE_AND_STENCIL_WRITE, 1, &temp);
	info.simultaneousTextureAndStencilWrite= GetFromGLEnum<FeatureSupport>((GLenum)temp);

	glGetInternalformativ(type, internal_format, GL_TEXTURE_COMPRESSED, 1, &temp);
	info.textureCompressed = (temp == GL_TRUE);

	glGetInternalformativ(type, internal_format, GL_TEXTURE_COMPRESSED_BLOCK_WIDTH, 1, &info.textureCompressedBlockWidth);

	glGetInternalformativ(type, internal_format, GL_TEXTURE_COMPRESSED_BLOCK_HEIGHT, 1, &info.textureCompressedBlockHeight);

	glGetInternalformativ(type, internal_format, GL_TEXTURE_COMPRESSED_BLOCK_SIZE, 1, &info.textureCompressedBlockSize);

	glGetInternalformativ(type, internal_format, GL_CLEAR_BUFFER, 1, &temp);
	info.clearBuffer = GetFromGLEnum<FeatureSupport>((GLenum)temp);

	glGetInternalformativ(type, internal_format, GL_TEXTURE_VIEW, 1, &temp);
	info.textureView = GetFromGLEnum<FeatureSupport>((GLenum)temp);

	glGetInternalformativ(type, internal_format, GL_VIEW_COMPATIBILITY_CLASS, 1, &temp);
	info.viewCompatibilityClass = GetFromGLEnum<ViewClass>((GLenum)temp);


	return true;
}

void GLRenderContext::DebugMessage(DebugMessageSource source, DebugMessageType type, DebugMessageSeverity severity, ErrorMessageId message_id, ...)
{
	if (_logger)
	{
		const char* message = GetMessageString(message_id);
		if (message)
		{
			va_list args;
			va_start(args, message_id);

			char buf[1024];
			vsnprintf(buf, 1024, message, args);

			va_end(args);

			_logger->DebugMessage(source, type, static_cast<uint>(message_id), severity, buf);
		}
	}
}

void GLRenderContext::GetViewport(int viewport[4])
{
	glGetIntegerv(GL_VIEWPORT, viewport);
}

void GLRenderContext::GetViewport(float viewport[4])
{
	glGetFloatv(GL_VIEWPORT, viewport);
}

void GLRenderContext::GetViewportIndexed(uint index, float viewport[4])
{
	glGetFloati_v(GL_VIEWPORT, index, viewport);
}

int64 GLRenderContext::GetGPUTimestamp()
{
	GLint64 timestamp;
	glGetInteger64v(GL_TIMESTAMP, &timestamp);
	return timestamp;
}

bool GLRenderContext::GetTextureInternalFormatInfo(TextureType type, PixelFormat internal_format, InternalFormatInfo& info)
{
	return GetInternalFormatInfo(GetGLEnum(type), GetGLEnum(internal_format), info);
}

bool GLRenderContext::GetRenderbufferInternalFormatInfo(PixelFormat internal_format, InternalFormatInfo& info)
{
	return GetInternalFormatInfo(GL_RENDERBUFFER, GetGLEnum(internal_format), info);
}

ErrorCode GLRenderContext::GetLastError()
{
	GLenum error = glGetError();
	return GetFromGLEnum<ErrorCode>(error);
}

IVertexFormat* GLRenderContext::CreateVertexFormat(const VertexAttribDesc* descriptors, int count)
{
	GLVertexFormat* vformat = new GLVertexFormat;
	if (!vformat->Create(_info, descriptors, count))
	{
		delete vformat;
		return nullptr;
	}

	return vformat;
}

void GLRenderContext::DestroyVertexFormat(IVertexFormat* vert_fmt)
{
	if (_vertexFormat == static_cast<GLVertexFormat*>(vert_fmt))
		_vertexFormat = nullptr;

	delete vert_fmt;
}

ISamplerState* GLRenderContext::CreateSamplerState(const SamplerStateDesc& descriptor)
{
	GLSamplerState* sstate = new GLSamplerState;
	bool result = sstate->Create(descriptor);
	if (!result)
	{
		delete sstate;
		sstate = nullptr;
	}
	return sstate;
}

void GLRenderContext::DestroySamplerState(ISamplerState* samp_state)
{
	if (samp_state)
	{
		GLSamplerState* gl_sampl = dyn_cast_ptr<GLSamplerState*>(samp_state);

		for (int i = 0; i < _info.maxCombinedTextureImageUnits; ++i)
		{
			if (_glState.imageUnits[i].sampler == gl_sampl->GetID())
			{
				_glState.imageUnits[i].sampler = 0;
				glBindSampler(i, 0);
			}
		}

		gl_sampl->Destroy();
		delete samp_state;
	}
}

ITexture1D* GLRenderContext::CreateTexture1D(sizei levels, PixelFormat internal_format, int width)
{
	GLTexture1D* tex = new GLTexture1D;
	bool result = tex->CreateImmutable(&_glState, levels, internal_format, width);
	if (!result)
	{
		delete tex;
		tex = nullptr;
	}
	return tex;
}

ITexture1D* GLRenderContext::CreateTexture1DView(ITexture1D* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels)
{
	GLTexture1D* tex = new GLTexture1D;
	bool result = tex->CreateView(&_glState, dyn_cast_ptr<GLTexture*>(orig_tex)->GetID(), internal_format, min_level, num_levels, 0);
	if (!result)
	{
		delete tex;
		tex = nullptr;
	}
	return tex;
}

ITexture1D* GLRenderContext::CreateTexture1DView(ITexture1DArray* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint layer)
{
	GLTexture1D* tex = new GLTexture1D;
	bool result = tex->CreateView(&_glState, dyn_cast_ptr<GLTexture*>(orig_tex)->GetID(), internal_format, min_level, num_levels, layer);
	if (!result)
	{
		delete tex;
		tex = nullptr;
	}
	return tex;
}

ITexture1DArray* GLRenderContext::CreateTexture1DArray(sizei levels, PixelFormat internal_format, int width, int height)
{
	GLTexture1DArray* tex = new GLTexture1DArray;
	bool result = tex->CreateImmutable(&_glState, levels, internal_format, width, height);
	if (!result)
	{
		delete tex;
		tex = nullptr;
	}
	return tex;
}

ITexture1DArray* GLRenderContext::CreateTexture1DArrayView(ITexture1D* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels)
{
	GLTexture1DArray* tex = new GLTexture1DArray;
	bool result = tex->CreateView(&_glState, dyn_cast_ptr<GLTexture*>(orig_tex)->GetID(), internal_format, min_level, num_levels, 0, 1);
	if (!result)
	{
		delete tex;
		tex = nullptr;
	}
	return tex;
}

ITexture1DArray* GLRenderContext::CreateTexture1DArrayView(ITexture1DArray* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint min_layer, uint num_layers)
{
	GLTexture1DArray* tex = new GLTexture1DArray;
	bool result = tex->CreateView(&_glState, dyn_cast_ptr<GLTexture*>(orig_tex)->GetID(), internal_format, min_level, num_levels, min_layer, num_layers);
	if (!result)
	{
		delete tex;
		tex = nullptr;
	}
	return tex;
}

ITexture2D* GLRenderContext::CreateTexture2D(sizei levels, PixelFormat internal_format, int width, int height)
{
	GLTexture2D* tex = new GLTexture2D;
	bool result = tex->CreateImmutable(&_glState, levels, internal_format, width, height);
	if (!result)
	{
		delete tex;
		tex = nullptr;
	}
	return tex;
}

ITexture2D* GLRenderContext::CreateTexture2DView(ITexture2D* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels)
{
	GLTexture2D* tex = new GLTexture2D;
	bool result = tex->CreateView(&_glState, dyn_cast_ptr<GLTexture*>(orig_tex)->GetID(), internal_format, min_level, num_levels, 0);
	if (!result)
	{
		delete tex;
		tex = nullptr;
	}
	return tex;
}

ITexture2D* GLRenderContext::CreateTexture2DView(ITexture2DArray* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint layer)
{
	GLTexture2D* tex = new GLTexture2D;
	bool result = tex->CreateView(&_glState, dyn_cast_ptr<GLTexture*>(orig_tex)->GetID(), internal_format, min_level, num_levels, layer);
	if (!result)
	{
		delete tex;
		tex = nullptr;
	}
	return tex;
}

ITexture2D* GLRenderContext::CreateTexture2DView(ITextureCube* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint layer)
{
	GLTexture2D* tex = new GLTexture2D;
	bool result = tex->CreateView(&_glState, dyn_cast_ptr<GLTexture*>(orig_tex)->GetID(), internal_format, min_level, num_levels, layer);
	if (!result)
	{
		delete tex;
		tex = nullptr;
	}
	return tex;
}

ITexture2D* GLRenderContext::CreateTexture2DView(ITextureCubeArray* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint layer)
{
	GLTexture2D* tex = new GLTexture2D;
	bool result = tex->CreateView(&_glState, dyn_cast_ptr<GLTexture*>(orig_tex)->GetID(), internal_format, min_level, num_levels, layer);
	if (!result)
	{
		delete tex;
		tex = nullptr;
	}
	return tex;
}

ITexture2DArray* GLRenderContext::CreateTexture2DArray(sizei levels, PixelFormat internal_format, int width, int height, int depth)
{
	GLTexture2DArray* tex = new GLTexture2DArray;
	bool result = tex->CreateImmutable(&_glState, levels, internal_format, width, height, depth);
	if (!result)
	{
		delete tex;
		tex = nullptr;
	}
	return tex;
}

ITexture2DArray* GLRenderContext::CreateTexture2DArrayView(ITexture2D* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels)
{
	GLTexture2DArray* tex = new GLTexture2DArray;
	bool result = tex->CreateView(&_glState, dyn_cast_ptr<GLTexture*>(orig_tex)->GetID(), internal_format, min_level, num_levels, 0, 1);
	if (!result)
	{
		delete tex;
		tex = nullptr;
	}
	return tex;
}

ITexture2DArray* GLRenderContext::CreateTexture2DArrayView(ITexture2DArray* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint min_layer, uint num_layers)
{
	GLTexture2DArray* tex = new GLTexture2DArray;
	bool result = tex->CreateView(&_glState, dyn_cast_ptr<GLTexture*>(orig_tex)->GetID(), internal_format, min_level, num_levels, min_layer, num_layers);
	if (!result)
	{
		delete tex;
		tex = nullptr;
	}
	return tex;
}

ITexture2DArray* GLRenderContext::CreateTexture2DArrayView(ITextureCube* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint min_layer, uint num_layers)
{
	GLTexture2DArray* tex = new GLTexture2DArray;
	bool result = tex->CreateView(&_glState, dyn_cast_ptr<GLTexture*>(orig_tex)->GetID(), internal_format, min_level, num_levels, min_layer, num_layers);
	if (!result)
	{
		delete tex;
		tex = nullptr;
	}
	return tex;
}

ITexture2DArray* GLRenderContext::CreateTexture2DArrayView(ITextureCubeArray* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint min_layer, uint num_layers)
{
	GLTexture2DArray* tex = new GLTexture2DArray;
	bool result = tex->CreateView(&_glState, dyn_cast_ptr<GLTexture*>(orig_tex)->GetID(), internal_format, min_level, num_levels, min_layer, num_layers);
	if (!result)
	{
		delete tex;
		tex = nullptr;
	}
	return tex;
}

ITexture2DMultisample* GLRenderContext::CreateTexture2DMultisample(int samples, PixelFormat internal_format, int width, int height, bool fixed_sample_locations)
{
	GLTexture2DMultisample* tex = new GLTexture2DMultisample;
	bool result = tex->CreateImmutable(&_glState, samples, internal_format, width, height, fixed_sample_locations);
	if (!result)
	{
		delete tex;
		tex = nullptr;
	}
	return tex;
}

ITexture2DMultisample* GLRenderContext::CreateTexture2DMultisampleView(ITexture2DMultisample* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels)
{
	GLTexture2DMultisample* tex = new GLTexture2DMultisample;
	bool result = tex->CreateView(&_glState, dyn_cast_ptr<GLTexture*>(orig_tex)->GetID(), internal_format, min_level, num_levels);
	if (!result)
	{
		delete tex;
		tex = nullptr;
	}
	return tex;
}

ITexture2DMultisample* GLRenderContext::CreateTexture2DMultisampleView(ITexture2DMultisampleArray* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels)
{
	GLTexture2DMultisample* tex = new GLTexture2DMultisample;
	bool result = tex->CreateView(&_glState, dyn_cast_ptr<GLTexture*>(orig_tex)->GetID(), internal_format, min_level, num_levels);
	if (!result)
	{
		delete tex;
		tex = nullptr;
	}
	return tex;
}

ITexture2DMultisampleArray* GLRenderContext::CreateTexture2DMultisampleArray(int samples, PixelFormat internal_format, int width, int height, int depth, bool fixed_sample_locations)
{
	GLTexture2DMultisampleArray* tex = new GLTexture2DMultisampleArray;
	bool result = tex->CreateImmutable(&_glState, samples, internal_format, width, height, depth, fixed_sample_locations);
	if (!result)
	{
		delete tex;
		tex = nullptr;
	}
	return tex;
}

ITexture2DMultisampleArray* GLRenderContext::CreateTexture2DMultisampleArrayView(ITexture2DMultisample* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels)
{
	GLTexture2DMultisampleArray* tex = new GLTexture2DMultisampleArray;
	bool result = tex->CreateView(&_glState, dyn_cast_ptr<GLTexture*>(orig_tex)->GetID(), internal_format, min_level, num_levels, 0, 1);
	if (!result)
	{
		delete tex;
		tex = nullptr;
	}
	return tex;
}

ITexture2DMultisampleArray* GLRenderContext::CreateTexture2DMultisampleArrayView(ITexture2DMultisampleArray* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint min_layer, uint num_layers)
{
	GLTexture2DMultisampleArray* tex = new GLTexture2DMultisampleArray;
	bool result = tex->CreateView(&_glState, dyn_cast_ptr<GLTexture*>(orig_tex)->GetID(), internal_format, min_level, num_levels, min_layer, num_layers);
	if (!result)
	{
		delete tex;
		tex = nullptr;
	}
	return tex;
}

ITexture3D* GLRenderContext::CreateTexture3D(sizei levels, PixelFormat internal_format, int width, int height, int depth)
{
	GLTexture3D* tex = new GLTexture3D;
	bool result = tex->CreateImmutable(&_glState, levels, internal_format, width, height, depth);
	if (!result)
	{
		delete tex;
		tex = nullptr;
	}
	return tex;
}

ITexture3D* GLRenderContext::CreateTexture3DView(ITexture3D* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels)
{
	GLTexture3D* tex = new GLTexture3D;
	bool result = tex->CreateView(&_glState, dyn_cast_ptr<GLTexture*>(orig_tex)->GetID(), internal_format, min_level, num_levels);
	if (!result)
	{
		delete tex;
		tex = nullptr;
	}
	return tex;
}

ITextureCube* GLRenderContext::CreateTextureCube(sizei levels, PixelFormat internal_format, int width)
{
	GLTextureCube* tex = new GLTextureCube;
	bool result = tex->CreateImmutable(&_glState, levels, internal_format, width);
	if (!result)
	{
		delete tex;
		tex = nullptr;
	}
	return tex;
}

ITextureCube* GLRenderContext::CreateTextureCubeView(ITextureCube* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels)
{
	GLTextureCube* tex = new GLTextureCube;
	bool result = tex->CreateView(&_glState, dyn_cast_ptr<GLTexture*>(orig_tex)->GetID(), internal_format, min_level, num_levels, 0, 6);
	if (!result)
	{
		delete tex;
		tex = nullptr;
	}
	return tex;
}

ITextureCube* GLRenderContext::CreateTextureCubeView(ITextureCubeArray* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint min_layer, uint num_layers)
{
	GLTextureCube* tex = new GLTextureCube;
	bool result = tex->CreateView(&_glState, dyn_cast_ptr<GLTexture*>(orig_tex)->GetID(), internal_format, min_level, num_levels, min_layer, num_layers);
	if (!result)
	{
		delete tex;
		tex = nullptr;
	}
	return tex;
}

ITextureCube* GLRenderContext::CreateTextureCubeView(ITexture2DArray* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint min_layer, uint num_layers)
{
	assert(orig_tex->GetWidth() == orig_tex->GetHeight() && orig_tex->GetWidth() <= _info.maxCubeMapSize);

	GLTextureCube* tex = new GLTextureCube;
	bool result = tex->CreateView(&_glState, dyn_cast_ptr<GLTexture*>(orig_tex)->GetID(), internal_format, min_level, num_levels, min_layer, num_layers);
	if (!result)
	{
		delete tex;
		tex = nullptr;
	}
	return tex;
}

ITextureCubeArray* GLRenderContext::CreateTextureCubeArray(sizei levels, PixelFormat internal_format, int width, int depth)
{
	GLTextureCubeArray* tex = new GLTextureCubeArray;
	bool result = tex->CreateImmutable(&_glState, levels, internal_format, width, depth);
	if (!result)
	{
		delete tex;
		tex = nullptr;
	}
	return tex;
}

ITextureCubeArray* GLRenderContext::CreateTextureCubeArrayView(ITextureCube* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint min_layer, uint num_layers)
{
	GLTextureCubeArray* tex = new GLTextureCubeArray;
	bool result = tex->CreateView(&_glState, dyn_cast_ptr<GLTexture*>(orig_tex)->GetID(), internal_format, min_level, num_levels, min_layer, num_layers);
	if (!result)
	{
		delete tex;
		tex = nullptr;
	}
	return tex;
}

ITextureCubeArray* GLRenderContext::CreateTextureCubeArrayView(ITextureCubeArray* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint min_layer, uint num_layers)
{
	GLTextureCubeArray* tex = new GLTextureCubeArray;
	bool result = tex->CreateView(&_glState, dyn_cast_ptr<GLTexture*>(orig_tex)->GetID(), internal_format, min_level, num_levels, min_layer, num_layers);
	if (!result)
	{
		delete tex;
		tex = nullptr;
	}
	return tex;
}

ITextureCubeArray* GLRenderContext::CreateTextureCubeArrayView(ITexture2DArray* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint min_layer, uint num_layers)
{
	assert(orig_tex->GetWidth() == orig_tex->GetHeight() && orig_tex->GetWidth() <= _info.maxCubeMapSize);

	GLTextureCubeArray* tex = new GLTextureCubeArray;
	bool result = tex->CreateView(&_glState, dyn_cast_ptr<GLTexture*>(orig_tex)->GetID(), internal_format, min_level, num_levels, min_layer, num_layers);
	if (!result)
	{
		delete tex;
		tex = nullptr;
	}
	return tex;
}

ITextureBuffer* GLRenderContext::CreateTextureBuffer()
{
	GLTextureBuffer* tex = new GLTextureBuffer;
	bool result = tex->Create(&_glState);
	if (!result)
	{
		delete tex;
		tex = nullptr;
	}
	return tex;
}

ITextureRectangle* GLRenderContext::CreateTextureRectangle(sizei levels, PixelFormat internal_format, int width, int height)
{
	GLTextureRectangle* tex = new GLTextureRectangle;
	bool result = tex->CreateImmutable(&_glState, levels, internal_format, width, height);
	if (!result)
	{
		delete tex;
		tex = nullptr;
	}
	return tex;
}

ITextureRectangle* GLRenderContext::CreateTextureRectangleView(ITextureRectangle* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels)
{
	GLTextureRectangle* tex = new GLTextureRectangle;
	bool result = tex->CreateView(&_glState, dyn_cast_ptr<GLTexture*>(orig_tex)->GetID(), internal_format, min_level, num_levels);
	if (!result)
	{
		delete tex;
		tex = nullptr;
	}
	return tex;
}

void GLRenderContext::DestroyTexture(ITexture* texture)
{
	if (texture)
	{
		GLTexture* gl_tex = dyn_cast_ptr<GLTexture*>(texture);
		// remove sampler references to this texture
		for (int i = 0; i <= _info.maxCombinedTextureImageUnits; ++i)
		{
			if (_glState.imageUnits[i].texture == gl_tex->GetID())
			{
				if (_glState.activeTexture != static_cast<GLuint>(i))
				{
					glActiveTexture(GL_TEXTURE0 + i);
					_glState.activeTexture = static_cast<GLuint>(i);
				}
				glBindTexture(gl_tex->GetTarget(), 0);
				_glState.imageUnits[i].texture = 0;
				_glState.imageUnits[i].target = 0;

				if (i == 0)
					_unit0Texture = nullptr;
			}
		}

		gl_tex->Destroy();
		delete texture;
	}
}

IBuffer* GLRenderContext::CreateBuffer(sizeiptr size, const void* data, uint flags)
{
	GLBuffer* buf = new GLBuffer;
	bool result = buf->Create(&_glState, size, data, flags);
	if (!result)
	{
		delete buf;
		buf = nullptr;
	}
	return buf;
}

void GLRenderContext::DestroyBuffer(IBuffer* buffer)
{
	if (buffer)
	{
		// If it is currently used, remove all references to this buffer, 
		// unbind if it is bound to any biding point and disable all attribute
		// arrays that use it.
		GLBuffer* gl_buf = dyn_cast_ptr<GLBuffer*>(buffer);
			
		if (!_info.featuresGL.ARB_vertex_attrib_binding)
		{
			for (int i = 0; i < _info.maxVertexAttribBindings; ++i)
			{
				if (_vertexStreams[i].buffer == gl_buf)
				{
					_vertexStreams[i].buffer = nullptr;
					if (_info.featuresGL.ARB_vertex_attrib_binding)
					{
						glBindVertexBuffer(i, 0, 0, 0);
					}
				}
			}

			for (int i = 0; i < _info.maxVertexAttribs; ++i)
			{
				if (_vertexAttribs[i].buffer == gl_buf)
				{
					_vertexAttribs[i].buffer = nullptr;
					if (_vertAttribEnabledState[i].currentState)
					{
						glDisableVertexAttribArray(i);
						_vertAttribEnabledState[i].currentState = false;
					}
				}
			}
		}

		if (_glState.vertexBuf == gl_buf->GetID())
			_glState.vertexBuf = 0;

		if (_glState.indexBuf == gl_buf->GetID())
			_glState.indexBuf = 0;

		if (_glState.pixelPackBuf == gl_buf->GetID())
			_glState.pixelPackBuf = 0;

		if (_glState.pixelUnpackBuf == gl_buf->GetID())
			_glState.pixelUnpackBuf = 0;

		if (_glState.textureBuf == gl_buf->GetID())
			_glState.textureBuf = 0;

		if (_glState.uniformBuf == gl_buf->GetID())
			_glState.uniformBuf = 0;

		if (_glState.transfFeedbackBuf == gl_buf->GetID())
			_glState.transfFeedbackBuf = 0;

		if (_glState.drawIndirectBuf == gl_buf->GetID())
			_glState.drawIndirectBuf = 0;

		if (_glState.dispatchIndirectBuf == gl_buf->GetID())
			_glState.dispatchIndirectBuf = 0;

		if (_glState.atomicCounterBuf == gl_buf->GetID())
			_glState.atomicCounterBuf = 0;

		if (_glState.shaderStorageBuf == gl_buf->GetID())
			_glState.shaderStorageBuf = 0;

		if (_glState.queryBuffer == gl_buf->GetID())
			_glState.queryBuffer = 0;

		if (_glState.copyWriteBuffer == gl_buf->GetID())
			_glState.copyWriteBuffer = 0;

		if (_glState.copyReadBuffer == gl_buf->GetID())
			_glState.copyReadBuffer = 0;

		gl_buf->Destroy();
		delete buffer;
	}
}

IFramebuffer* GLRenderContext::CreateFramebuffer()
{
	GLFramebuffer* fbuf = new GLFramebuffer;
	bool result = fbuf->Create(&_glState);
	if (!result)
	{
		delete fbuf;
		fbuf = nullptr;
	}
	return fbuf;
}

IFramebuffer* GLRenderContext::CreateFramebufferWithoutAttachments(const FramebufferParams& params)
{
	GLFramebuffer* fbuf = new GLFramebuffer;
	bool result = fbuf->CreateWithoutAttachments(&_glState, params);
	if (!result)
	{
		delete fbuf;
		fbuf = nullptr;
	}
	return fbuf;
}

void GLRenderContext::DestroyFramebuffer(IFramebuffer* framebuffer)
{
	if (framebuffer)
	{
		GLFramebuffer* gl_fbuf = dyn_cast_ptr<GLFramebuffer*>(framebuffer);
		assert(gl_fbuf);

		if (_glState.drawFbuf == gl_fbuf->GetID())
			_glState.drawFbuf = 0;

		if (_glState.readFbuf == gl_fbuf->GetID())
			_glState.readFbuf = 0;

		gl_fbuf->Destroy();
		delete framebuffer;
	}
}

IRenderbuffer* GLRenderContext::CreateRenderbuffer(sizei samples, PixelFormat internal_format, sizei width, sizei height)
{
	GLRenderbuffer* rbuf = new GLRenderbuffer;
	if (!rbuf->Create(&_glState, samples, internal_format, width, height))
	{
		delete rbuf;
		rbuf = nullptr;
	}
	return rbuf;
}

void GLRenderContext::DestroyRenderbuffer(IRenderbuffer* renderbuffer)
{
	if (renderbuffer)
	{
		GLRenderbuffer* rbuf = dyn_cast_ptr<GLRenderbuffer*>(renderbuffer);

		if (_glState.renderbuffer == rbuf->GetID())
			_glState.renderbuffer = 0;

		rbuf->Destroy();
		delete renderbuffer;
	}
}

IQuery* GLRenderContext::CreateQuery()
{
	GLQuery* query = new GLQuery;
	query->Create();
	return query;
}

void GLRenderContext::DestroyQuery(IQuery* query)
{
	if (query)
	{
		static_cast<GLQuery*>(query)->Destroy();
		delete query;
	}
}

IVertexShader* GLRenderContext::CreateVertexShader(sizei count, const char** source, bool& success)
{
	GLVertexShader* shader = new GLVertexShader;
	success = shader->Create(count, source);
	return shader;
}

IVertexShader* GLRenderContext::CreateVertexShader(sizei size, const void* binary, uint format, bool& success)
{
	GLVertexShader* shader = new GLVertexShader;
	success = shader->Create(size, binary, format);
	return shader;
}

IVertexShader* GLRenderContext::CreateVertexShaderWithTransformFeedback(sizei count, const char** source, sizei attrib_count, const char** attrib_names, bool& success)
{
	GLVertexShader* shader = new GLVertexShader;
	success = shader->CreateWithTransformFeedback(count, source, attrib_count, attrib_names);
	return shader;
}

IVertexShader* GLRenderContext::CreateVertexShaderWithTransformFeedback(sizei size, const void* binary, uint format, sizei attrib_count, const char** attrib_names, bool& success)
{
	GLVertexShader* shader = new GLVertexShader;
	success = shader->CreateWithTransformFeedback(size, binary, format, attrib_count, attrib_names);
	return shader;
}

ITessControlShader* GLRenderContext::CreateTessControlShader(sizei count, const char** source, bool& success)
{
	GLTessControlShader* shader = new GLTessControlShader;
	success = shader->Create(count, source);
	return shader;
}

ITessControlShader* GLRenderContext::CreateTessControlShader(sizei size, const void* binary, uint format, bool& success)
{
	GLTessControlShader* shader = new GLTessControlShader;
	success = shader->Create(size, binary, format);
	return shader;
}

ITessControlShader* GLRenderContext::CreateTessControlShaderWithTransformFeedback(sizei count, const char** source, sizei attrib_count, const char** attrib_names, bool& success)
{
	GLTessControlShader* shader = new GLTessControlShader;
	success = shader->CreateWithTransformFeedback(count, source, attrib_count, attrib_names);
	return shader;
}

ITessControlShader* GLRenderContext::CreateTessControlShaderWithTransformFeedback(sizei size, const void* binary, uint format, sizei attrib_count, const char** attrib_names, bool& success)
{
	GLTessControlShader* shader = new GLTessControlShader;
	success = shader->CreateWithTransformFeedback(size, binary, format, attrib_count, attrib_names);
	return shader;
}

ITessEvaluationShader* GLRenderContext::CreateTessEvaluationShader(sizei count, const char** source, bool& success)
{
	GLTessEvaluationShader* shader = new GLTessEvaluationShader;
	success = shader->Create(count, source);
	return shader;
}

ITessEvaluationShader* GLRenderContext::CreateTessEvaluationShader(sizei size, const void* binary, uint format, bool& success)
{
	GLTessEvaluationShader* shader = new GLTessEvaluationShader;
	success = shader->Create(size, binary, format);
	return shader;
}

ITessEvaluationShader* GLRenderContext::CreateTessEvaluationShaderWithTransformFeedback(sizei count, const char** source, sizei attrib_count, const char** attrib_names, bool& success)
{
	GLTessEvaluationShader* shader = new GLTessEvaluationShader;
	success = shader->CreateWithTransformFeedback(count, source, attrib_count, attrib_names);
	return shader;
}

ITessEvaluationShader* GLRenderContext::CreateTessEvaluationShaderWithTransformFeedback(sizei size, const void* binary, uint format, sizei attrib_count, const char** attrib_names, bool& success)
{
	GLTessEvaluationShader* shader = new GLTessEvaluationShader;
	success = shader->CreateWithTransformFeedback(size, binary, format, attrib_count, attrib_names);
	return shader;
}

IGeometryShader* GLRenderContext::CreateGeometryShader(sizei count, const char** source, bool& success)
{
	GLGeometryShader* shader = new GLGeometryShader;
	success = shader->Create(count, source);
	return shader;
}

IGeometryShader* GLRenderContext::CreateGeometryShader(sizei size, const void* binary, uint format, bool& success)
{
	GLGeometryShader* shader = new GLGeometryShader;
	success = shader->Create(size, binary, format);
	return shader;
}

IGeometryShader* GLRenderContext::CreateGeometryShaderWithTransformFeedback(sizei count, const char** source, sizei attrib_count, const char** attrib_names, bool& success)
{
	GLGeometryShader* shader = new GLGeometryShader;
	success = shader->CreateWithTransformFeedback(count, source, attrib_count, attrib_names);
	return shader;
}

IGeometryShader* GLRenderContext::CreateGeometryShaderWithTransformFeedback(sizei size, const void* binary, uint format, sizei attrib_count, const char** attrib_names, bool& success)
{
	GLGeometryShader* shader = new GLGeometryShader;
	success = shader->CreateWithTransformFeedback(size, binary, format, attrib_count, attrib_names);
	return shader;
}

IFragmentShader* GLRenderContext::CreateFragmentShader(sizei count, const char** source, bool& success)
{
	GLFragmentShader* shader = new GLFragmentShader;
	success = shader->Create(count, source);
	return shader;
}

IFragmentShader* GLRenderContext::CreateFragmentShader(sizei size, const void* binary, uint format, bool& success)
{
	GLFragmentShader* shader = new GLFragmentShader;
	success = shader->Create(size, binary, format);
	return shader;
}

IComputeShader* GLRenderContext::CreateComputeShader(sizei count, const char** source, bool& success)
{
	GLComputeShader* shader = new GLComputeShader;
	success = shader->Create(count, source);
	return shader;
}

IComputeShader* GLRenderContext::CreateComputeShader(sizei size, const void* binary, uint format, bool& success)
{
	GLComputeShader* shader = new GLComputeShader;
	success = shader->Create(size, binary, format);
	return shader;
}

void GLRenderContext::DestroyShader(IShader* shader)
{
	if (shader)
	{
		dyn_cast_ptr<GLShader*>(shader)->Destroy();
		delete shader;
	}
}

ITransformFeedback* GLRenderContext::CreateTransformFeedback()
{
	GLTransformFeedback* transform_feedback = new GLTransformFeedback;
	if (!transform_feedback->Create(&_glState))
	{
		delete transform_feedback;
		transform_feedback = nullptr;
	}

	return transform_feedback;
}

void GLRenderContext::DestroyTransformFeedback(ITransformFeedback* transform_feedback)
{
	if (transform_feedback)
	{
		GLTransformFeedback* tfb = dyn_cast_ptr<GLTransformFeedback*>(transform_feedback);
		
		if (_glState.transformFeedback == tfb->GetID())
			_glState.transformFeedback = 0;

		tfb->Destroy();
		delete transform_feedback;
	}
}

static void APIENTRY DebugMessageProc(
	GLenum source, GLenum type, GLuint id, GLenum severity,
	GLsizei length, const char* message, const void* user_param)
{
	IDebugLogger* logger = (IDebugLogger*)user_param;
	if (logger)
	{
		logger->DebugMessage(GetFromGLEnum<DebugMessageSource>(source), GetFromGLEnum<DebugMessageType>(type), id, GetFromGLEnum<DebugMessageSeverity>(severity), message);
	}
}

void GLRenderContext::EnableDebugOutput(bool enable, bool synchronous)
{
	if (enable)
	{
		glEnable(GL_DEBUG_OUTPUT);

		if (synchronous)
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		else
			glDisable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

		glDebugMessageCallback(DebugMessageProc, _logger);
	}
	else
	{
		glDisable(GL_DEBUG_OUTPUT);
	}
}

void GLRenderContext::EnableDebugMessages(DebugMessageSource source, DebugMessageType type, DebugMessageSeverity severity, bool enable)
{
	glDebugMessageControl(GetGLEnum(source), GetGLEnum(type), GetGLEnum(severity), 0, nullptr, enable);
}

void GLRenderContext::EnableDebugMessages(DebugMessageSource source, DebugMessageType type, uint id_count, uint* ids, bool enable)
{
	if (source != DebugMessageSource::All && type != DebugMessageType::All)
	{
		glDebugMessageControl(GetGLEnum(source), GetGLEnum(type), GL_DONT_CARE, id_count, ids, enable);
	}
}

void GLRenderContext::EnableDebugMessage(DebugMessageSource source, DebugMessageType type, uint id, bool enable)
{
	if (source != DebugMessageSource::All && type != DebugMessageType::All)
	{
		glDebugMessageControl(GetGLEnum(source), GetGLEnum(type), GL_DONT_CARE, 1, &id, enable);
	}
}

void GLRenderContext::InsertDebugMessage(DebugMessageSource source, DebugMessageType type, uint id, DebugMessageSeverity severity, const char* message)
{
	if (source == DebugMessageSource::Application || source == DebugMessageSource::ThirdParty)
	{
		GLsizei length = std::min((int)strlen(message), _info.maxDebugMessageLength);
		glDebugMessageInsert(GetGLEnum(source), GetGLEnum(type), id, GetGLEnum(severity), length, message);
	}
}

void GLRenderContext::PushDebugGroup(DebugMessageSource source, uint id, const char* message)
{
	if (source == DebugMessageSource::Application || source == DebugMessageSource::ThirdParty)
	{
		GLsizei length = std::min((int)strlen(message), _info.maxDebugMessageLength);
		glPushDebugGroup(GetGLEnum(source), id, length, message);
	}
}

void GLRenderContext::PopDebugGroup()
{
	glPopDebugGroup();
}

int GLRenderContext::GetDebugGroupStackDepth()
{
	int depth;
	glGetIntegerv(GL_DEBUG_GROUP_STACK_DEPTH, &depth);
	return depth;
}

void GLRenderContext::ResourceDebugLabel(IResource* resource, const char* label)
{
	glObjectLabel(GetGLEnum(resource->GetType()), dyn_cast_ptr<GLResource*>(resource)->GetID(), -1, label);
}

void GLRenderContext::SyncObjectDebugLabel(SyncObject sync_object, const char* label)
{
	glObjectPtrLabel(sync_object, -1, label);
}

} // namespace gls::internals
