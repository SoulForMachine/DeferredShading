
#ifndef _GL_RENDER_CONTEXT_H_
#define _GL_RENDER_CONTEXT_H_

#include "GLCommon.h"
#include "GLSlayer/RenderContext.h"
#include "GLFramebuffer.h"
#include "GLBuffer.h"
#include "GLVertexFormat.h"
#include "GLShader.h"
#include "GLTexture.h"
#include "GLSamplerState.h"
#include "GLQuery.h"
#include "GLTransformFeedback.h"
#include "GLContextInfo.h"
#include "GLSlayerMessages.h"



#define LOAD_EXTENSION(ext) \
	if(!glextLoad_##ext()) \
		DebugMessage(DebugMessageSource::ThirdParty, DebugMessageType::Error, DebugMessageSeverity::Notification, ErrorMessageId::UnsupportedExtension, #ext);

#define LOAD_EXTENSION_REQ(ext) \
	if(!glextLoad_##ext()){ \
		DebugMessage(DebugMessageSource::ThirdParty, DebugMessageType::Error, DebugMessageSeverity::High, ErrorMessageId::UnsupportedExtension, #ext); \
		result = false; } \


namespace gls::internals
{

class GLRenderContext : public IRenderContext
{
public:
	GLRenderContext(IDebugLogger* logger);
	~GLRenderContext();

	GLRenderContext(const GLRenderContext&) = delete;
	GLRenderContext& operator = (const GLRenderContext&) = delete;

#if defined(_WIN32)
	bool Create(uint version, HINSTANCE instance_handle, HWND window_handle, const FramebufferFormat& format, bool debug_context, IRenderContext* shareContext);
	bool SetWindowCompatiblePixelFormat(HWND windowHandle);
	HWND SetContextWindow(HWND windowHandle);
#elif defined(__linux__)
	bool Create(uint version, Display* display, Window window, const FramebufferFormat& format, bool debug_context, IRenderContext* shareContext);
	Window SetContextWindow(Window window);
#endif

	void Destroy();

	virtual bool SetCurrentContext() override;
	virtual void UnsetCurrentContext() override;
	virtual const ContextInfo& GetInfo() const override;

	// vertex stream
	virtual void VertexSource(int stream, IBuffer* buffer, sizei stride, intptr offset, uint divisor) override;
	virtual void IndexSource(IBuffer* buffer, DataType index_type) override;
	virtual void ActiveVertexFormat(IVertexFormat* format) override;
	virtual void EnablePrimitiveRestart(bool enable) override;
	virtual void EnablePrimitiveRestartFixedIndex(bool enable) override;
	virtual void PrimitiveRestartIndex(uint index) override;
	virtual void ProvokingVertex(VertexConvention vertex_convention) override;

	// tessellation
	virtual void PatchVertexCount(int count) override;
	virtual void PatchDefaultOuterLevels(const float values[4]) override;
	virtual void PatchDefaultInnerLevels(const float values[2]) override;

	// conditional render
	virtual void BeginConditionalRender(IQuery* query, ConditionalRenderQueryMode mode) override;
	virtual void EndConditionalRender() override;

	// transform feedback
	virtual void BeginTransformFeedback(PrimitiveType primitive, ITransformFeedback* transform_feedback) override;
	virtual void EndTransformFeedback() override;
	virtual void PauseTransformFeedback() override;
	virtual void ResumeTransformFeedback() override;

	// viewport transform
	virtual void Viewport(int x, int y, int width, int height) override;
	virtual void ViewportIndexed(uint index, float x, float y, float width, float height) override;
	virtual void ClipControl(ClipOrigin origin, ClipDepth depth) override;

	// back face culling
	virtual void EnableFaceCulling(bool enable) override;
	virtual void CullFace(PolygonFace face) override;
	virtual void FrontFace(VertexWinding orient) override;

	// rasterization
	virtual void RasterizationMode(RasterMode mode) override;
	virtual void EnableRasterizerDiscard(bool enable) override;
	virtual void LineWidth(float width) override;
	virtual void EnableLineAntialiasing(bool enable) override;

	// multisampling
	virtual void EnableMultisampling(bool enable) override;
	virtual void EnableSampleAlphaToCoverage(bool enable) override;
	virtual void EnableSampleAlphaToOne(bool enable) override;
	virtual void EnableSampleCoverage(bool enable) override;
	virtual void EnableSampleShading(bool enable) override;
	virtual void SampleCoverage(float value, bool invert) override;
	virtual void SampleMask(uint index, uint mask) override;
	virtual void GetSamplePosition(uint index, float position[2]) override;
	virtual void MinSampleShading(float value) override;

	// scissor test
	virtual void EnableScissorTest(bool enable) override;
	virtual void EnableScissorTestIndexed(uint index, bool enable) override;
	virtual void Scissor(int x, int y, int width, int height) override;
	virtual void ScissorIndexed(uint index, int x, int y, int width, int height) override;

	// depth test
	virtual void EnableDepthTest(bool enable) override;
	virtual void DepthTestFunc(CompareFunc func) override;
	virtual void DepthRange(float dnear, float dfar) override;
	virtual void DepthRangeIndexed(uint index, float dnear, float dfar) override;
	virtual void DepthOffset(float factor, float units) override;
	virtual void EnableDepthClamp(bool enable) override;

	// stencil test
	virtual void EnableStencilTest(bool enable) override;
	virtual void StencilTestFunc(PolygonFace face, CompareFunc func, int ref, uint mask) override;
	virtual void StencilOperation(PolygonFace face, StencilOp stencil_fail, StencilOp depth_fail, StencilOp depth_pass) override;

	// blending
	virtual void EnableBlending(bool enable) override;
	virtual void EnableBlending(uint buffer, bool enable) override;
	virtual void BlendingColor(const float color[4]) override;
	virtual void BlendingFunc(BlendFunc src_factor, BlendFunc dest_factor) override;
	virtual void BlendingFunc(BlendFunc src_rgb_factor, BlendFunc dest_rgb_factor, BlendFunc src_alpha_factor, BlendFunc dest_alpha_factor) override;
	virtual void BlendingFunc(uint buffer, BlendFunc src_factor, BlendFunc dest_factor) override;
	virtual void BlendingFunc(uint buffer, BlendFunc src_rgb_factor, BlendFunc dest_rgb_factor, BlendFunc src_alpha_factor, BlendFunc dest_alpha_factor) override;
	virtual void BlendingOperation(BlendOp op) override;
	virtual void BlendingOperation(BlendOp op_rgb, BlendOp op_alpha) override;
	virtual void BlendingOperation(uint buffer, BlendOp op) override;
	virtual void BlendingOperation(uint buffer, BlendOp op_rgb, BlendOp op_alpha) override;

	// logic operation
	virtual void EnableLogicOperation(bool enable) override;
	virtual void LogicOperation(LogicOp op) override;

	// framebuffer
	virtual void SetFramebuffer(IFramebuffer* fbuf) override;
	virtual void ActiveColorBuffers(IFramebuffer* fbuf, const ColorBuffer* buffers, sizei count) override;
	virtual void EnableFramebufferSRGB(bool enable) override;
	virtual void EnableColorWrite(bool r, bool g, bool b, bool a) override;
	virtual void EnableColorWrite(uint buffer, bool r, bool g, bool b, bool a) override;
	virtual void EnableDepthWrite(bool enable) override;
	virtual void EnableStencilWrite(PolygonFace face, uint mask) override;
	virtual void ClearColorBuffer(IFramebuffer* fbuf, uint buffer, const float color[4]) override;
	virtual void ClearColorBuffer(IFramebuffer* fbuf, uint buffer, const int color[4]) override;
	virtual void ClearColorBuffer(IFramebuffer* fbuf, uint buffer, const uint color[4]) override;
	virtual void ClearDepthBuffer(IFramebuffer* fbuf, float depth) override;
	virtual void ClearStencilBuffer(IFramebuffer* fbuf, int stencil) override;
	virtual void ClearDepthStencilBuffer(IFramebuffer* fbuf, float depth, int stencil) override;
	virtual void ReadPixels(IFramebuffer* source_fbuf, ColorBuffer source_color_buf, ColorReadClamp color_clamp, int x, int y, sizei width, sizei height, ImageFormat format, DataType type, const PixelStore* pixel_store, void* buffer) override;
	virtual void ReadPixels(IFramebuffer* source_fbuf, ColorBuffer source_color_buf, ColorReadClamp color_clamp, int x, int y, sizei width, sizei height, ImageFormat format, DataType type, const PixelStore* pixel_store, IBuffer* buffer, intptr buffer_offset) override;
	virtual void BlitFramebuffer(IFramebuffer* source_fbuf, ColorBuffer source_color_buf, int src_x0, int src_y0, int src_x1, int src_y1, IFramebuffer* dest_fbuf, int dest_x0, int dest_y0, int dest_x1, int dest_y1, uint buffers, TexFilter filter) override;
	virtual void SwapBuffers() override;
	virtual void SwapInterval(int interval) override;

	// shaders
	virtual void SetVertexShader(IVertexShader* shader) override;
	virtual void SetTessControlShader(ITessControlShader* shader) override;
	virtual void SetTessEvaluationShader(ITessEvaluationShader* shader) override;
	virtual void SetGeometryShader(IGeometryShader* shader) override;
	virtual void SetFragmentShader(IFragmentShader* shader) override;
	virtual void SetComputeShader(IComputeShader* shader) override;
	virtual void SetUniformBuffer(uint index, IBuffer* buffer) override;
	virtual void SetUniformBuffer(uint index, IBuffer* buffer, intptr offset, sizeiptr size) override;
	virtual void SetAtomicCounterBuffer(uint index, IBuffer* buffer) override;
	virtual void SetAtomicCounterBuffer(uint index, IBuffer* buffer, intptr offset, sizeiptr size) override;
	virtual void SetStorageBuffer(uint index, IBuffer* buffer) override;
	virtual void SetStorageBuffer(uint index, IBuffer* buffer, intptr offset, sizeiptr size) override;
	virtual void UniformSubroutine(ShaderType shader_type, sizei count, const uint* indices) override;
	virtual void SetImageTexture(uint image_unit, ITexture* texture, int level, bool layered, int layer, BufferAccess access, PixelFormat format) override;
	virtual bool ValidateShaderPipeline() override;

	// textures and samplers
	virtual void SetSamplerTexture(int sampler, ITexture* texture) override;
	virtual void SetSamplerState(int sampler, ISamplerState* state) override;
	virtual void EnableSeamlessCubeMap(bool enable) override;

	// drawing commands
	virtual void Draw(PrimitiveType prim, int first, sizei count) override;
	virtual void DrawInstanced(PrimitiveType prim, int first, sizei count, uint base_inst, sizei inst_count) override;
	virtual void DrawIndirect(PrimitiveType prim, IBuffer* buffer, intptr offset) override;
	virtual void MultiDrawIndirect(PrimitiveType prim, IBuffer* buffer, intptr offset, sizei count, sizei stride) override;
	virtual void DrawIndexed(PrimitiveType prim, intptr index_start, int base_vertex, sizei count) override;
	virtual void DrawIndexed(PrimitiveType prim, uint start, uint end, intptr index_start, int base_vertex, sizei count) override;
	virtual void DrawIndexedInstanced(PrimitiveType prim, intptr index_start, int base_vertex, sizei count, uint base_inst, sizei inst_count) override;
	virtual void DrawIndexedIndirect(PrimitiveType prim, IBuffer* buffer, intptr offset) override;
	virtual void MultiDrawIndexedIndirect(PrimitiveType prim, IBuffer* buffer, intptr offset, sizei count, sizei stride) override;
	virtual void DrawTransformFeedback(PrimitiveType prim, ITransformFeedback* transform_feedback, uint stream) override;
	virtual void DrawTransformFeedbackInstanced(PrimitiveType prim, ITransformFeedback* transform_feedback, uint stream, sizei inst_count) override;

	// compute commands
	virtual void DispatchCompute(uint num_groups_x, uint num_groups_y, uint num_groups_z) override;
	virtual void DispatchComputeIndirect(IBuffer* buffer, intptr offset) override;

	// synchronization
	virtual void Flush() override;
	virtual void Finish() override;
	virtual SyncObject InsertFenceSync(FenceSyncCondition condition, uint flags) override;
	virtual void DeleteSync(SyncObject sync) override;
	virtual SyncWaitStatus ClientWaitSync(SyncObject sync, uint flags, uint64 timeout) override;
	virtual void Wait(SyncObject sync, uint flags, uint64 timeout) override;
	virtual void MemoryBarrier(uint flags) override;
	virtual void MemoryBarrierByRegion(uint flags) override;
	virtual void TextureBarrier() override;

	// buffer and image copying
	virtual void CopyBufferData(IBuffer* source, intptr source_offset, IBuffer* dest, intptr dest_offset, sizeiptr size) override;
	virtual void CopyTextureData(
		ITexture* source, int source_level, int source_x, int source_y, int source_z, sizei width, sizei height, sizei depth,
		ITexture* dest, int dest_level, int dest_x, int dest_y, int dest_z) override;
	virtual void CopyRenderbufferData(IRenderbuffer* source, int source_x, int source_y, sizei width, sizei height, IRenderbuffer* dest, int dest_x, int dest_y) override;

	// state queries
	virtual void GetViewport(int viewport[4]) override;
	virtual void GetViewport(float viewport[4]) override;
	virtual void GetViewportIndexed(uint index, float viewport[4]) override;
	virtual int64 GetGPUTimestamp() override;
	virtual bool GetTextureInternalFormatInfo(TextureType type, PixelFormat internal_format, InternalFormatInfo& info) override;
	virtual bool GetRenderbufferInternalFormatInfo(PixelFormat internal_format, InternalFormatInfo& info) override;
	virtual ErrorCode GetLastError() override;

	// object creation

	virtual IVertexFormat* CreateVertexFormat(const VertexAttribDesc* descriptors, int count) override;
	virtual void DestroyVertexFormat(IVertexFormat* vert_fmt) override;

	virtual ISamplerState* CreateSamplerState(const SamplerStateDesc& descriptor) override;
	virtual void DestroySamplerState(ISamplerState* samp_state) override;

	virtual ITexture1D* CreateTexture1D(sizei levels, PixelFormat internal_format, int width) override;
	virtual ITexture1D* CreateTexture1DView(ITexture1D* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels) override;
	virtual ITexture1D* CreateTexture1DView(ITexture1DArray* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint layer) override;

	virtual ITexture1DArray* CreateTexture1DArray(sizei levels, PixelFormat internal_format, int width, int height) override;
	virtual ITexture1DArray* CreateTexture1DArrayView(ITexture1D* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels) override;
	virtual ITexture1DArray* CreateTexture1DArrayView(ITexture1DArray* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint min_layer, uint num_layers) override;

	virtual ITexture2D* CreateTexture2D(sizei levels, PixelFormat internal_format, int width, int height) override;
	virtual ITexture2D* CreateTexture2DView(ITexture2D* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels) override;
	virtual ITexture2D* CreateTexture2DView(ITexture2DArray* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint layer) override;
	virtual ITexture2D* CreateTexture2DView(ITextureCube* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint layer) override;
	virtual ITexture2D* CreateTexture2DView(ITextureCubeArray* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint layer) override;

	virtual ITexture2DArray* CreateTexture2DArray(sizei levels, PixelFormat internal_format, int width, int height, int depth) override;
	virtual ITexture2DArray* CreateTexture2DArrayView(ITexture2D* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels) override;
	virtual ITexture2DArray* CreateTexture2DArrayView(ITexture2DArray* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint min_layer, uint num_layers) override;
	virtual ITexture2DArray* CreateTexture2DArrayView(ITextureCube* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint min_layer, uint num_layers) override;
	virtual ITexture2DArray* CreateTexture2DArrayView(ITextureCubeArray* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint min_layer, uint num_layers) override;

	virtual ITexture2DMultisample* CreateTexture2DMultisample(int samples, PixelFormat internal_format, int width, int height, bool fixed_sample_locations) override;
	virtual ITexture2DMultisample* CreateTexture2DMultisampleView(ITexture2DMultisample* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels) override;
	virtual ITexture2DMultisample* CreateTexture2DMultisampleView(ITexture2DMultisampleArray* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels) override;

	virtual ITexture2DMultisampleArray* CreateTexture2DMultisampleArray(int samples, PixelFormat internal_format, int width, int height, int depth, bool fixed_sample_locations) override;
	virtual ITexture2DMultisampleArray* CreateTexture2DMultisampleArrayView(ITexture2DMultisample* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels) override;
	virtual ITexture2DMultisampleArray* CreateTexture2DMultisampleArrayView(ITexture2DMultisampleArray* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint min_layer, uint num_layers) override;

	virtual ITexture3D* CreateTexture3D(sizei levels, PixelFormat internal_format, int width, int height, int depth) override;
	virtual ITexture3D* CreateTexture3DView(ITexture3D* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels) override;

	virtual ITextureCube* CreateTextureCube(sizei levels, PixelFormat internal_format, int width) override;
	virtual ITextureCube* CreateTextureCubeView(ITextureCube* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels) override;
	virtual ITextureCube* CreateTextureCubeView(ITextureCubeArray* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint min_layer, uint num_layers) override;
	virtual ITextureCube* CreateTextureCubeView(ITexture2DArray* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint min_layer, uint num_layers) override;

	virtual ITextureCubeArray* CreateTextureCubeArray(sizei levels, PixelFormat internal_format, int width, int depth) override;
	virtual ITextureCubeArray* CreateTextureCubeArrayView(ITextureCube* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint min_layer, uint num_layers) override;
	virtual ITextureCubeArray* CreateTextureCubeArrayView(ITextureCubeArray* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint min_layer, uint num_layers) override;
	virtual ITextureCubeArray* CreateTextureCubeArrayView(ITexture2DArray* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels, uint min_layer, uint num_layers) override;

	virtual ITextureBuffer* CreateTextureBuffer() override;

	virtual ITextureRectangle* CreateTextureRectangle(sizei levels, PixelFormat internal_format, int width, int height) override;
	virtual ITextureRectangle* CreateTextureRectangleView(ITextureRectangle* orig_tex, PixelFormat internal_format, uint min_level, uint num_levels) override;

	virtual void DestroyTexture(ITexture* texture) override;

	virtual IBuffer* CreateBuffer(sizeiptr size, const void* data, uint flags) override;
	virtual void DestroyBuffer(IBuffer* buffer) override;

	virtual IFramebuffer* CreateFramebuffer() override;
	virtual IFramebuffer* CreateFramebufferWithoutAttachments(const FramebufferParams& params) override;
	virtual void DestroyFramebuffer(IFramebuffer* framebuffer) override;
	virtual IRenderbuffer* CreateRenderbuffer(sizei samples, PixelFormat internal_format, sizei width, sizei height) override;
	virtual void DestroyRenderbuffer(IRenderbuffer* renderbuffer) override;

	virtual IQuery* CreateQuery() override;
	virtual void DestroyQuery(IQuery* query) override;

	virtual IVertexShader* CreateVertexShader(sizei count, const char** source, bool& success) override;
	virtual IVertexShader* CreateVertexShader(sizei size, const void* binary, uint format, bool& success) override;
	virtual IVertexShader* CreateVertexShaderWithTransformFeedback(sizei count, const char** source, sizei attrib_count, const char** attrib_names, bool& success) override;
	virtual IVertexShader* CreateVertexShaderWithTransformFeedback(sizei size, const void* binary, uint format, sizei attrib_count, const char** attrib_names, bool& success) override;

	virtual ITessControlShader* CreateTessControlShader(sizei count, const char** source, bool& success) override;
	virtual ITessControlShader* CreateTessControlShader(sizei size, const void* binary, uint format, bool& success) override;
	virtual ITessControlShader* CreateTessControlShaderWithTransformFeedback(sizei count, const char** source, sizei attrib_count, const char** attrib_names, bool& success) override;
	virtual ITessControlShader* CreateTessControlShaderWithTransformFeedback(sizei size, const void* binary, uint format, sizei attrib_count, const char** attrib_names, bool& success) override;

	virtual ITessEvaluationShader* CreateTessEvaluationShader(sizei count, const char** source, bool& success) override;
	virtual ITessEvaluationShader* CreateTessEvaluationShader(sizei size, const void* binary, uint format, bool& success) override;
	virtual ITessEvaluationShader* CreateTessEvaluationShaderWithTransformFeedback(sizei count, const char** source, sizei attrib_count, const char** attrib_names, bool& success) override;
	virtual ITessEvaluationShader* CreateTessEvaluationShaderWithTransformFeedback(sizei size, const void* binary, uint format, sizei attrib_count, const char** attrib_names, bool& success) override;

	virtual IGeometryShader* CreateGeometryShader(sizei count, const char** source, bool& success) override;
	virtual IGeometryShader* CreateGeometryShader(sizei size, const void* binary, uint format, bool& success) override;
	virtual IGeometryShader* CreateGeometryShaderWithTransformFeedback(sizei count, const char** source, sizei attrib_count, const char** attrib_names, bool& success) override;
	virtual IGeometryShader* CreateGeometryShaderWithTransformFeedback(sizei size, const void* binary, uint format, sizei attrib_count, const char** attrib_names, bool& success) override;

	virtual IFragmentShader* CreateFragmentShader(sizei count, const char** source, bool& success) override;
	virtual IFragmentShader* CreateFragmentShader(sizei size, const void* binary, uint format, bool& success) override;

	virtual IComputeShader* CreateComputeShader(sizei count, const char** source, bool& success) override;
	virtual IComputeShader* CreateComputeShader(sizei size, const void* binary, uint format, bool& success) override;

	virtual void DestroyShader(IShader* shader) override;

	virtual ITransformFeedback* CreateTransformFeedback() override;
	virtual void DestroyTransformFeedback(ITransformFeedback* transform_feedback) override;

	// debugging
	virtual void EnableDebugOutput(bool enable, bool synchronous) override;
	virtual void EnableDebugMessages(DebugMessageSource source, DebugMessageType type, DebugMessageSeverity severity, bool enable) override;
	virtual void EnableDebugMessages(DebugMessageSource source, DebugMessageType type, uint id_count, uint* ids, bool enable) override;
	virtual void EnableDebugMessage(DebugMessageSource source, DebugMessageType type, uint id, bool enable) override;
	virtual void InsertDebugMessage(DebugMessageSource source, DebugMessageType type, uint id, DebugMessageSeverity severity, const char* message) override;
	virtual void PushDebugGroup(DebugMessageSource source, uint id, const char* message) override;
	virtual void PopDebugGroup() override;
	virtual int GetDebugGroupStackDepth() override;
	virtual void ResourceDebugLabel(IResource* resource, const char* label) override;
	virtual void SyncObjectDebugLabel(SyncObject sync_object, const char* label) override;

private:
	bool InitCommon(uint version);
	void DeinitCommon();
	bool CreateContext(uint version, const FramebufferFormat& format, bool debug_context, IRenderContext* shareContext);
	void GetContextInfo();
	bool LoadOpenGLExtensions(uint version);
	bool LoadPlatformOpenGLExtensions();
	bool IsExtSupported(const char* extension);
	void Clear();
	void DelayedDrawingStateSetup();
	bool GetInternalFormatInfo(GLenum type, GLenum internal_format, InternalFormatInfo& info);
	void DebugMessage(DebugMessageSource source, DebugMessageType type, DebugMessageSeverity severity, ErrorMessageId message_id, ...);

	// Include declarations of extension loading functions.
	// Declare platform specific data.
#if defined(_WIN32)

#include "extensions/glext_windows_load_decl.h"
#include "extensions/wglext_load_decl.h"

	HWND _hwnd;
	HGLRC _hglrc;
	HDC _hdc;
	HINSTANCE _instanceHandle;

#elif defined(__linux__)

#include "extensions/glext_linux_load_decl.h"
#include "extensions/glxext_load_decl.h"

	Display* _display;
	Window _window;
	GLXContext _context;

#endif

	GLContextInfo _info;
	bool _initialized;

	struct VertexStream
	{
		GLBuffer* buffer;
		sizei stride;
		intptr offset;
		uint divisor;
	};

	struct VertexAttrib
	{
		GLBuffer* buffer;
		sizei stride;
		intptr bufferBase;
		int numComponents;
		DataType type;
		bool integer;
		bool normalized;
		uint offset;
		uint divisor;
	};

	struct VertexAttribEnabledState
	{
		bool currentState;
		bool newState;
	};

	class NullLogger : public IDebugLogger
	{
	public:
		void DebugMessage(DebugMessageSource source, DebugMessageType type, uint id, DebugMessageSeverity severity, const char* message) { }
	};

	// current state
	GLState _glState;
	VertexStream* _vertexStreams;
	VertexAttrib* _vertexAttribs;
	GLVertexFormat* _vertexFormat;
	VertexAttribEnabledState* _vertAttribEnabledState;
	DataType _indexType;
	GLuint _pipeline;
	GLTexture* _unit0Texture;
	GLFramebuffer* _framebuffer;
	IDebugLogger* _logger;
	NullLogger _nullLogger;
};

} // namespace gls::internals

#endif // _GL_RENDER_CONTEXT_H_
