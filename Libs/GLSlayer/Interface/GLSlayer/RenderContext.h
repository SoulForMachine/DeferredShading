
#ifndef _GLSLAYER_RENDER_CONTEXT_H_
#define _GLSLAYER_RENDER_CONTEXT_H_

#include "Common.h"
#include "Framebuffer.h"
#include "Buffer.h"
#include "VertexFormat.h"
#include "Shader.h"
#include "Texture.h"
#include "SamplerState.h"
#include "Query.h"
#include "TransformFeedback.h"


namespace gls
{

	struct ContextInfo
	{
		const char* renderer;
		const char* versionString;
		int versionMajor;
		int versionMinor;
		const char* vendor;
		const char* shadingLanguageVersion;

		FramebufferFormat framebufferFormat;

		int maxRecomendedVertexCount;
		int maxRecomendedIndexCount;
		int maxTextureSize;
		int max3DTextureSize;
		int maxCubeMapSize;
		int maxRectangleTextureSize;
		float maxTextureLODBias;
		int maxViewportWidth;
		int maxViewportHeight;
		int maxVertexAttribs;
		int maxCombinedTextureImageUnits;
		float maxTextureAnisotropy;
		int maxDrawBuffers;
		int maxTextureBufferSize;
		int maxElementVertices;
		int maxElementIndices;
		int maxUniformBlockSize;
		int maxCombinedUniformBlocks;
		int uniformBufferOffsetAlignment;
		int64 maxServerWaitTimeout;
		int maxSampleMaskWords;
		int maxColorTextureSamples;
		int maxDepthTextureSamples;
		int maxIntegerSamples;
		int maxSamples;
		int numSamples;
		int numSampleBuffers;
		int maxGeometryShaderInvocations;
		int minFragmentInterpolationOffset;
		int maxFragmentInterpolationOffset;
		int fragmentInterpolationOffsetBits;
		int maxVertexStreams;
		float minSampleShadingValue;
		int minTexGatherOffset;
		int maxTexGatherOffset;
		//int maxTexGatherComponents;
		int maxTransformFeedbackBuffers;
		int maxSubroutines;
		int maxSubroutineUniformLocations;
		int maxPatchVertices;
		int maxTessGenLevel;
		int maxTessControlUniformComponents;
		int maxTessEvaluationUniformComponents;
		int maxTessControlTextureImageUnits;
		int maxTessEvaluationTextureImageUnits;
		int maxTessControlOutputComponents;
		int maxTessPatchComponents;
		int maxTessControlTotalOutputComponents;
		int maxTessEvaluationOutputComponents;
		int maxTessUniformBlocks;
		int maxTessEvaluationUniformBlocks;
		int maxTessControlInputComponents;
		int maxTessEvaluationInputComponents;
		int maxCombinedTessControlUniformComponents;
		int maxCombinedTessEvaluationUniformComponents;
		int maxViewports;
		int viewportSubpixelBits;
		float viewportBoundsRange[2];
		int minMapBufferAlignment;
		int maxVertexAtomicCounterBuffers;
		int maxTessControlAtomicCounterBuffers;
		int maxTessEvaluationAtomicCounterBuffers;
		int maxGeometryAtomicCounterBuffers;
		int maxFragmentAtomicCounterBuffers;
		int maxCombinedAtomicCounterBuffers;
		int maxVertexAtomicCounters;
		int maxTessControlAtomicCounters;
		int maxTessEvaluationAtomicCounters;
		int maxGeometryAtomicCounters;
		int maxFragmentAtomicCounters;
		int maxCombinedAtomicCounters;
		int maxAtomicCounterBufferSize;
		int maxAtomicCounterBufferBindings;
		int maxImageUnits;
		int maxCombinedShaderOutputResources;
		int maxImageSamplers;
		int maxVertexImageUniforms;
		int maxTessControlImageUniforms;
		int maxTessEvaluationImageUniforms;
		int maxGeometryImageUniforms;
		int maxFragmentImageUniforms;
		int maxCombinedImageUniforms;
		int maxComputeUniformBlocks;
		int maxComputeTextureImageUnits;
		int maxComputeImageUniforms;
		int maxComputeSharedMemorySize;
		int maxComputeUniformComponents;
		int maxComputeAtomicCounterBuffers;
		int maxComputeAtomicCounters;
		int maxCombinedComputeUniformComponents;
		int maxComputeWorkGroupInvocations;
		int maxComputeWorkGroupCountX;
		int maxComputeWorkGroupCountY;
		int maxComputeWorkGroupCountZ;
		int maxComputeWorkGroupSizeX;
		int maxComputeWorkGroupSizeY;
		int maxComputeWorkGroupSizeZ;
		int maxUniformLocations;
		int maxFramebufferWidth;
		int maxFramebufferHeight;
		int maxFramebufferLayers;
		int maxFramebufferSamples;
		int maxVertexShaderStorageBlocks;
		int maxGeometryShaderStorageBlocks;
		int maxTessControlShaderStorageBlocks;
		int maxTessEvaluationShaderStorageBlocks;
		int maxFragmentShaderStorageBlocks;
		int maxComputeShaderStorageBlocks;
		int maxCombinedShaderStorageBlocks;
		int maxShaderStorageBufferBindings;
		int maxShaderStorageBlockSize;
		int shaderStorageBufferOffsetAlignment;
		int textureBufferOffsetAlignment;
		int maxDebugMessageLength;
		int maxDebugLoggedMessages;
		int maxDebugGroupStackDepth;
		int maxLabelLength;
		int maxVertexAttribBindings;
		int maxVertexAttribRelativeOffset;
		int maxCullDistances;
		int maxCombinedClipAndCullDistances;
	};


	struct InternalFormatInfo
	{
		int numSampleCounts;
		int samples[16];
		PixelFormat preferredInternalFormat;
		int redSize;
		int greenSize;
		int blueSize;
		int alphaSize;
		int depthSize;
		int stencilSize;
		int sharedSize;
		TextureComponentType redType;
		TextureComponentType greenType;
		TextureComponentType blueType;
		TextureComponentType alphaType;
		TextureComponentType depthType;
		TextureComponentType stencilType;
		int maxWidth;
		int maxHeight;
		int maxDepth;
		int maxLayers;
		int64 maxCombinedDimensions;
		bool colorComponents;
		bool depthComponents;
		bool stencilComponents;
		bool colorRenderable;
		bool depthRenderable;
		bool stencilRenderable;
		FeatureSupport framebufferRenderable;
		FeatureSupport framebufferRenderableLayered;
		FeatureSupport framebufferBlend;
		FeatureSupport readPixels;							// Support for reading pixels from the resource when it is attached to a framebuffer.
		ImageFormat readPixelsFormat;						// The format to pass to ReadPixels to obtain best performance and image quality when reading from a framebuffer.
		DataType readPixelsType;							// The type to pass to ReadPixels to obtain best performance and image quality when reading from a framebuffer.
		ImageFormat textureImageFormat;						// Preferred format to pass to TexImage and TexSubImage.
		DataType textureImageType;							// Preferred type to pass to TexImage and TexSubImage.
		ImageFormat getTextureImageFormat;					// Preferred format to pass to GetTexImage.
		DataType getTextureImageType;						// Preferred type to pass to GetTexImage.
		bool mipmap;										// True if resource supports mipmaps.
		FeatureSupport manualGenerateMipmap;				// Support for manually generating mipmaps for the resource.
		ColorEncoding colorEncoding;
		FeatureSupport sRGBRead;							// Support for converting from sRGB colorspace on read operations from the resource.
		FeatureSupport sRGBWrite;							// Support for converting to sRGB colorspace on write operations to the resource.
		FeatureSupport filter;								// Support for filter types other than NEAREST and NEAREST_MIPMAP_NEAREST.
		FeatureSupport vertexTexture;						// Support for using the resource for texture sampling in a vertex shader.
		FeatureSupport tessControlTexture;					// Support for using the resource for texture sampling in a tessellation control shader.
		FeatureSupport tessEvaluationTexture;				// Support for using the resource for texture sampling in a tessellation evaluation shader.
		FeatureSupport geometryTexture;						// Support for using the resource for texture sampling in a geometry shader.
		FeatureSupport fragmentTexture;						// Support for using the resource for texture sampling in a fragment shader.
		FeatureSupport computeTexture;						// Support for using the resource for texture sampling in a compute shader.
		FeatureSupport textureShadow;						// Support for using the resource with shadow samplers.
		FeatureSupport textureGather;						// Support for using the resource with texture gather operations.
		FeatureSupport textureGatherShadow;					// Support for using the resource with texture gather operations with shadow samplers.
		FeatureSupport shaderImageLoad;						// Support for using the resource with image load operations in shaders.
		FeatureSupport shaderImageStore;					// Support for using the resource with image store operations in shaders.
		FeatureSupport shaderImageAtomic;					// Support for using the resource with atomic memory operations from shaders.
		int imageTexelSize;									// Size of a texel when the resource is used as an image texture.
		ImageClass imageCompatibilityClass;					// Compatibility class of the resource when used as an image texture.
		ImageFormat imagePixelFormat;						// Format of the resource when used as an image texture.
		DataType imagePixelType;							// Type of the resource when used as an image texture.
		ImageFormatCompatibilityType imageFormatCompatibilityType;
		FeatureSupport simultaneousTextureAndDepthTest;
		FeatureSupport simultaneousTextureAndStencilTest;
		FeatureSupport simultaneousTextureAndDepthWrite;
		FeatureSupport simultaneousTextureAndStencilWrite;
		bool textureCompressed;
		int textureCompressedBlockWidth;
		int textureCompressedBlockHeight;
		int textureCompressedBlockSize;
		FeatureSupport clearBuffer;
		FeatureSupport textureView;							// Support for using resource with texture views.
		ViewClass viewCompatibilityClass;
	};

	struct DrawIndirectData
	{
		uint count;
		uint instanceCount;
		uint firstVertex;
		uint baseInstance;
	};

	struct DrawIndexedIndirectData
	{
		uint count;
		uint instanceCount;
		uint firstIndex;
		int baseVertex;
		uint baseInstance;
	};

	

	class IRenderContext
	{
	public:
		virtual ~IRenderContext() = default;
		virtual bool SetCurrentContext() = 0;
		virtual void UnsetCurrentContext() = 0;
		virtual const ContextInfo& GetInfo() const = 0;

		// vertex stream
		virtual void VertexSource(int stream, IBuffer* buffer, sizei stride, intptr offset, uint divisor) = 0;
		virtual void IndexSource(IBuffer* buffer, DataType index_type) = 0;
		virtual void ActiveVertexFormat(IVertexFormat* format) = 0;
		virtual void EnablePrimitiveRestart(bool enable) = 0;
		virtual void EnablePrimitiveRestartFixedIndex(bool enable) = 0;
		virtual void PrimitiveRestartIndex(uint index) = 0;
		virtual void ProvokingVertex(VertexConvention vertex_convention) = 0;

		// tessellation
		virtual void PatchVertexCount(int count) = 0;
		virtual void PatchDefaultOuterLevels(const float values[4]) = 0;
		virtual void PatchDefaultInnerLevels(const float values[2]) = 0;

		// conditional render
		virtual void BeginConditionalRender(IQuery* query, ConditionalRenderQueryMode mode) = 0;
		virtual void EndConditionalRender() = 0;

		// transform feedback
		virtual void BeginTransformFeedback(PrimitiveType primitive, ITransformFeedback* transform_feedback) = 0;
		virtual void EndTransformFeedback() = 0;
		virtual void PauseTransformFeedback() = 0;
		virtual void ResumeTransformFeedback() = 0;

		// viewport transform
		virtual void Viewport(int x, int y, int width, int height) = 0;
		virtual void ViewportIndexed(uint index, float x, float y, float width, float height) = 0;
		virtual void ClipControl(ClipOrigin origin, ClipDepth depth) = 0;

		// back face culling
		virtual void EnableFaceCulling(bool enable) = 0;
		virtual void CullFace(PolygonFace face) = 0;
		virtual void FrontFace(VertexWinding orient) = 0;

		// rasterization
		virtual void RasterizationMode(RasterMode mode) = 0;
		virtual void EnableRasterizerDiscard(bool enable) = 0;
		virtual void LineWidth(float width) = 0;
		virtual void EnableLineAntialiasing(bool enable) = 0;

		// multisampling
		virtual void EnableMultisampling(bool enable) = 0;
		virtual void EnableSampleAlphaToCoverage(bool enable) = 0;
		virtual void EnableSampleAlphaToOne(bool enable) = 0;
		virtual void EnableSampleCoverage(bool enable) = 0;
		virtual void EnableSampleShading(bool enable) = 0;
		virtual void SampleCoverage(float value, bool invert) = 0;
		virtual void SampleMask(uint index, uint mask) = 0;
		virtual void GetSamplePosition(uint index, float position[2]) = 0;
		virtual void MinSampleShading(float value) = 0;

		// scissor test
		virtual void EnableScissorTest(bool enable) = 0;
		virtual void EnableScissorTestIndexed(uint index, bool enable) = 0;
		virtual void Scissor(int x, int y, int width, int height) = 0;
		virtual void ScissorIndexed(uint index, int x, int y, int width, int height) = 0;

		// depth test
		virtual void EnableDepthTest(bool enable) = 0;
		virtual void DepthTestFunc(CompareFunc func) = 0;
		virtual void DepthRange(float dnear, float dfar) = 0;
		virtual void DepthRangeIndexed(uint index, float dnear, float dfar) = 0;
		virtual void DepthOffset(float factor, float units) = 0;
		virtual void EnableDepthClamp(bool enable) = 0;

		// stencil test
		virtual void EnableStencilTest(bool enable) = 0;
		virtual void StencilTestFunc(PolygonFace face, CompareFunc func, int ref, uint mask) = 0;
		virtual void StencilOperation(PolygonFace face, StencilOp stencil_fail, StencilOp depth_fail, StencilOp depth_pass) = 0;

		// blending
		virtual void EnableBlending(bool enable) = 0;
		virtual void EnableBlending(uint buffer, bool enable) = 0;
		virtual void BlendingColor(const float color[4]) = 0;
		virtual void BlendingFunc(BlendFunc src_factor, BlendFunc dest_factor) = 0;
		virtual void BlendingFunc(BlendFunc src_rgb_factor, BlendFunc dest_rgb_factor, BlendFunc src_alpha_factor, BlendFunc dest_alpha_factor) = 0;
		virtual void BlendingFunc(uint buffer, BlendFunc src_factor, BlendFunc dest_factor) = 0;
		virtual void BlendingFunc(uint buffer, BlendFunc src_rgb_factor, BlendFunc dest_rgb_factor, BlendFunc src_alpha_factor, BlendFunc dest_alpha_factor) = 0;
		virtual void BlendingOperation(BlendOp op) = 0;
		virtual void BlendingOperation(BlendOp op_rgb, BlendOp op_alpha) = 0;
		virtual void BlendingOperation(uint buffer, BlendOp op) = 0;
		virtual void BlendingOperation(uint buffer, BlendOp op_rgb, BlendOp op_alpha) = 0;

		// logic operation
		virtual void EnableLogicOperation(bool enable) = 0;
		virtual void LogicOperation(LogicOp op) = 0;

		// framebuffer
		virtual void SetFramebuffer(IFramebuffer* fbuf) = 0;
		virtual void ActiveColorBuffers(IFramebuffer* fbuf, const ColorBuffer* buffers, sizei count) = 0;
		virtual void EnableFramebufferSRGB(bool enable) = 0;
		virtual void EnableColorWrite(bool r, bool g, bool b, bool a) = 0;
		virtual void EnableColorWrite(uint buffer, bool r, bool g, bool b, bool a) = 0;
		virtual void EnableDepthWrite(bool enable) = 0;
		virtual void EnableStencilWrite(PolygonFace face, uint mask) = 0;
		virtual void ClearColorBuffer(IFramebuffer* fbuf, uint buffer, const float color[4]) = 0;
		virtual void ClearColorBuffer(IFramebuffer* fbuf, uint buffer, const int color[4]) = 0;
		virtual void ClearColorBuffer(IFramebuffer* fbuf, uint buffer, const uint color[4]) = 0;
		virtual void ClearDepthBuffer(IFramebuffer* fbuf, float depth) = 0;
		virtual void ClearStencilBuffer(IFramebuffer* fbuf, int stencil) = 0;
		virtual void ClearDepthStencilBuffer(IFramebuffer* fbuf, float depth, int stencil) = 0;
		virtual void ReadPixels(IFramebuffer* source_fbuf, ColorBuffer source_color_buf, ColorReadClamp color_clamp, int x, int y, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, void* buffer) = 0;
		virtual void ReadPixels(IFramebuffer* source_fbuf, ColorBuffer source_color_buf, ColorReadClamp color_clamp, int x, int y, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset) = 0;
		virtual void BlitFramebuffer(IFramebuffer* source_fbuf, ColorBuffer source_color_buf, int src_x0, int src_y0, int src_x1, int src_y1, IFramebuffer* dest_fbuf, int dest_x0, int dest_y0, int dest_x1, int dest_y1, uint buffers, TexFilter filter) = 0;
		virtual void SwapBuffers() = 0;
		virtual void SwapInterval(int interval) = 0;

		// shaders
		virtual void SetVertexShader(IVertexShader* shader) = 0;
		virtual void SetTessControlShader(ITessControlShader* shader) = 0;
		virtual void SetTessEvaluationShader(ITessEvaluationShader* shader) = 0;
		virtual void SetGeometryShader(IGeometryShader* shader) = 0;
		virtual void SetFragmentShader(IFragmentShader* shader) = 0;
		virtual void SetComputeShader(IComputeShader* shader) = 0;
		virtual void SetUniformBuffer(uint index, IBuffer* buffer) = 0;
		virtual void SetUniformBuffer(uint index, IBuffer* buffer, intptr offset, sizeiptr size) = 0;
		virtual void SetAtomicCounterBuffer(uint index, IBuffer* buffer) = 0;
		virtual void SetAtomicCounterBuffer(uint index, IBuffer* buffer, intptr offset, sizeiptr size) = 0;
		virtual void SetStorageBuffer(uint index, IBuffer* buffer) = 0;
		virtual void SetStorageBuffer(uint index, IBuffer* buffer, intptr offset, sizeiptr size) = 0;
		virtual void UniformSubroutine(ShaderType shader_type, sizei count, const uint* indices) = 0;
		virtual void SetImageTexture(uint image_unit, ITexture* texture, int level, bool layered, int layer, BufferAccess access, PixelFormat format) = 0;
		virtual bool ValidateShaderPipeline() = 0;

		// textures and samplers
		virtual void SetSamplerTexture(int sampler, ITexture* texture) = 0;
		virtual void SetSamplerState(int sampler, ISamplerState* state) = 0;
		virtual void EnableSeamlessCubeMap(bool enable) = 0;

		// drawing commands
		virtual void Draw(PrimitiveType prim, int first, sizei count) = 0;
		virtual void DrawInstanced(PrimitiveType prim, int first, sizei count, uint base_inst, sizei inst_count) = 0;
		virtual void DrawIndirect(PrimitiveType prim, IBuffer* buffer, intptr offset) = 0;
		virtual void MultiDrawIndirect(PrimitiveType prim, IBuffer* buffer, intptr offset, sizei count, sizei stride) = 0;
		virtual void DrawIndexed(PrimitiveType prim, intptr index_start, int base_vertex, sizei count) = 0;
		virtual void DrawIndexed(PrimitiveType prim, uint start, uint end, intptr index_start, int base_vertex, sizei count) = 0;
		virtual void DrawIndexedInstanced(PrimitiveType prim, intptr index_start, int base_vertex, sizei count, uint base_inst, sizei inst_count) = 0;
		virtual void DrawIndexedIndirect(PrimitiveType prim, IBuffer* buffer, intptr offset) = 0;
		virtual void MultiDrawIndexedIndirect(PrimitiveType prim, IBuffer* buffer, intptr offset, sizei count, sizei stride) = 0;
		virtual void DrawTransformFeedback(PrimitiveType prim, ITransformFeedback* transform_feedback, uint stream) = 0;
		virtual void DrawTransformFeedbackInstanced(PrimitiveType prim, ITransformFeedback* transform_feedback, uint stream, sizei inst_count) = 0;

		// compute commands
		virtual void DispatchCompute(uint num_groups_x, uint num_groups_y, uint num_groups_z) = 0;
		virtual void DispatchComputeIndirect(IBuffer* buffer, intptr offset) = 0;

		// synchronization
		virtual void Flush() = 0;
		virtual void Finish() = 0;
		virtual SyncObject InsertFenceSync(FenceSyncCondition condition, uint flags) = 0;
		virtual void DeleteSync(SyncObject sync) = 0;
		virtual SyncWaitStatus ClientWaitSync(SyncObject sync, uint flags, uint64 timeout) = 0;
		virtual void Wait(SyncObject sync, uint flags, uint64 timeout) = 0;
		virtual void MemoryBarrier(uint flags) = 0;
		virtual void MemoryBarrierByRegion(uint flags) = 0;
		virtual void TextureBarrier() = 0;

		// buffer and image copying
		virtual void CopyBufferData(IBuffer* source, intptr source_offset, IBuffer* dest, intptr dest_offset, sizeiptr size) = 0;
		virtual void CopyTextureData(
			ITexture* source, int source_level, int source_x, int source_y, int source_z, int width, int height, int depth,
			ITexture* dest, int dest_level, int dest_x, int dest_y, int dest_z) = 0;
		virtual void CopyRenderbufferData(IRenderbuffer* source, int source_x, int source_y, int width, int height, IRenderbuffer* dest, int dest_x, int dest_y) = 0;

		// state queries
		virtual void GetViewport(int viewport[4]) = 0;
		virtual void GetViewport(float viewport[4]) = 0;
		virtual void GetViewportIndexed(uint index, float viewport[4]) = 0;
		virtual int64 GetGPUTimestamp() = 0;
		virtual bool GetTextureInternalFormatInfo(TextureType type, PixelFormat internal_format, InternalFormatInfo& info) = 0;
		virtual bool GetRenderbufferInternalFormatInfo(PixelFormat internal_format, InternalFormatInfo& info) = 0;
		virtual ErrorCode GetLastError() = 0;

		// object creation

		virtual IVertexFormat* CreateVertexFormat(const VertexAttribDesc* descriptors, int count) = 0;
		virtual void DestroyVertexFormat(IVertexFormat* vert_fmt) = 0;

		virtual ISamplerState* CreateSamplerState(const SamplerStateDesc& descriptor) = 0;
		virtual void DestroySamplerState(ISamplerState* samp_state) = 0;

		virtual ITexture1D* CreateTexture1D(sizei levels, PixelFormat internal_format, int width) = 0;
		virtual ITexture1D* CreateTexture1DView(ITexture1D* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels) = 0;
		virtual ITexture1D* CreateTexture1DView(ITexture1DArray* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint layer) = 0;

		virtual ITexture1DArray* CreateTexture1DArray(sizei levels, PixelFormat internal_format, int width, int height) = 0;
		virtual ITexture1DArray* CreateTexture1DArrayView(ITexture1D* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels) = 0;
		virtual ITexture1DArray* CreateTexture1DArrayView(ITexture1DArray* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint min_layer, uint num_layers) = 0;

		virtual ITexture2D* CreateTexture2D(sizei levels, PixelFormat internal_format, int width, int height) = 0;
		virtual ITexture2D* CreateTexture2DView(ITexture2D* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels) = 0;
		virtual ITexture2D* CreateTexture2DView(ITexture2DArray* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint layer) = 0;
		virtual ITexture2D* CreateTexture2DView(ITextureCube* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint layer) = 0;
		virtual ITexture2D* CreateTexture2DView(ITextureCubeArray* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint layer) = 0;

		virtual ITexture2DArray* CreateTexture2DArray(sizei levels, PixelFormat internal_format, int width, int height, int depth) = 0;
		virtual ITexture2DArray* CreateTexture2DArrayView(ITexture2D* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels) = 0;
		virtual ITexture2DArray* CreateTexture2DArrayView(ITexture2DArray* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint min_layer, uint num_layers) = 0;
		virtual ITexture2DArray* CreateTexture2DArrayView(ITextureCube* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint min_layer, uint num_layers) = 0;
		virtual ITexture2DArray* CreateTexture2DArrayView(ITextureCubeArray* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint min_layer, uint num_layers) = 0;

		virtual ITexture2DMultisample* CreateTexture2DMultisample(int samples, PixelFormat internal_format, int width, int height, bool fixed_sample_locations) = 0;
		virtual ITexture2DMultisample* CreateTexture2DMultisampleView(ITexture2DMultisample* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels) = 0;
		virtual ITexture2DMultisample* CreateTexture2DMultisampleView(ITexture2DMultisampleArray* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels) = 0;

		virtual ITexture2DMultisampleArray* CreateTexture2DMultisampleArray(int samples, PixelFormat internal_format, int width, int height, int depth, bool fixed_sample_locations) = 0;
		virtual ITexture2DMultisampleArray* CreateTexture2DMultisampleArrayView(ITexture2DMultisample* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels) = 0;
		virtual ITexture2DMultisampleArray* CreateTexture2DMultisampleArrayView(ITexture2DMultisampleArray* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint min_layer, uint num_layers) = 0;

		virtual ITexture3D* CreateTexture3D(sizei levels, PixelFormat internal_format, int width, int height, int depth) = 0;
		virtual ITexture3D* CreateTexture3DView(ITexture3D* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels) = 0;

		virtual ITextureCube* CreateTextureCube(sizei levels, PixelFormat internal_format, int width) = 0;
		virtual ITextureCube* CreateTextureCubeView(ITextureCube* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels) = 0;
		virtual ITextureCube* CreateTextureCubeView(ITextureCubeArray* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint min_layer, uint num_layers) = 0;
		virtual ITextureCube* CreateTextureCubeView(ITexture2DArray* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint min_layer, uint num_layers) = 0;

		virtual ITextureCubeArray* CreateTextureCubeArray(sizei levels, PixelFormat internal_format, int width, int depth) = 0;
		virtual ITextureCubeArray* CreateTextureCubeArrayView(ITextureCube* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint min_layer, uint num_layers) = 0;
		virtual ITextureCubeArray* CreateTextureCubeArrayView(ITextureCubeArray* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint min_layer, uint num_layers) = 0;
		virtual ITextureCubeArray* CreateTextureCubeArrayView(ITexture2DArray* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint min_layer, uint num_layers) = 0;

		virtual ITextureBuffer* CreateTextureBuffer() = 0;

		virtual ITextureRectangle* CreateTextureRectangle(sizei levels, PixelFormat internal_format, int width, int height) = 0;
		virtual ITextureRectangle* CreateTextureRectangleView(ITextureRectangle* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels) = 0;

		virtual void DestroyTexture(ITexture* texture) = 0;

		virtual IBuffer* CreateBuffer(sizeiptr size, const void* data, uint flags) = 0;
		virtual void DestroyBuffer(IBuffer* buffer) = 0;

		virtual IFramebuffer* CreateFramebuffer() = 0;
		virtual IFramebuffer* CreateFramebufferWithoutAttachments(const FramebufferParams& params) = 0;
		virtual void DestroyFramebuffer(IFramebuffer* framebuffer) = 0;
		virtual IRenderbuffer* CreateRenderbuffer(sizei samples, gls::PixelFormat internal_format, sizei width, sizei height) = 0;
		virtual void DestroyRenderbuffer(IRenderbuffer* renderbuffer) = 0;

		virtual IQuery* CreateQuery() = 0;
		virtual void DestroyQuery(IQuery* query) = 0;

		virtual IVertexShader* CreateVertexShader(sizei count, const char** source, bool& success) = 0;
		virtual IVertexShader* CreateVertexShader(sizei size, const void* binary, uint format, bool& success) = 0;
		virtual IVertexShader* CreateVertexShaderWithTransformFeedback(sizei count, const char** source, sizei attrib_count, const char** attrib_names, bool& success) = 0;
		virtual IVertexShader* CreateVertexShaderWithTransformFeedback(sizei size, const void* binary, uint format, sizei attrib_count, const char** attrib_names, bool& success) = 0;

		virtual ITessControlShader* CreateTessControlShader(sizei count, const char** source, bool& success) = 0;
		virtual ITessControlShader* CreateTessControlShader(sizei size, const void* binary, uint format, bool& success) = 0;
		virtual ITessControlShader* CreateTessControlShaderWithTransformFeedback(sizei count, const char** source, sizei attrib_count, const char** attrib_names, bool& success) = 0;
		virtual ITessControlShader* CreateTessControlShaderWithTransformFeedback(sizei size, const void* binary, uint format, sizei attrib_count, const char** attrib_names, bool& success) = 0;

		virtual ITessEvaluationShader* CreateTessEvaluationShader(sizei count, const char** source, bool& success) = 0;
		virtual ITessEvaluationShader* CreateTessEvaluationShader(sizei size, const void* binary, uint format, bool& success) = 0;
		virtual ITessEvaluationShader* CreateTessEvaluationShaderWithTransformFeedback(sizei count, const char** source, sizei attrib_count, const char** attrib_names, bool& success) = 0;
		virtual ITessEvaluationShader* CreateTessEvaluationShaderWithTransformFeedback(sizei size, const void* binary, uint format, sizei attrib_count, const char** attrib_names, bool& success) = 0;

		virtual IGeometryShader* CreateGeometryShader(sizei count, const char** source, bool& success) = 0;
		virtual IGeometryShader* CreateGeometryShader(sizei size, const void* binary, uint format, bool& success) = 0;
		virtual IGeometryShader* CreateGeometryShaderWithTransformFeedback(sizei count, const char** source, sizei attrib_count, const char** attrib_names, bool& success) = 0;
		virtual IGeometryShader* CreateGeometryShaderWithTransformFeedback(sizei size, const void* binary, uint format, sizei attrib_count, const char** attrib_names, bool& success) = 0;

		virtual IFragmentShader* CreateFragmentShader(sizei count, const char** source, bool& success) = 0;
		virtual IFragmentShader* CreateFragmentShader(sizei size, const void* binary, uint format, bool& success) = 0;

		virtual IComputeShader* CreateComputeShader(sizei count, const char** source, bool& success) = 0;
		virtual IComputeShader* CreateComputeShader(sizei size, const void* binary, uint format, bool& success) = 0;

		virtual void DestroyShader(IShader* shader) = 0;

		virtual ITransformFeedback* CreateTransformFeedback() = 0;
		virtual void DestroyTransformFeedback(ITransformFeedback* transform_feedback) = 0;

		// debugging
		virtual void EnableDebugOutput(bool enable, bool synchronous) = 0;
		virtual void EnableDebugMessages(DebugMessageSource source, DebugMessageType type, DebugMessageSeverity severity, bool enable) = 0;
		virtual void EnableDebugMessages(DebugMessageSource source, DebugMessageType type, uint id_count, uint* ids, bool enable) = 0;
		virtual void EnableDebugMessage(DebugMessageSource source, DebugMessageType type, uint id, bool enable) = 0;
		virtual void InsertDebugMessage(DebugMessageSource source, DebugMessageType type, uint id, DebugMessageSeverity severity, const char* message) = 0;
		virtual void PushDebugGroup(DebugMessageSource source, uint id, const char* message) = 0;
		virtual void PopDebugGroup() = 0;
		virtual int GetDebugGroupStackDepth() = 0;
		virtual void ResourceDebugLabel(IResource* resource, const char* label) = 0;
		virtual void SyncObjectDebugLabel(SyncObject sync_object, const char* label) = 0;
	};

}


#endif // _GLSLAYER_RENDER_CONTEXT_H_
