
#ifndef _GLSLAYER_COMMON_H_
#define _GLSLAYER_COMMON_H_

#include <cstdint>
#include <cstddef>

#if defined(_WIN32)

#ifdef GLSLAYER_EXPORTS
#define GLSLAYER_API	__declspec(dllexport)
#else
#define GLSLAYER_API	__declspec(dllimport)
#endif

#elif defined(__linux__)

#ifdef GLSLAYER_EXPORTS
#define GLSLAYER_API	__attribute__((visibility("default")))
#else
#define GLSLAYER_API
#endif

#endif


namespace gls
{

	enum class ResourceType
	{
		Buffer,
		Shader,
		Texture,
		Framebuffer,
		Renderbuffer,
		TransformFeedback,
		SamplerState,
	};

	enum class BufferType
	{
		Vertex,
		Index,
		PixelPack,
		PixelUnpack,
		Texture,
		Uniform,
		TransformFeedback,
		DrawIndirect,
		DispatchIndirect,
		AtomicCounter,
		ShaderStorage,
		Query,
	};

	enum class ShaderType
	{
		Vertex,
		Geometry,
		TessControl,
		TessEvaluation,
		Fragment,
		Compute,
	};

	enum class TextureType
	{
		Tex1D,
		Tex2D,
		Tex2DMultisample,
		Tex3D,
		TexCube,
		Tex1DArray,
		Tex2DArray,
		Tex2DMultisampleArray,
		TexCubeArray,
		TexRectangle,
		TexBuffer,
	};

	enum class QueryType
	{
		Undefined,
		SamplesPassed,
		AnySamplesPassed,
		AnySamplesPassedConservative,
		PrimitivesGenerated,
		TransformFeedbackPrimitivesWritten,
		TimeElapsed,
		Timestamp,
	};

	enum class DataType
	{
		None,
		UnsignedByte,
		Byte,
		UnsignedShort,
		Short,
		UnsignedInt,
		Int,
		Int_2_10_10_10_Rev,
		HalfFloat,
		Float,
		Double,
		Fixed,
		UnsignedByte_3_3_2,
		UnsignedByte_2_3_2_Rev,
		UnsignedShort_5_6_5,
		UnsignedShort_5_6_5_Rev,
		UnsignedShort_4_4_4_4,
		UnsignedShort_4_4_4_4_Rev,
		UnsignedShort_5_5_5_1,
		UnsignedShort_1_5_5_5_Rev,
		UnsignedInt_8_8_8_8,
		UnsignedInt_8_8_8_8_Rev,
		UnsignedInt_10_10_10_2,
		UnsignedInt_2_10_10_10_Rev,
		UnsignedInt_24_8,
		UnsignedInt_10F_11F_11F_Rev,
		UnsignedInt_5_9_9_9_Rev,
		Float_32_UnsignedInt_24_8_Rev,
	};

	enum class ShaderDataType
	{
		Float,
		FloatVec2,
		FloatVec3,
		FloatVec4,
		Double,
		DoubleVec2,
		DoubleVec3,
		DoubleVec4,
		Int,
		IntVec2,
		IntVec3,
		IntVec4,
		UnsignedInt,
		UnsignedIntVec2,
		UnsignedIntVec3,
		UnsignedIntVec4,
		Bool,
		BoolVec2,
		BoolVec3,
		BoolVec4,
		FloatMat2,
		FloatMat3,
		FloatMat4,
		FloatMat2x3,
		FloatMat2x4,
		FloatMat3x2,
		FloatMat3x4,
		FloatMat4x2,
		FloatMat4x3,
		DoubleMat2,
		DoubleMat3,
		DoubleMat4,
		DoubleMat2x3,
		DoubleMat2x4,
		DoubleMat3x2,
		DoubleMat3x4,
		DoubleMat4x2,
		DoubleMat4x3,
		Sampler1D,
		Sampler2D,
		Sampler3D,
		SamplerCube,
		Sampler1DShadow,
		Sampler2DShadow,
		Sampler1DArray,
		Sampler2DArray,
		Sampler1DArrayShadow,
		Sampler2DArrayShadow,
		Sampler2DMultisample,
		Sampler2DMultisampleArray,
		SamplerCubeShadow,
		SamplerBuffer,
		Sampler2DRect,
		Sampler2DRectShadow,
		IntSampler1D,
		IntSampler2D,
		IntSampler3D,
		IntSamplerCube,
		IntSampler1DArray,
		IntSampler2DArray,
		IntSampler2DMultisample,
		IntSampler2DMultisampleArray,
		IntSamplerBuffer,
		IntSampler2DRect,
		UnsignedIntSampler1D,
		UnsignedIntSampler2D,
		UnsignedIntSampler3D,
		UnsignedIntSamplerCube,
		UnsignedIntSampler1DArray,
		UnsignedIntSampler2DArray,
		UnsignedIntSampler2DMultisample,
		UnsignedIntSampler2DMultisampleArray,
		UnsignedIntSamplerBuffer,
		UnsignedIntSampler2DRect,
	};

	enum class ImageFormat
	{
		None,
		Depth,
		DepthStencil,
		Stencil,
		Red,
		Green,
		Blue,
		RG,
		RGB,
		RGBA,
		BGR,
		BGRA,
		RedInteger,
		GreenInteger,
		BlueInteger,
		RG_Integer,
		RGB_Integer,
		RGBA_Integer,
		BGR_Integer,
		BGRA_Integer,
	};

	enum class PixelFormat
	{
		None,

		// integer formats
		R8,
		R16,
		RG8,
		RG16,
		R3_G3_B2,
		RGB4,
		RGB5,
		RGB8,
		RGB10,
		RGB12,
		RGB16,
		RGBA2,
		RGBA4,
		RGB5_A1,
		RGBA8,
		RGB10_A2,
		RGBA12,
		RGBA16,
		SRGB8,
		SRGB8_A8,

		// signed normalized integer formats
		Red_SNorm,
		RG_SNorm,
		RGB_SNorm,
		RGBA_SNorm,
		R8_SNorm,
		RG8_SNorm,
		RGB8_SNorm,
		RGBA8_SNorm,
		R16_SNorm,
		RG16_SNorm,
		RGB16_SNorm,
		RGBA16_SNorm,

		// floating point formats
		R16F,
		RG16F,
		RGB16F,
		RGBA16F,
		R32F,
		RG32F,
		RGB32F,
		RGBA32F,
		R11F_G11F_B10F,
		RGB9_E5,

		// pure integer formats
		R8I,
		R8UI,
		R16I,
		R16UI,
		R32I,
		R32UI,
		RG8I,
		RG8UI,
		RG16I,
		RG16UI,
		RG32I,
		RG32UI,
		RGB8I,
		RGB8UI,
		RGB16I,
		RGB16UI,
		RGB32I,
		RGB32UI,
		RGBA8I,
		RGBA8UI,
		RGB10_A2UI,
		RGBA16I,
		RGBA16UI,
		RGBA32I,
		RGBA32UI,

		// depth-stencil formats
		Depth16,
		Depth24,
		Depth32,
		Depth32F,
		Depth24_Stencil8,
		Depth32F_Stencil8,
		Stencil1,
		Stencil4,
		Stencil8,
		Stencil16,

		// compressed formats
		Compressed_R,
		Compressed_RG,
		Compressed_RGB,
		Compressed_RGBA,
		Compressed_SRGB,
		Compressed_SRGB_A,
		Compressed_R_RGTC1,
		Compressed_Signed_R_RGTC1,
		Compressed_RG_RGTC2,
		Compressed_Signed_RG_RGTC2,
		Compressed_RGB_DXT1,
		Compressed_RGBA_DXT1,
		Compressed_RGBA_DXT3,
		Compressed_RGBA_DXT5,
		Compressed_SRGB_DXT1,
		Compressed_SRGB_A_DXT1,
		Compressed_SRGB_A_DXT3,
		Compressed_SRGB_A_DXT5,
		Compressed_RGBA_BPTC_UNorm,
		Compressed_SRGB_Alpha_BPTC_UNorm,
		Compressed_RGB_BPTC_SignedFloat,
		Compressed_RGB_BPTC_UnsignedFloat,
		Compressed_RGB8_ETC2,
		Compressed_SRGB8_ETC2,
		Compressed_RGB8_PunchthroughAlpha1_ETC2,
		Compressed_SRGB8_PunchthroughAlpha1_ETC2,
		Compressed_RGBA8_ETC2_EAC,
		Compressed_SRGB8_ALPHA8_ETC2_EAC,
		Compressed_R11_EAC,
		Compressed_Signed_R11_EAC,
		Compressed_RG11_EAC,
		Compressed_Signed_RG11_EAC,
	};

	enum class CubeFace
	{
		PositiveX,
		NegativeX,
		PositiveY,
		NegativeY,
		PositiveZ,
		NegativeZ,
	};

	// Buffer storage flags
	enum
	{
		BUFFER_MAP_READ_BIT = 0x0001,				// Allows reading from buffer when mapped.
		BUFFER_MAP_WRITE_BIT = 0x0002,				// Allows writing to buffer when mapped.
		BUFFER_MAP_PERSISTENT_BIT = 0x0040,			// Allows buffer to be used while mapped. Requires at least one of BUFFER_MAP_READ_BIT or BUFFER_MAP_WRITE_BIT.
		BUFFER_MAP_COHERENT_BIT = 0x0080,			// Allows reads and writes to be coherent without explicit barier. Requires BUFFER_MAP_PERSISTENT_BIT.
		BUFFER_DYNAMIC_STORAGE_BIT = 0x0100,	// Required for BufferSubData.
		BUFFER_CLIENT_STORAGE_BIT = 0x0200,		// Hint that suggests that storage for the buffer should reside in client memory.
	};

	enum class BufferAccess
	{
		ReadOnly,
        WriteOnly,
        ReadWrite,
	};

	// Buffer map flags
	enum
	{
		MAP_READ_BIT = 0x0001,
		MAP_WRITE_BIT = 0x0002,
		MAP_INVALIDATE_RANGE_BIT = 0x0004,	// cannot be used with MAP_READ_BIT
		MAP_INVALIDATE_BUFFER_BIT = 0x0008,	// cannot be used with MAP_READ_BIT
		MAP_FLUSH_EXPLICIT_BIT = 0x0010,	// MAP_WRITE_BIT must be set
		MAP_UNSYNCHRONIZED_BIT = 0x0020,	// cannot be used with MAP_READ_BIT
	};

	enum class ColorBuffer
	{
		None,

		FrontLeft,
		FrontRight,
		BackLeft,
		BackRight,

		Color0,
		Color1,
		Color2,
		Color3,
		Color4,
		Color5,
		Color6,
		Color7,
		Color8,
		Color9,
		Color10,
		Color11,
		Color12,
		Color13,
		Color14,
		Color15,
	};

	enum class AttachmentBuffer
	{
		Color0,
		Color1,
		Color2,
		Color3,
		Color4,
		Color5,
		Color6,
		Color7,
		Color8,
		Color9,
		Color10,
		Color11,
		Color12,
		Color13,
		Color14,
		Color15,
		Depth,
		Stencil,
		DepthStencil,
	};

	// Renderbuffer type flags, used in BlitFramebuffer
	enum
	{
		COLOR_BUFFER_BIT = 0x0001,
		DEPTH_BUFFER_BIT = 0x0002,
		STENCIL_BUFFER_BIT = 0x0004
	};

	enum class PolygonFace
	{
		Front,
		Back,
		FrontAndBack,
	};

	enum class PrimitiveType
	{
		Points,
		Lines,
		LinesAdjacency,
		LineStrip,
		LineStripAdjacency,
		LineLoop,
		Triangles,
		TrianglesAdjacency,
		TriangleStrip,
		TriangleStripAdjacency,
		TriangleFan,
		Patches,
	};

	enum class TessGenPrimitiveType
	{
		Quads,
		Triangles,
		Isolines,
	};

	enum class TessGenSpacing
	{
		Equal,
		FractionalEven,
		FractionalOdd,
	};

	enum class RasterMode
	{
		Point,
		Line,
		Fill,
	};

	enum class VertexWinding
	{
		Clockwise,
		Counterclockwise,
	};

	enum class CompareFunc
	{
		NeverPass,
		Less,
		Equal,
		LessOrEqual,
		Greater,
		NotEqual,
		GreaterOrEqual,
		AlwaysPass,
	};

	enum class StencilOp
	{
		Keep,
		Zero,
		Replace,
		Increment,
		IncrementWrap,
		Decrement,
		DecrementWrap,
		Invert,
	};

	enum class BlendOp
	{
		Add,
		Subtract,
		ReverseSubtract,
		Min,
		Max,
	};

	enum class BlendFunc
	{
		Zero,
		One,
		SrcColor,
		OneMinusSrcColor,
		DstColor,
		OneMinusDstColor,
		SrcAlpha,
		OneMinusSrcAlpha,
		DstAlpha,
		OneMinusDstAlpha,
		ConstantColor,
		OneMinusConstantColor,
		ConstantAlpha,
		OneMinusConstantAlpha,
		SrcAlphaSaturate,
		Src1Color,
		Src1Alpha,
		OneMinusSrc1Color,
		OneMinusSrc1Alpha,
	};

	enum class ColorReadClamp
	{
		Off,
		On,
		FixedOnly,
	};

	enum class LogicOp
	{
		Clear,
		And,
		AndReverse,
		Copy,
		AndInverted,
		Noop,
		XOr,
		Or,
		NOr,
		Equiv,
		Invert,
		OrReverse,
		CopyInverted,
		OrInverted,
		NAnd,
		Set,
	};

	enum class TexAddressMode
	{
		ClampToBorder,
		ClampToEdge,
		Repeat,
		MirroredRepeat,
		MirrorClampToEdge,
	};

	enum class TexFilter
	{
		Nearest,
		Linear,
		NearestMipmapNearest,
		NearestMipmapLinear,
		LinearMipmapNearest,
		LinearMipmapLinear,
	};

	enum class TexCompareMode
	{
		None,
		RefToTexture,
	};

	enum class TexSwizzleSource
	{
		Red,
		Green,
		Blue,
		Alpha,
		Zero,
		One,
	};

	enum class TexSwizzleDest
	{
		Red,
		Green,
		Blue,
		Alpha,
	};

	enum class FramebufferStatus
	{
		Complete,
		IncompleteAttachment,
		FramebufferIncompleteMissingAttachment,
		IncompleteDrawBuffer,
		IncompleteReadBuffer,
		FramebufferUnsuported,
		FramebufferIncompleteMultisample,
		FramebufferUndefined,
	};

	enum class SwapMethod
	{
		Exchange,
		Copy,
		Undifined,
	};

	enum class ColorBufferType
	{
		RGBA,
		RGBA_Float,
	};

	enum class ProgramParam
	{
		GeometryVertOut,
		GeometryInputType,
		GeometryOutputType,
	};

	enum class ConditionalRenderQueryMode
	{
		Wait,
		NoWait,
		ByRegionWait,
		ByRegionNoWait,
		WaitInverted,
		NoWaitInverted,
		ByRegionWaitInverted,
		ByRegionNoWaitInverted,
	};

	enum class TransformFeedbackBufferMode
	{
		InterleavedAttribs,
		SeparateAttribs,
	};

	enum class VertexConvention
	{
		FirstVertex,
		LastVertex,
	};

	enum class FenceSyncCondition
	{
		GPUCommandsComplete
	};

	enum class SyncWaitStatus
	{
		AlreadySignaled,
		TimeoutExpired,
		ConditionSatisfied,
		WaitFailed,
	};

	// Sync wait flags
	enum
	{
		SYNC_FLUSH_COMMANDS_BIT = 0x0001, // used in ClientWaitSync
	};

	// Memory barrier flags
	enum
	{
		BARRIER_VERTEX_ATTRIB_ARRAY_BIT = 0x0001,
		BARRIER_INDEX_ARRAY_BIT = 0x0002,
		BARRIER_UNIFORM_BIT = 0x0004,
		BARRIER_TEXTURE_FETCH_BIT = 0x0008,
		BARRIER_SHADER_IMAGE_ACCESS_BIT = 0x0010,
		BARRIER_COMMAND_BIT = 0x0020,
		BARRIER_PIXEL_BUFFER_BIT = 0x0040,
		BARRIER_TEXTURE_UPDATE_BIT = 0x0080,
		BARRIER_BUFFER_UPDATE_BIT = 0x0100,
		BARRIER_FRAMEBUFFER_BIT = 0x0200,
		BARRIER_TRANSFORM_FEEDBACK_BIT = 0x0400,
		BARRIER_ATOMIC_COUNTER_BIT = 0x0800,
		BARRIER_SHADER_STORAGE_BIT = 0x1000,
		BARRIER_QUERY_BUFFER_BIT = 0x2000,
		BARRIER_ALL_BITS = 0xFFFFFFFF,
	};

	enum class FeatureSupport
	{
		None,
		Caveat,
		Full,
	};

	enum class TextureComponentType
	{
		None,
		SignedNormalized,
		UnsignedNormalized,
		Float,
		Int,
		UnsignedInt,
	};

	enum class ColorEncoding
	{
		None,
		Linear,
		SRGB,
	};

	enum class ImageClass
	{
		None,
		Class_4_X_32,
		Class_2_X_32,
		Class_1_X_32,
		Class_4_X_16,
		Class_2_X_16,
		Class_1_X_16,
		Class_4_X_8,
		Class_2_X_8,
		Class_1_X_8,
		Class_11_11_10,
		Class_10_10_10_2,
	};

	enum class ImageFormatCompatibilityType
	{
		None,
		BySize,
		ByClass,
	};

	enum class ViewClass
	{
		None,
		Class_128_Bits,
		Class_96_Bits,
		Class_64_Bits,
		Class_48_Bits,
		Class_32_Bits,
		Class_24_Bits,
		Class_16_Bits,
		Class_8_Bits,
		Class_RGTC1_Red,
		Class_RGTC2_RG,
		Class_BPTC_UNorm,
		Class_BPTC_Float,
	};

	enum class DepthStencilTexMode
	{
		DepthComponent,
		StencilIndex,
	};

	enum class ErrorCode
	{
		None,
		InvalidEnum,
		InvalidValue,
		InvalidOperation,
		InvalidFramebufferOperation,
		OutOfMemory,
		StackUnderflow,
		StackOverflow,
	};

	enum class DebugMessageSource
	{
		API,
		WindowSystem,
		ShaderCompiler,
		ThirdParty,
		Application,
		Other,
		All,
	};

	enum class DebugMessageType
	{
		Error,
		DeprecatedBehavior,
		UndefinedBehavior,
		Portability,
		Performance,
		Other,
		Marker,
		All,
	};

	enum class DebugMessageSeverity
	{
		High,
		Medium,
		Low,
		Notification,
		All,
	};

	enum class ClipOrigin
	{
		LowerLeft,
		UpperLeft,
	};

	enum class ClipDepth
	{
		NegativeOneToOne,
		ZeroToOne,
	};


	typedef void* SyncObject;

	typedef unsigned long ulong;
	typedef unsigned int uint;
	typedef unsigned short ushort;
	typedef unsigned char ubyte;
	typedef uint64_t uint64;
	typedef int64_t int64;
	typedef ptrdiff_t intptr;
	typedef ptrdiff_t sizeiptr;
	typedef int sizei;
	
	struct PixelStore
	{
		bool swapBytes = false;
		bool lsbFirst = false;
		int rowLength = 0;	// Number of pixels in a row.
		int skipRows = 0;
		int skipPixels = 0;
		int alignment = 4;
		int imageHeight = 0;
		int skipImages = 0;
		int compressedBlockWidth = 0;
		int compressedBlockHeight = 0;
		int compressedBlockDepth = 0;
		int compressedBlockSize = 0;
	};

	struct FramebufferFormat
	{
		ColorBufferType colorBufferType;
		bool sRGB;
		bool doubleBuffer;
		int colorBits;
		int depthBits;
		int stencilBits;
		int multisampleSamples;
		SwapMethod swapMethod;
	};

	class IDebugLogger
	{
	public:
		virtual ~IDebugLogger() = default;
		virtual void DebugMessage(DebugMessageSource source, DebugMessageType type, uint id, DebugMessageSeverity severity, const char* message) = 0;
	};

}



#endif // _GLSLAYER_COMMON_H_
