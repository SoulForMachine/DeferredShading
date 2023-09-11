#include "DeferredRenderer.h"
#include <cassert>
#include <string>
#include <algorithm>
#include <numeric>
#include <random>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <locale>
#include <filesystem>

#include <GLSlayer/RenderContextInit.h>
#include "Utils.h"


#pragma pack(push, 1)
struct SSRectVertex
{
	math3d::vec2f position;
	math3d::vec2f uv;
};

struct UniformGbufferTexViewData
{
	math3d::vec4f rangeMin;
	math3d::vec4f rangeMax;
	int texComponents;
};

struct UniformSceneXformData
{
	math3d::mat4f viewProjMatrix;
	math3d::vec4f viewport;
};

struct UniformLightData
{
	math3d::vec4f positionRadius;
	math3d::vec4f colorFalloffExp;
};

#pragma pack(pop)


DeferredRenderer::~DeferredRenderer()
{
	Deinit();
}

bool DeferredRenderer::Init(gls::CreateContextInfo& info)
{
	// Create rendering context and setup some states.

#ifdef _DEBUG
	info.debugContext = true;
#else
	info.debugContext = false;
#endif
	info.logger = &_console;

	_renderContext = gls::CreateRenderContext(info);

	if (!_renderContext)
		return false;

	_renderContext->SetCurrentContext();
	_renderContext->CullFace(gls::PolygonFace::Back);
	_renderContext->FrontFace(gls::VertexWinding::Counterclockwise);
	_renderContext->EnableFaceCulling(true);
	_renderContext->SwapInterval(_curSwapInterval);
#ifdef _DEBUG
	_renderContext->EnableDebugOutput(true, true);
	_renderContext->EnableDebugMessages(gls::DebugMessageSource::All, gls::DebugMessageType::All, gls::DebugMessageSeverity::All, true);
	_renderContext->EnableDebugMessages(gls::DebugMessageSource::All, gls::DebugMessageType::All, gls::DebugMessageSeverity::Low, false);
#endif

	// Load shaders.

	_fragShaderGeometryPass = LoadFragmentShader("GeometryPass.frag");
	if (_fragShaderGeometryPass == nullptr)
	{
		Deinit();
		return false;
	}

	_vertShaderScreenSpace = LoadVertexShader("ScreenSpace.vert");
	if (_vertShaderScreenSpace == nullptr)
	{
		Deinit();
		return false;
	}

	_fragShaderVisGBuffer = LoadFragmentShader("VisGBuffer.frag");
	if (_fragShaderVisGBuffer == nullptr)
	{
		Deinit();
		return false;
	}

	_vertShaderLightingPass = LoadVertexShader("LightingPass.vert");
	if (_vertShaderLightingPass == nullptr)
	{
		Deinit();
		return false;
	}

	_fragShaderLightingPass = LoadFragmentShader("LightingPass.frag");
	if (_fragShaderLightingPass == nullptr)
	{
		Deinit();
		return false;
	}

	_vertShaderLightSource = LoadVertexShader("LightSource.vert");
	if (_vertShaderLightSource == nullptr)
	{
		Deinit();
		return false;
	}

	_fragShaderLightSource = LoadFragmentShader("LightSource.frag");
	if (_fragShaderLightSource == nullptr)
	{
		Deinit();
		return true;
	}

	_vertShaderImGui = LoadVertexShader("ImGui.vert");
	if (_vertShaderImGui == nullptr)
	{
		Deinit();
		return false;
	}

	_fragShaderImGui = LoadFragmentShader("ImGui.frag");
	if (_fragShaderImGui == nullptr)
	{
		Deinit();
		return false;
	}

	_vertShaderForward = LoadVertexShader("Forward.vert");
	if (_vertShaderForward == nullptr)
	{
		Deinit();
		return false;
	}

	_fragShaderForward = LoadFragmentShader("Forward.frag");
	if (_fragShaderForward == nullptr)
	{
		Deinit();
		return false;
	}

	_fragShaderForwardSP = LoadFragmentShader("ForwardSinglePass.frag");
	if (_fragShaderForwardSP == nullptr)
	{
		Deinit();
		return false;
	}

	_fragShaderForwardTransp = LoadFragmentShader("ForwardTransp.frag");
	if (_fragShaderForwardTransp == nullptr)
	{
		Deinit();
		return false;
	}

	_fragShaderForwardTranspSP = LoadFragmentShader("ForwardTranspSinglePass.frag");
	if (_fragShaderForwardTranspSP == nullptr)
	{
		Deinit();
		return false;
	}

	_vertShaderDepthOnly = LoadVertexShader("DepthOnlyPass.vert");
	if (_vertShaderDepthOnly == nullptr)
	{
		Deinit();
		return false;
	}

	// Load the main scene.

	if (!_sponzaScene.Load(_renderContext, "Sponza/sponza.obj"))
	{
		Deinit();
		_console.PrintLn("Error: failed to load the main scene.");
		return false;
	}

	_sponzaScene.GetBounds(_sceneBoundsMin, _sceneBoundsMax);
	math3d::vec3f bounds = _sceneBoundsMax - _sceneBoundsMin;
	_cameraPosition = _sceneBoundsMin + bounds / 2.0f;
	_cameraForwardVector = { 0.0f, 0.0f, -1.0f };
	_cameraRightVector = { 1.0f, 0.0f, 0.0f };
	_rotX = 0.0f;
	_rotY = 0.0f;

	// Lights

	_lightBoundsMin = _sceneBoundsMin;
	_lightBoundsMin.x += bounds.x / 7.0f;
	_lightBoundsMin.y += bounds.y / 10.0f;
	_lightBoundsMin.z += bounds.z / 4.0f;
	_lightBoundsMax = _sceneBoundsMax;
	_lightBoundsMax.x -= bounds.x / 7.0f;
	_lightBoundsMax.y -= bounds.y / 10.0f;
	_lightBoundsMax.z -= bounds.z / 4.0f;

	_newLightMinRandomRadius = 200.0f;
	_newLightMaxRandomRadius = 600.0f;
	_newLightRadius = 400.0f;

	CreateRandomLights(_randomLightsToCreate, _lightBoundsMin, _lightBoundsMax);

	_lightInfoBuf = _renderContext->CreateBuffer(MaxLights * sizeof(UniformLightData), nullptr, gls::BUFFER_DYNAMIC_STORAGE_BIT);
	_lightInfoTex = _renderContext->CreateTextureBuffer();
	_lightInfoTex->TexBuffer(gls::PixelFormat::RGBA32F, _lightInfoBuf);

	_lightIndexBuf = _renderContext->CreateBuffer(MaxLights * sizeof(int32_t), nullptr, gls::BUFFER_DYNAMIC_STORAGE_BIT);
	_lightIndexTex = _renderContext->CreateTextureBuffer();
	_lightIndexTex->TexBuffer(gls::PixelFormat::R32I, _lightIndexBuf);

	CreateSphere(1.0f, 16, 16);

	// vertex formats

	gls::VertexAttribDesc vertDesc[] =
	{
		{ 0, 0, 3, gls::DataType::Float, false, false, offsetof(ObjScene::Vertex, position) },
		{ 0, 1, 3, gls::DataType::Float, false, false, offsetof(ObjScene::Vertex, normal) },
		{ 0, 2, 3, gls::DataType::Float, false, false, offsetof(ObjScene::Vertex, tangent) },
		{ 0, 3, 2, gls::DataType::Float, false, false, offsetof(ObjScene::Vertex, texcoords) },
	};
	_vertexFormat = _renderContext->CreateVertexFormat(vertDesc, CountOf(vertDesc));

	gls::VertexAttribDesc rectVertDesc[] =
	{
		{ 0, 0, 2, gls::DataType::Float, false, false, 0 },
		{ 0, 1, 2, gls::DataType::Float, false, false, 8 },
	};
	_vertFmtScreenRect = _renderContext->CreateVertexFormat(rectVertDesc, CountOf(rectVertDesc));

	gls::VertexAttribDesc imGuiVertDesc[] =
	{
		{ 0, 0, 2, gls::DataType::Float, false, false, offsetof(ImDrawVert, pos) },
		{ 0, 1, 2, gls::DataType::Float, false, false, offsetof(ImDrawVert, uv) },
		{ 0, 2, 4, gls::DataType::UnsignedByte, false, true, offsetof(ImDrawVert, col) },
	};
	_vertFmtImGui = _renderContext->CreateVertexFormat(imGuiVertDesc, CountOf(imGuiVertDesc));

	// uniform buffers

	_ubufSceneXformData = _renderContext->CreateBuffer(sizeof(UniformSceneXformData), nullptr, gls::BUFFER_DYNAMIC_STORAGE_BIT);
	_ubufLightData = _renderContext->CreateBuffer(sizeof(UniformLightData), nullptr, gls::BUFFER_DYNAMIC_STORAGE_BIT);
	_ubufGbufferTexViewData = _renderContext->CreateBuffer(sizeof(UniformGbufferTexViewData), nullptr, gls::BUFFER_DYNAMIC_STORAGE_BIT);
	_ubufImGui = _renderContext->CreateBuffer(sizeof(math3d::mat4f), nullptr, gls::BUFFER_DYNAMIC_STORAGE_BIT);

	// Screen-space rectangles buffer.

	_rectVertBuf = _renderContext->CreateBuffer(2 * 2 * 6 * sizeof(SSRectVertex), nullptr, gls::BUFFER_DYNAMIC_STORAGE_BIT);

	// samplers

	gls::SamplerStateDesc samplerSurfaceDesc;
	samplerSurfaceDesc.addressU = gls::TexAddressMode::Repeat;
	samplerSurfaceDesc.addressV = gls::TexAddressMode::Repeat;
	samplerSurfaceDesc.minFilter = gls::TexFilter::LinearMipmapLinear;
	samplerSurfaceDesc.magFilter = gls::TexFilter::Linear;
	_samplerSurfaceTex = _renderContext->CreateSamplerState(samplerSurfaceDesc);

	gls::SamplerStateDesc samplerLinearClampDesc;
	samplerLinearClampDesc.addressU = gls::TexAddressMode::ClampToEdge;
	samplerLinearClampDesc.addressV = gls::TexAddressMode::ClampToEdge;
	samplerLinearClampDesc.minFilter = gls::TexFilter::Linear;
	samplerLinearClampDesc.magFilter = gls::TexFilter::Linear;

	_samplerLinearClamp = _renderContext->CreateSamplerState(samplerLinearClampDesc);

	gls::SamplerStateDesc samplerGBufferDesc;
	samplerGBufferDesc.addressU = gls::TexAddressMode::ClampToEdge;
	samplerGBufferDesc.addressV = gls::TexAddressMode::ClampToEdge;
	samplerGBufferDesc.minFilter = gls::TexFilter::Nearest;
	samplerGBufferDesc.magFilter = gls::TexFilter::Nearest;

	_samplerGBuffer = _renderContext->CreateSamplerState(samplerGBufferDesc);

	// Initialize ImGui.

	ImGui::CreateContext();
	_imGuiIO = &ImGui::GetIO();
	int width, height;
	unsigned char* pixels = nullptr;
	_imGuiIO->Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
	gls::ITexture2D* guiTexture = _renderContext->CreateTexture2D(1, gls::PixelFormat::RGBA8, width, height);
	guiTexture->TexSubImage(0, 0, 0, width, height, gls::ImageFormat::RGBA, gls::DataType::UnsignedByte, nullptr, pixels);
	_imGuiIO->Fonts->TexID = guiTexture;
	_imGuiIO->IniFilename = nullptr;

	_imGuiIO->KeyMap[ImGuiKey_Tab] = static_cast<int>(Key::Tab);
	_imGuiIO->KeyMap[ImGuiKey_LeftArrow] = static_cast<int>(Key::LeftArrow);
	_imGuiIO->KeyMap[ImGuiKey_RightArrow] = static_cast<int>(Key::RightArrow);
	_imGuiIO->KeyMap[ImGuiKey_UpArrow] = static_cast<int>(Key::UpArrow);
	_imGuiIO->KeyMap[ImGuiKey_DownArrow] = static_cast<int>(Key::DownArrow);
	_imGuiIO->KeyMap[ImGuiKey_PageUp] = static_cast<int>(Key::PageUp);
	_imGuiIO->KeyMap[ImGuiKey_PageDown] = static_cast<int>(Key::PageDown);
	_imGuiIO->KeyMap[ImGuiKey_Home] = static_cast<int>(Key::Home);
	_imGuiIO->KeyMap[ImGuiKey_End] = static_cast<int>(Key::End);
	_imGuiIO->KeyMap[ImGuiKey_Insert] = static_cast<int>(Key::Insert);
	_imGuiIO->KeyMap[ImGuiKey_Delete] = static_cast<int>(Key::Delete);
	_imGuiIO->KeyMap[ImGuiKey_Backspace] = static_cast<int>(Key::Backspace);
	_imGuiIO->KeyMap[ImGuiKey_Space] = static_cast<int>(Key::Space);
	_imGuiIO->KeyMap[ImGuiKey_Enter] = static_cast<int>(Key::Enter);
	_imGuiIO->KeyMap[ImGuiKey_Escape] = static_cast<int>(Key::Escape);
	_imGuiIO->KeyMap[ImGuiKey_KeyPadEnter] = static_cast<int>(Key::Enter);
	_imGuiIO->KeyMap[ImGuiKey_A] = static_cast<int>(Key::A);
	_imGuiIO->KeyMap[ImGuiKey_C] = static_cast<int>(Key::C);
	_imGuiIO->KeyMap[ImGuiKey_V] = static_cast<int>(Key::V);
	_imGuiIO->KeyMap[ImGuiKey_X] = static_cast<int>(Key::X);
	_imGuiIO->KeyMap[ImGuiKey_Y] = static_cast<int>(Key::Y);
	_imGuiIO->KeyMap[ImGuiKey_Z] = static_cast<int>(Key::Z);

	_imGuiVertBuffer = _renderContext->CreateBuffer((1 << 16) * sizeof(ImDrawVert), nullptr, gls::BUFFER_DYNAMIC_STORAGE_BIT);
	_imGuiIndexBuffer = _renderContext->CreateBuffer((1 << 20)* sizeof(ImDrawIdx), nullptr, gls::BUFFER_DYNAMIC_STORAGE_BIT);

	_framerateValues.resize(60, 0.0f);
	_framerateValueAddTime = 0.0f;

	_demoPlayer.SetCallbacks(
		[this]() { RecorderSamplingFunc(); },
		[this](int32_t id, size_t size, const void* data) { PlayerSampleUpdate(id, size, data); });

	return true;
}

void DeferredRenderer::Deinit()
{
	if (_renderContext)
	{
		DestroySphere();
		DestroyFramebuffers();
		_sponzaScene.Unload();

		_renderContext->DestroyBuffer(_imGuiVertBuffer);
		_renderContext->DestroyBuffer(_imGuiIndexBuffer);
		if (_imGuiIO != nullptr)
			_renderContext->DestroyTexture(static_cast<gls::ITexture2D*>(_imGuiIO->Fonts->TexID));
		if (ImGui::GetCurrentContext() != nullptr)
			ImGui::DestroyContext();

		_renderContext->DestroyTexture(_lightInfoTex);
		_renderContext->DestroyBuffer(_lightInfoBuf);
		_renderContext->DestroyTexture(_lightIndexTex);
		_renderContext->DestroyBuffer(_lightIndexBuf);
		_renderContext->DestroyShader(_fragShaderGeometryPass);
		_renderContext->DestroyShader(_vertShaderScreenSpace);
		_renderContext->DestroyShader(_fragShaderVisGBuffer);
		_renderContext->DestroyShader(_vertShaderLightingPass);
		_renderContext->DestroyShader(_fragShaderLightingPass);
		_renderContext->DestroyShader(_vertShaderLightSource);
		_renderContext->DestroyShader(_fragShaderLightSource);
		_renderContext->DestroyShader(_vertShaderImGui);
		_renderContext->DestroyShader(_fragShaderImGui);
		_renderContext->DestroyShader(_vertShaderForward);
		_renderContext->DestroyShader(_fragShaderForward);
		_renderContext->DestroyShader(_fragShaderForwardSP);
		_renderContext->DestroyShader(_fragShaderForwardTransp);
		_renderContext->DestroyShader(_fragShaderForwardTranspSP);
		_renderContext->DestroyShader(_vertShaderDepthOnly);
		_renderContext->DestroyVertexFormat(_vertexFormat);
		_renderContext->DestroyVertexFormat(_vertFmtScreenRect);
		_renderContext->DestroyVertexFormat(_vertFmtImGui);
		_renderContext->DestroyBuffer(_rectVertBuf);
		_renderContext->DestroyBuffer(_ubufSceneXformData);
		_renderContext->DestroyBuffer(_ubufLightData);
		_renderContext->DestroyBuffer(_ubufGbufferTexViewData);
		_renderContext->DestroyBuffer(_ubufImGui);
		_renderContext->DestroySamplerState(_samplerSurfaceTex);
		_renderContext->DestroySamplerState(_samplerLinearClamp);
		_renderContext->DestroySamplerState(_samplerGBuffer);

		gls::DestroyRenderContext(_renderContext);

		*this = {};		// Reset all member variables to default values.
	}
}

gls::FramebufferFormat DeferredRenderer::GetFramebufferFormat()
{
	gls::FramebufferFormat fbufFormat;
	fbufFormat.colorBits = 32;
	fbufFormat.colorBufferType = gls::ColorBufferType::RGBA;
	fbufFormat.depthBits = 0;
	fbufFormat.stencilBits = 0;
	fbufFormat.doubleBuffer = true;
	fbufFormat.multisampleSamples = 0;
	fbufFormat.sRGB = false;
	fbufFormat.swapMethod = gls::SwapMethod::Exchange;
	return fbufFormat;
}

gls::IVertexShader* DeferredRenderer::LoadVertexShader(const char* fileName)
{
	std::string source = LoadShaderSource(fileName);
	if (source.empty())
	{
		_console.PrintLn("Failed to load vertex shader from file: %s", fileName);
		return nullptr;
	}

	const char* sources[] = { source.c_str() };
	bool success;
	gls::IVertexShader* vertShader = _renderContext->CreateVertexShader(1, sources, success);
	if (vertShader->GetInfoLogLength() > 1)
	{
		_console.PrintLn("Compiling vertex shader: %s\n%s", fileName, vertShader->GetInfoLog());
	}

	if (!success)
	{
		_renderContext->DestroyShader(vertShader);
		return nullptr;
	}

	return vertShader;
}

gls::IFragmentShader* DeferredRenderer::LoadFragmentShader(const char* fileName)
{
	std::string source = LoadShaderSource(fileName);
	if (source.empty())
	{
		_console.PrintLn("Failed to load fragment shader from file: %s", fileName);
		return nullptr;
	}

	const char* sources[] = { source.c_str() };
	bool success;
	gls::IFragmentShader* fragShader = _renderContext->CreateFragmentShader(1, sources, success);
	if (fragShader->GetInfoLogLength() > 1)
	{
		_console.PrintLn("Compiling fragment shader: %s\n%s", fileName, fragShader->GetInfoLog());
	}

	if (!success)
	{
		_renderContext->DestroyShader(fragShader);
		return nullptr;
	}

	return fragShader;
}

void DeferredRenderer::CreateFramebuffers(int width, int height)
{
	DestroyFramebuffers();

	_texPosition = _renderContext->CreateTexture2D(1, gls::PixelFormat::RGB16F, width, height);
	_texNormal = _renderContext->CreateTexture2D(1, gls::PixelFormat::RGB16F, width, height);
	_texDiffuse = _renderContext->CreateTexture2D(1, gls::PixelFormat::RGB8, width, height);
	_depthBuffer = _renderContext->CreateTexture2D(1, gls::PixelFormat::Depth24_Stencil8, width, height);

	_gbuffer = _renderContext->CreateFramebuffer();
	_gbuffer->AttachTexture(gls::AttachmentBuffer::Color0, _texPosition, 0);
	_gbuffer->AttachTexture(gls::AttachmentBuffer::Color1, _texNormal, 0);
	_gbuffer->AttachTexture(gls::AttachmentBuffer::Color2, _texDiffuse, 0);
	_gbuffer->AttachTexture(gls::AttachmentBuffer::DepthStencil, _depthBuffer, 0);
	gls::FramebufferStatus status = _gbuffer->CheckStatus();
	assert(status == gls::FramebufferStatus::Complete);

	gls::ColorBuffer buffers[] = { gls::ColorBuffer::Color0, gls::ColorBuffer::Color1, gls::ColorBuffer::Color2 };
	_renderContext->ActiveColorBuffers(_gbuffer, buffers, CountOf(buffers));

	_sceneBuffer = _renderContext->CreateFramebuffer();
	_texSceneColor = _renderContext->CreateTexture2D(1, gls::PixelFormat::RGBA8, width, height);
	_sceneBuffer->AttachTexture(gls::AttachmentBuffer::Color0, _texSceneColor, 0);
	_sceneBuffer->AttachTexture(gls::AttachmentBuffer::DepthStencil, _depthBuffer, 0);
	status = _sceneBuffer->CheckStatus();
	assert(status == gls::FramebufferStatus::Complete);

	buffers[0] = gls::ColorBuffer::Color0;
	_renderContext->ActiveColorBuffers(_sceneBuffer, buffers, 1);

	buffers[0] = gls::ColorBuffer::BackLeft;
	_renderContext->ActiveColorBuffers(nullptr, buffers, 1);
}

void DeferredRenderer::DestroyFramebuffers()
{
	if (_gbuffer)
	{
		_renderContext->DestroyFramebuffer(_gbuffer);
		_gbuffer = nullptr;
	}

	if (_depthBuffer)
	{
		_renderContext->DestroyTexture(_depthBuffer);
		_depthBuffer = nullptr;
	}

	if (_texPosition)
	{
		_renderContext->DestroyTexture(_texPosition);
		_texPosition = nullptr;
	}

	if (_texNormal)
	{
		_renderContext->DestroyTexture(_texNormal);
		_texNormal = nullptr;
	}

	if (_texDiffuse)
	{
		_renderContext->DestroyTexture(_texDiffuse);
		_texDiffuse = nullptr;
	}

	if (_sceneBuffer)
	{
		_renderContext->DestroyFramebuffer(_sceneBuffer);
		_sceneBuffer = nullptr;
	}

	if (_texSceneColor)
	{
		_renderContext->DestroyTexture(_texSceneColor);
		_texSceneColor = nullptr;
	}
}

void DeferredRenderer::RenderGeometryPass()
{
	_renderContext->SetFramebuffer(_gbuffer);
	_renderContext->ClearColorBuffer(_gbuffer, 0, math3d::vec4f(0.0f, 0.0f, 0.0f, 0.0f));
	_renderContext->ClearColorBuffer(_gbuffer, 1, math3d::vec4f(0.0f, 0.0f, 0.0f, 0.0f));
	_renderContext->ClearColorBuffer(_gbuffer, 2, math3d::vec4f(0.0f, 0.0f, 0.0f, 0.0f));
	_renderContext->ClearDepthStencilBuffer(_gbuffer, 1.0f, 0);

	_renderContext->EnableDepthTest(true);

	_renderContext->SetUniformBuffer(0, _ubufSceneXformData);

	_renderContext->ActiveVertexFormat(_vertexFormat);
	_renderContext->VertexSource(0, _sponzaScene.GetVertexBuffer(), sizeof(ObjScene::Vertex), 0, 0);
	_renderContext->IndexSource(_sponzaScene.GetIndexBuffer(), gls::DataType::UnsignedInt);

	// First fill only the depth buffer.
	_renderContext->SetVertexShader(_vertShaderDepthOnly);
	_renderContext->SetFragmentShader(nullptr);

	_renderContext->EnableColorWrite(false, false, false, false);

	for (const ObjScene::Mesh* mesh : _visibleObjects)
	{
		_renderContext->DrawIndexed(gls::PrimitiveType::Triangles, static_cast<gls::intptr>(mesh->indexOffset) * 4, 0, mesh->numIndices);
	}

	_renderContext->EnableColorWrite(true, true, true, true);
	_renderContext->EnableDepthWrite(false);
	_renderContext->DepthTestFunc(gls::CompareFunc::Equal);

	// Render to G-buffer.
	_renderContext->SetVertexShader(_vertShaderForward);
	_renderContext->SetFragmentShader(_fragShaderGeometryPass);

	_renderContext->SetSamplerState(0, _samplerSurfaceTex);
	_renderContext->SetSamplerState(1, _samplerSurfaceTex);

	int prevMatInd = -1;

	for (const ObjScene::Mesh* mesh : _visibleObjects)
	{
		if (mesh->materialIndex != prevMatInd)
		{
			const ObjScene::Material& material = _sponzaScene.GetMaterial(mesh->materialIndex);
			_renderContext->SetSamplerTexture(0, material.diffuseTexture);
			_renderContext->SetSamplerTexture(1, material.normalTexture);
			prevMatInd = mesh->materialIndex;
		}

		_renderContext->DrawIndexed(gls::PrimitiveType::Triangles, static_cast<gls::intptr>(mesh->indexOffset) * 4, 0, mesh->numIndices);
	}

	_renderContext->EnableDepthTest(false);
	_renderContext->EnableDepthWrite(true);
	_renderContext->DepthTestFunc(gls::CompareFunc::Less);
}

void DeferredRenderer::RenderLightingPass()
{
	_renderContext->SetFramebuffer(_sceneBuffer);
	_renderContext->ClearColorBuffer(_sceneBuffer, 0, math3d::vec4f(0.0f, 0.0f, 0.0f, 0.0f));
	// Stencil buffer was cleared in the previous pass together with depth buffer, while it was attached to the G-buffer.
	// We use the content of the depth buffer from the previous pass.

	_renderContext->SetUniformBuffer(0, _ubufSceneXformData);
	_renderContext->SetUniformBuffer(1, _ubufLightData);

	_renderContext->ActiveVertexFormat(_vertFmtSphere);
	_renderContext->VertexSource(0, _sphereVertBuf, sizeof(math3d::vec3f), 0, 0);
	_renderContext->IndexSource(_sphereIndexBuf, gls::DataType::UnsignedShort);
	_renderContext->SetVertexShader(_vertShaderLightingPass);

	_renderContext->SetSamplerState(0, _samplerGBuffer);
	_renderContext->SetSamplerState(1, _samplerGBuffer);
	_renderContext->SetSamplerState(2, _samplerGBuffer);

	_renderContext->SetSamplerTexture(0, _texDiffuse);
	_renderContext->SetSamplerTexture(1, _texPosition);
	_renderContext->SetSamplerTexture(2, _texNormal);

	_renderContext->BlendingFunc(gls::BlendFunc::One, gls::BlendFunc::One);
	_renderContext->EnableStencilTest(true);
	_renderContext->EnableDepthTest(true);
	_renderContext->EnableDepthWrite(false);
	_renderContext->StencilOperation(gls::PolygonFace::Front, gls::StencilOp::Keep, gls::StencilOp::Replace, gls::StencilOp::Keep);
	_renderContext->StencilOperation(gls::PolygonFace::Back, gls::StencilOp::Keep, gls::StencilOp::Keep, gls::StencilOp::Keep);

	gls::uint stencilRefVal = 0;

	for (auto light : _visibleLights)
	{
		// Clear the stencil buffer each time we wrap the stencil reference value.
		stencilRefVal = (stencilRefVal + 1) % 256;
		if (stencilRefVal == 0)
		{
			_renderContext->ClearStencilBuffer(_sceneBuffer, 0);
			stencilRefVal = 1;
		}

		// Draw front faces of the light sphere, marking pixels in the stencil buffer when the depth test fails.

		UniformLightData lightData = {
			math3d::vec4f(light->position, light->radius * _lightRadiusScale),
			math3d::vec4f(light->color, light->falloffExponent)
		};
		_ubufLightData->BufferSubData(0, sizeof(lightData), &lightData);

		_renderContext->SetFragmentShader(nullptr);

		_renderContext->EnableColorWrite(false, false, false, false);

		_renderContext->StencilTestFunc(gls::PolygonFace::Front, gls::CompareFunc::AlwaysPass, stencilRefVal, 0);

		_renderContext->DrawIndexed(gls::PrimitiveType::Triangles, 0, 0, _sphereIndexCount);

		_renderContext->EnableColorWrite(true, true, true, true);

		// Draw back faces of the light sphere using the GBuffer and set depth function to greater, affecting only pixels where stencil
		// value is not marked by previous draw and the back of the sphere is behind visible surfaces. Add light contribution by blending.

		_renderContext->SetFragmentShader(_fragShaderLightingPass);

		_renderContext->EnableDepthClamp(true);
		_renderContext->EnableBlending(true);
		_renderContext->CullFace(gls::PolygonFace::Front);

		_renderContext->StencilTestFunc(gls::PolygonFace::Back, gls::CompareFunc::NotEqual, stencilRefVal, static_cast<gls::uint>(-1));
		_renderContext->DepthTestFunc(gls::CompareFunc::Greater);

		_renderContext->DrawIndexed(gls::PrimitiveType::Triangles, 0, 0, _sphereIndexCount);

		_renderContext->EnableDepthClamp(false);
		_renderContext->EnableBlending(false);
		_renderContext->CullFace(gls::PolygonFace::Back);
		_renderContext->DepthTestFunc(gls::CompareFunc::Less);
	}

	_renderContext->EnableStencilTest(false);
	_renderContext->EnableDepthTest(false);
	_renderContext->EnableDepthWrite(true);
}

void DeferredRenderer::RenderGBufferPreview()
{
	_renderContext->SetFramebuffer(nullptr);
	_renderContext->ClearColorBuffer(nullptr, 0, math3d::vec4f(0.1f, 0.3f, 0.3f, 1.0f));

	_renderContext->SetUniformBuffer(0, _ubufSceneXformData);
	_renderContext->SetUniformBuffer(1, _ubufGbufferTexViewData);
	_renderContext->SetVertexShader(_vertShaderScreenSpace);
	_renderContext->SetFragmentShader(_fragShaderVisGBuffer);
	_renderContext->ActiveVertexFormat(_vertFmtScreenRect);
	_renderContext->VertexSource(0, _rectVertBuf, sizeof(SSRectVertex), 0, 0);
	_renderContext->IndexSource(nullptr, gls::DataType::None);

	float x1 = 0.0f;
	float y1 = 0.0f;
	const int rows = 2, cols = 2;
	float width = static_cast<float>(_viewportWidth) / cols;
	float height = static_cast<float>(_viewportHeight)/ rows;
	SSRectVertex vertices[rows * cols * 6];
	for (int r = 0; r < rows; ++r)
	{
		for (int c = 0; c < cols; ++c)
		{
			int index = (r * cols + c) * 6;
			vertices[index + 0].position.set(x1 + c * width, y1 + r * height);
			vertices[index + 0].uv.set(0.0f, 0.0f);
			vertices[index + 1].position.set(x1 + c * width + width, y1 + r * height);
			vertices[index + 1].uv.set(1.0f, 0.0f);
			vertices[index + 2].position.set(x1 + c * width + width, y1 + r * height + height);
			vertices[index + 2].uv.set(1.0f, 1.0f);
			vertices[index + 3].position.set(x1 + c * width, y1 + r * height);
			vertices[index + 3].uv.set(0.0f, 0.0f);
			vertices[index + 4].position.set(x1 + c * width + width, y1 + r * height + height);
			vertices[index + 4].uv.set(1.0f, 1.0f);
			vertices[index + 5].position.set(x1 + c * width, y1 + r * height + height);
			vertices[index + 5].uv.set(0.0f, 1.0f);
		}
	}
	_rectVertBuf->BufferSubData(0, sizeof(SSRectVertex) * rows * cols * 6, vertices);

	_renderContext->SetSamplerState(0, _samplerLinearClamp);

	UniformGbufferTexViewData texViewData;

	texViewData.texComponents = 3;
	texViewData.rangeMin.set(0.0f, 0.0f, 0.0f, 0.0f);
	texViewData.rangeMax.set(1.0f, 1.0f, 1.0f, 0.0f);
	_ubufGbufferTexViewData->BufferSubData(0, sizeof(texViewData), &texViewData);

	_renderContext->SetSamplerTexture(0, _texDiffuse);
	_renderContext->Draw(gls::PrimitiveType::Triangles, 0, 6);

	texViewData.texComponents = 3;
	texViewData.rangeMin.set(_sceneBoundsMin, 0.0f);
	texViewData.rangeMax.set(_sceneBoundsMax, 0.0f);
	_ubufGbufferTexViewData->BufferSubData(0, sizeof(texViewData), &texViewData);

	_renderContext->SetSamplerTexture(0, _texPosition);
	_renderContext->Draw(gls::PrimitiveType::Triangles, 6, 6);

	texViewData.texComponents = 3;
	texViewData.rangeMin.set(-1.0f, -1.0f, -1.0f);
	texViewData.rangeMax.set(1.0f, 1.0f, 1.0f);
	_ubufGbufferTexViewData->BufferSubData(0, sizeof(texViewData), &texViewData);

	_renderContext->SetSamplerTexture(0, _texNormal);
	_renderContext->Draw(gls::PrimitiveType::Triangles, 12, 6);

	texViewData.texComponents = 1;
	texViewData.rangeMin.set(0.9f, 0.9f, 0.9f);
	texViewData.rangeMax.set(1.0f, 1.0f, 1.0f);
	_ubufGbufferTexViewData->BufferSubData(0, sizeof(texViewData), &texViewData);

	_renderContext->SetSamplerTexture(0, _depthBuffer);
	_renderContext->Draw(gls::PrimitiveType::Triangles, 18, 6);
}

void DeferredRenderer::RenderForward()
{
	_renderContext->SetFramebuffer(_sceneBuffer);
	_renderContext->ClearColorBuffer(_sceneBuffer, 0, math3d::vec4f(0.0f, 0.0f, 0.0f, 0.0f));
	_renderContext->ClearDepthStencilBuffer(_sceneBuffer, 1.0f, 0);

	// First fill only the depth buffer.

	_renderContext->EnableDepthTest(true);
	_renderContext->EnableColorWrite(false, false, false, false);

	_renderContext->SetUniformBuffer(0, _ubufSceneXformData);
	_renderContext->SetVertexShader(_vertShaderDepthOnly);
	_renderContext->SetFragmentShader(nullptr);

	_renderContext->ActiveVertexFormat(_vertexFormat);
	_renderContext->VertexSource(0, _sponzaScene.GetVertexBuffer(), sizeof(ObjScene::Vertex), 0, 0);
	_renderContext->IndexSource(_sponzaScene.GetIndexBuffer(), gls::DataType::UnsignedInt);

	for (const ObjScene::Mesh* mesh : _visibleObjects)
	{
		_renderContext->DrawIndexed(gls::PrimitiveType::Triangles, static_cast<gls::intptr>(mesh->indexOffset) * 4, 0, mesh->numIndices);
	}

	// Draw lit objects.
	
	_renderContext->EnableColorWrite(true, true, true, true);
	_renderContext->DepthTestFunc(gls::CompareFunc::Equal);
	_renderContext->EnableDepthWrite(false);

	_renderContext->SetUniformBuffer(1, _ubufLightData);
	_renderContext->SetVertexShader(_vertShaderForward);
	_renderContext->SetFragmentShader(_fragShaderForward);

	_renderContext->EnableBlending(true);
	_renderContext->BlendingFunc(gls::BlendFunc::One, gls::BlendFunc::One);

	_renderContext->SetSamplerState(0, _samplerSurfaceTex);
	_renderContext->SetSamplerState(1, _samplerSurfaceTex);

	int prevMatInd = -1;

	for (size_t objInd = 0; objInd < _visibleObjects.size(); ++objInd)
	{
		const std::vector<int32_t>& lightIndices = _interactions[objInd];

		if (lightIndices.size() > 0)
		{
			const ObjScene::Mesh* mesh = _visibleObjects[objInd];

			if (mesh->materialIndex != prevMatInd)
			{
				const ObjScene::Material& material = _sponzaScene.GetMaterial(mesh->materialIndex);
				_renderContext->SetSamplerTexture(0, material.diffuseTexture);
				_renderContext->SetSamplerTexture(1, material.normalTexture);
				prevMatInd = mesh->materialIndex;
			}

			for (int32_t lightIndex : lightIndices)
			{
				const PointLight* light = _visibleLights[lightIndex];
				UniformLightData lightData = {
					math3d::vec4f(light->position, light->radius * _lightRadiusScale),
					math3d::vec4f(light->color, light->falloffExponent)
				};
				_ubufLightData->BufferSubData(0, sizeof(lightData), &lightData);

				_renderContext->DrawIndexed(gls::PrimitiveType::Triangles, static_cast<gls::intptr>(mesh->indexOffset) * 4, 0, mesh->numIndices);
			}
		}
	}

	_renderContext->DepthTestFunc(gls::CompareFunc::Less);
	_renderContext->EnableDepthWrite(true);
	_renderContext->EnableDepthTest(false);
	_renderContext->EnableBlending(false);
}

void DeferredRenderer::RenderForwardSinglePass()
{
	_renderContext->SetFramebuffer(_sceneBuffer);
	_renderContext->ClearColorBuffer(_sceneBuffer, 0, math3d::vec4f(0.0f, 0.0f, 0.0f, 0.0f));
	_renderContext->ClearDepthStencilBuffer(_sceneBuffer, 1.0f, 0);

	// First fill only the depth buffer.

	_renderContext->EnableDepthTest(true);
	_renderContext->EnableColorWrite(false, false, false, false);

	_renderContext->SetUniformBuffer(0, _ubufSceneXformData);
	_renderContext->SetVertexShader(_vertShaderDepthOnly);
	_renderContext->SetFragmentShader(nullptr);

	_renderContext->ActiveVertexFormat(_vertexFormat);
	_renderContext->VertexSource(0, _sponzaScene.GetVertexBuffer(), sizeof(ObjScene::Vertex), 0, 0);
	_renderContext->IndexSource(_sponzaScene.GetIndexBuffer(), gls::DataType::UnsignedInt);

	for (const ObjScene::Mesh* mesh : _visibleObjects)
	{
		_renderContext->DrawIndexed(gls::PrimitiveType::Triangles, static_cast<gls::intptr>(mesh->indexOffset) * 4, 0, mesh->numIndices);
	}

	// Draw lit objects.

	_renderContext->EnableColorWrite(true, true, true, true);
	_renderContext->DepthTestFunc(gls::CompareFunc::Equal);
	_renderContext->EnableDepthWrite(false);

	_renderContext->SetUniformBuffer(1, _ubufLightData);
	_renderContext->SetVertexShader(_vertShaderForward);
	_renderContext->SetFragmentShader(_fragShaderForwardSP);

	_renderContext->SetSamplerState(0, _samplerSurfaceTex);
	_renderContext->SetSamplerState(1, _samplerSurfaceTex);
	_renderContext->SetSamplerState(2, nullptr);
	_renderContext->SetSamplerTexture(2, _lightIndexTex);
	_renderContext->SetSamplerState(3, nullptr);
	_renderContext->SetSamplerTexture(3, _lightInfoTex);

	int prevMatInd = -1;

	for (size_t objInd = 0; objInd < _visibleObjects.size(); ++objInd)
	{
		const std::vector<int32_t>& lightIndices = _interactions[objInd];
		int32_t numLights = static_cast<int32_t>(lightIndices.size());

		if (numLights > 0)
		{
			const ObjScene::Mesh* mesh = _visibleObjects[objInd];

			if (mesh->materialIndex != prevMatInd)
			{
				const ObjScene::Material& material = _sponzaScene.GetMaterial(mesh->materialIndex);
				_renderContext->SetSamplerTexture(0, material.diffuseTexture);
				_renderContext->SetSamplerTexture(1, material.normalTexture);
				prevMatInd = mesh->materialIndex;
			}

			_ubufLightData->BufferSubData(0, sizeof(int32_t), &numLights);
			_lightIndexBuf->BufferSubData(0, sizeof(int32_t) * numLights, lightIndices.data());

			_renderContext->DrawIndexed(gls::PrimitiveType::Triangles, static_cast<gls::intptr>(mesh->indexOffset) * 4, 0, mesh->numIndices);
		}
	}

	_renderContext->DepthTestFunc(gls::CompareFunc::Less);
	_renderContext->EnableDepthWrite(true);
	_renderContext->EnableDepthTest(false);
}

void DeferredRenderer::RenderForwardTransparent()
{
	_renderContext->ActiveVertexFormat(_vertexFormat);
	_renderContext->VertexSource(0, _sponzaScene.GetVertexBuffer(), sizeof(ObjScene::Vertex), 0, 0);
	_renderContext->IndexSource(_sponzaScene.GetIndexBuffer(), gls::DataType::UnsignedInt);

	_renderContext->SetUniformBuffer(0, _ubufSceneXformData);
	_renderContext->SetUniformBuffer(1, _ubufLightData);
	_renderContext->SetVertexShader(_vertShaderForward);
	_renderContext->SetFragmentShader(_fragShaderForwardTransp);

	_renderContext->EnableDepthTest(true);
	_renderContext->EnableFaceCulling(false);
	_renderContext->EnableBlending(true);

	_renderContext->SetSamplerState(0, _samplerSurfaceTex);
	_renderContext->SetSamplerState(1, _samplerSurfaceTex);

	int prevMatInd = -1;

	for (size_t objInd = 0; objInd < _visibleTranspObjects.size(); ++objInd)
	{
		const std::vector<int32_t>& lightIndices = _transpInteractions[objInd];

		if (lightIndices.size() > 0)
		{
			const ObjScene::Mesh* mesh = _visibleTranspObjects[objInd];

			if (mesh->materialIndex != prevMatInd)
			{
				const ObjScene::Material& material = _sponzaScene.GetMaterial(mesh->materialIndex);
				_renderContext->SetSamplerTexture(0, material.diffuseTexture);
				_renderContext->SetSamplerTexture(1, material.normalTexture);
				prevMatInd = mesh->materialIndex;
			}

			int lightCount = 0;
			_renderContext->BlendingFunc(gls::BlendFunc::SrcAlpha, gls::BlendFunc::OneMinusSrcAlpha);

			for (int32_t lightIndex : lightIndices)
			{
				const PointLight* light = _visibleLights[lightIndex];
				UniformLightData lightData = {
					math3d::vec4f(light->position, light->radius * _lightRadiusScale),
					math3d::vec4f(light->color, light->falloffExponent)
				};
				_ubufLightData->BufferSubData(0, sizeof(lightData), &lightData);

				if (lightCount == 1)
				{
					// After the first light, change the blending function to accumulate colors
					// and set depth test to pass when equal.
					_renderContext->BlendingFunc(gls::BlendFunc::SrcAlpha, gls::BlendFunc::One);
					_renderContext->DepthTestFunc(gls::CompareFunc::Equal);
				}

				_renderContext->DrawIndexed(gls::PrimitiveType::Triangles, static_cast<gls::intptr>(mesh->indexOffset) * 4, 0, mesh->numIndices);
				++lightCount;
			}

			if (lightCount > 1)
				_renderContext->DepthTestFunc(gls::CompareFunc::Less);
		}
	}

	_renderContext->EnableDepthTest(false);
	_renderContext->EnableBlending(false);
	_renderContext->EnableFaceCulling(true);
}

void DeferredRenderer::RenderForwardTransparentSinglePass()
{
	_renderContext->ActiveVertexFormat(_vertexFormat);
	_renderContext->VertexSource(0, _sponzaScene.GetVertexBuffer(), sizeof(ObjScene::Vertex), 0, 0);
	_renderContext->IndexSource(_sponzaScene.GetIndexBuffer(), gls::DataType::UnsignedInt);

	_renderContext->SetUniformBuffer(0, _ubufSceneXformData);
	_renderContext->SetUniformBuffer(1, _ubufLightData);
	_renderContext->SetVertexShader(_vertShaderForward);
	_renderContext->SetFragmentShader(_fragShaderForwardTranspSP);

	_renderContext->EnableDepthTest(true);
	_renderContext->EnableFaceCulling(false);
	_renderContext->EnableBlending(true);
	_renderContext->BlendingFunc(gls::BlendFunc::SrcAlpha, gls::BlendFunc::OneMinusSrcAlpha);

	_renderContext->SetSamplerState(0, _samplerSurfaceTex);
	_renderContext->SetSamplerState(1, _samplerSurfaceTex);
	_renderContext->SetSamplerState(2, nullptr);
	_renderContext->SetSamplerTexture(2, _lightIndexTex);
	_renderContext->SetSamplerState(3, nullptr);
	_renderContext->SetSamplerTexture(3, _lightInfoTex);

	int prevMatInd = -1;

	for (size_t objInd = 0; objInd < _visibleTranspObjects.size(); ++objInd)
	{
		const std::vector<int32_t>& lightIndices = _transpInteractions[objInd];
		int32_t numLights = static_cast<int32_t>(lightIndices.size());

		if (numLights > 0)
		{
			const ObjScene::Mesh* mesh = _visibleTranspObjects[objInd];

			if (mesh->materialIndex != prevMatInd)
			{
				const ObjScene::Material& material = _sponzaScene.GetMaterial(mesh->materialIndex);
				_renderContext->SetSamplerTexture(0, material.diffuseTexture);
				_renderContext->SetSamplerTexture(1, material.normalTexture);
				prevMatInd = mesh->materialIndex;
			}

			_ubufLightData->BufferSubData(0, sizeof(int32_t), &numLights);
			_lightIndexBuf->BufferSubData(0, sizeof(int32_t) * numLights, lightIndices.data());

			_renderContext->DrawIndexed(gls::PrimitiveType::Triangles, static_cast<gls::intptr>(mesh->indexOffset) * 4, 0, mesh->numIndices);
		}
	}

	_renderContext->EnableDepthTest(false);
	_renderContext->EnableBlending(false);
	_renderContext->EnableFaceCulling(true);
}

void DeferredRenderer::RenderLightSources()
{
	_renderContext->SetUniformBuffer(0, _ubufSceneXformData);

	_renderContext->SetVertexShader(_vertShaderLightSource);
	_renderContext->SetFragmentShader(_fragShaderLightSource);
	_renderContext->ActiveVertexFormat(_vertFmtSphere);
	_renderContext->VertexSource(0, _sphereVertBuf, sizeof(math3d::vec3f), 0, 0);
	_renderContext->IndexSource(_sphereIndexBuf, gls::DataType::UnsignedShort);

	_renderContext->SetSamplerState(0, nullptr);
	_renderContext->SetSamplerTexture(0, _lightInfoTex);

	_renderContext->EnableDepthTest(true);

	_renderContext->DrawIndexedInstanced(gls::PrimitiveType::Triangles, 0, 0, _sphereIndexCount, 0, static_cast<gls::sizei>(_visibleLights.size()));

	_renderContext->EnableDepthTest(false);
}

void DeferredRenderer::Update(float frameTime)
{
	if (_renderContext == nullptr)
		return;

	_demoPlayer.Update(frameTime);

	// Update ImGui data.

	_imGuiIO->DeltaTime = frameTime;

	ImGui::NewFrame();

	// Update frame rate history for the UI graph.

	_framerateValueAddTime += frameTime;
	static float framerateAddPeriod = 2.0f;		// Wait 2 seconds for the first value to give ImGui time to get correct rolling average.
	if (_framerateValueAddTime > framerateAddPeriod)
	{
		framerateAddPeriod = 1.0f;
		_framerateValueAddTime = 0.0f;
		std::rotate(_framerateValues.begin(), _framerateValues.begin() + 1, _framerateValues.end());
		_framerateValues.back() = _imGuiIO->Framerate;
	}

	// If the demo or benchmark is running, update it.

	if (_runMode == RunMode::Benchmark)
		UpdateBenchmark(frameTime);
	else if (_runMode == RunMode::Demo)
		UpdateDemo();

	// Setup UI.

	if (_settingsDlgVisible)
		ImGuiSettingsDlg();

	if (_demosDlgVisible)
		ImGuiDemoDlg();

	if (_demoPlayer.GetState() == DemoPlayer::State::Recording)
		ImGuiRecordingOverlay(frameTime);

	if (_bmarkResultsDlgVisible)
		ImGuiBenchmarkResultsDlg();

	// Update camera, light positions and find lights and objects which are inside the current view frustum.
	// Light-object interactions are updated only for render paths that need them.

	UpdateCamera(frameTime);
	UpdateLights(frameTime);
	UpdateVisibleObjects();
	UpdateLightObjectInteractions();

	// If the vsync setting has changed, set the new swap interval here.

	int swapInterval = _vsync ? 1 : 0;
	if (swapInterval != _curSwapInterval)
	{
		_renderContext->SwapInterval(swapInterval);
		_curSwapInterval = swapInterval;
	}

	// Update shader uniform buffer per-frame data.

	UniformSceneXformData xformData;
	xformData.viewProjMatrix = _viewProjMat;
	xformData.viewport.set(0.0f, 0.0f, static_cast<float>(_viewportWidth), static_cast<float>(_viewportHeight));
	_ubufSceneXformData->BufferSubData(0, sizeof(UniformSceneXformData), &xformData);

	// Update light info buffer (necessary only for forward single pass rendering or when showing light sources).

	if (_showLightSources || _renderPath == RenderPath::ForwardSinglePass || (_renderPath == RenderPath::Deferred && _showTranspSurfaces))
	{
		UniformLightData lightInfo[MaxLights];
		size_t numVisLights = _visibleLights.size();
		for (size_t i = 0; i < numVisLights; ++i)
		{
			lightInfo[i].positionRadius.set(_visibleLights[i]->position, _visibleLights[i]->radius * _lightRadiusScale);
			lightInfo[i].colorFalloffExp.set(_visibleLights[i]->color, _visibleLights[i]->falloffExponent);
		}
		_lightInfoBuf->BufferSubData(0, sizeof(UniformLightData) * numVisLights, lightInfo);
	}
}

void DeferredRenderer::RenderImGui()
{
	ImDrawData* drawData = ImGui::GetDrawData();

	_renderContext->SetFramebuffer(nullptr);
	_renderContext->EnableBlending(true);
	_renderContext->BlendingFunc(gls::BlendFunc::SrcAlpha, gls::BlendFunc::OneMinusSrcAlpha);
	_renderContext->EnableFaceCulling(false);
	_renderContext->EnableScissorTest(true);

	_renderContext->SetVertexShader(_vertShaderImGui);
	_renderContext->SetFragmentShader(_fragShaderImGui);
	_renderContext->SetUniformBuffer(0, _ubufImGui);
	_renderContext->ActiveVertexFormat(_vertFmtImGui);
	_renderContext->IndexSource(_imGuiIndexBuffer, gls::DataType::UnsignedShort);
	_renderContext->VertexSource(0, _imGuiVertBuffer, sizeof(ImDrawVert), 0, 0);
	_renderContext->SetSamplerState(0, _samplerLinearClamp);

	ImVec2 displPos = drawData->DisplayPos;
	ImVec2 displSize = drawData->DisplaySize;
	ImVec2 fbScale = drawData->FramebufferScale;
	int fbWidth = static_cast<int>(displSize.x * fbScale.x);
	int fbHeight = static_cast<int>(displSize.y * fbScale.y);

	math3d::mat4f projMat;
	projMat.ortho(displPos.x, displPos.x + displSize.x, displPos.y + displSize.y, displPos.y, -1.0f, 1.0f);

	_ubufImGui->BufferSubData(0, sizeof(math3d::mat4f), projMat);

	for (int i = 0; i < drawData->CmdListsCount; ++i)
	{
		const ImDrawList* cmdList = drawData->CmdLists[i];

		_imGuiVertBuffer->BufferSubData(0, cmdList->VtxBuffer.Size * sizeof(ImDrawVert), cmdList->VtxBuffer.Data);
		_imGuiIndexBuffer->BufferSubData(0, cmdList->IdxBuffer.Size * sizeof(ImDrawIdx), cmdList->IdxBuffer.Data);

		for (int cmdInd = 0; cmdInd < cmdList->CmdBuffer.Size; ++cmdInd)
		{
			const ImDrawCmd* cmd = &cmdList->CmdBuffer[cmdInd];

			if (cmd->UserCallback)
			{
				cmd->UserCallback(cmdList, cmd);
			}
			else
			{
				ImVec4 clipRect;
				clipRect.x = (cmd->ClipRect.x - displPos.x) * fbScale.x;
				clipRect.y = (cmd->ClipRect.y - displPos.y) * fbScale.y;
				clipRect.z = (cmd->ClipRect.z - displPos.x) * fbScale.x;
				clipRect.w = (cmd->ClipRect.w - displPos.y) * fbScale.y;

				if (clipRect.x < fbWidth && clipRect.y < fbHeight && clipRect.z >= 0.0f && clipRect.w >= 0.0f)
				{
					_renderContext->SetSamplerTexture(0, static_cast<gls::ITexture2D*>(cmd->TextureId));
					_renderContext->Scissor(
						static_cast<int>(clipRect.x),
						static_cast<int>(fbHeight - clipRect.w),
						static_cast<int>(clipRect.z - clipRect.x),
						static_cast<int>(clipRect.w - clipRect.y));

					_renderContext->DrawIndexed(gls::PrimitiveType::Triangles, cmd->IdxOffset * sizeof(ImDrawIdx), cmd->VtxOffset, cmd->ElemCount);
				}
			}
		}
	}

	_renderContext->EnableBlending(false);
	_renderContext->EnableFaceCulling(true);
	_renderContext->EnableScissorTest(false);
}

void DeferredRenderer::ImGuiSettingsDlg()
{
	if (ImGui::Begin("Settings", &_settingsDlgVisible, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImVec4 teal(0.0f, 1.0f, 1.0f, 1.0f);
		ImVec4 orange(0.97f, 0.51f, 0.16f, 1.0f);
		ImVec4 yellow(1.0f, 1.0f, 0.0f, 1.0f);
		ImVec4 plum(0.3f, 0.4f, 0.7f, 1.0f);

		const gls::ContextInfo& ctxInfo = _renderContext->GetInfo();
		ImGui::TextColored(plum, "Renderer: %s", ctxInfo.renderer);
		ImGui::TextColored(plum, "Vendor: %s", ctxInfo.vendor);
		ImGui::TextColored(plum, "OpenGL version: %s", ctxInfo.versionString);
		ImGui::TextColored(plum, "GLSL version: %s", ctxInfo.shadingLanguageVersion);
		ImGui::TextColored(teal, "Click and drag left mouse button to look around.");
		ImGui::TextColored(teal, "Use w, s, a, d keys to move; F2 to show demo dialog.");
		int visibleObjects = static_cast<int>(_visibleObjects.size() + (_showTranspSurfaces ? _visibleTranspObjects.size() : 0));
		ImGui::TextColored(orange, "Objects in view: %d / %d", visibleObjects, _sponzaScene.GetMeshCount());
		ImGui::TextColored(orange, "Lights in view: %d / %d", static_cast<int>(_visibleLights.size()), static_cast<int>(_lights.size()));
		ImGui::TextColored(orange, "FPS: %.0f", _imGuiIO->Framerate);
		ImGui::PlotLines("##plot", _framerateValues.data(), static_cast<int>(_framerateValues.size()), 0, nullptr, 0.0f, FLT_MAX, ImVec2(400.0f, 100.0f));

		ImGui::Combo("renderer", reinterpret_cast<int*>(&_renderPath), "Forward multi-pass\0Forward single pass\0Deferred\0\0");
		
		if (_renderPath == RenderPath::Deferred)
			ImGui::Checkbox("Show G-buffer (tab)", &_showGBuffer);
		else
			_showGBuffer = false;

		ImGui::Checkbox("Move lights (m)", &_moveLights);
		ImGui::SameLine();
		ImGui::SliderFloat("##lightspeed", &_lightSourceSpeed, 10.0f, 300.0f, "%.0f cm/s");

		ImGui::Checkbox("Show light sources (l)", &_showLightSources);
		ImGui::Checkbox("Show transp. surfaces (t)", &_showTranspSurfaces);

		ImGui::Checkbox("V-sync (v)", &_vsync);

		ImGui::Text("FOV angle");
		ImGui::SameLine();
		if (ImGui::SliderFloat("##fovdeg", &_fovAngleDeg, MinFOV, MaxFOV, u8"%.0f\u00B0"))
			UpdateProjectionMatrix();
		ImGui::SameLine();
		if (ImGui::Button("reset##fov"))
		{
			_fovAngleDeg = DefaultFOV;
			UpdateProjectionMatrix();
		}

		ImGui::Text("Light radius scale");
		ImGui::SameLine();
		ImGui::SliderFloat("##radscale", &_lightRadiusScale, 0.5f, 3.0f, "%.1f x");
		ImGui::SameLine();
		if (ImGui::Button("reset##radscale"))
			_lightRadiusScale = 1.0f;

		ImGui::TextColored(yellow, "Create new light (space)");
		ImGui::SameLine();

		if (ImGui::RadioButton("Random", _newLightRandomParams))
			_newLightRandomParams = true;
		ImGui::SameLine();
		if (ImGui::RadioButton("Custom", !_newLightRandomParams))
			_newLightRandomParams = false;

		if (_newLightRandomParams)
		{
			float radiusMinMax[2] = { _newLightMinRandomRadius, _newLightMaxRandomRadius };
			ImGui::DragFloat2("Radius range", radiusMinMax, 1.0f, LightMinRadius, LightMaxRadius, "%.0f cm");
			_newLightMinRandomRadius = std::min(radiusMinMax[0], _newLightMaxRandomRadius);
			_newLightMaxRandomRadius = std::max(radiusMinMax[1], _newLightMinRandomRadius);

			float falloffExpMinMax[2] = { _newLightMinRandomFalloffExp, _newLightMaxRandomFalloffExp };
			ImGui::DragFloat2("Falloff exponent range", falloffExpMinMax, 0.001f, LightMinFalloffExp, LightMaxFalloffExp, "%.2f");
			_newLightMinRandomFalloffExp = std::min(falloffExpMinMax[0], _newLightMaxRandomFalloffExp);
			_newLightMaxRandomFalloffExp = std::max(falloffExpMinMax[1], _newLightMinRandomFalloffExp);
		}
		else
		{
			ImGui::ColorEdit3("Color", _newLightColor, ImGuiColorEditFlags_PickerHueWheel);
			ImGui::SliderFloat("Radius", &_newLightRadius, LightMinRadius, LightMaxRadius, "%.0f cm");
			ImGui::SliderFloat("Falloff exponent", &_newLightFalloffExponent, LightMinFalloffExp, LightMaxFalloffExp);
		}

		if (ImGui::Button("Recreate"))
		{
			CreateRandomLights(_randomLightsToCreate, _lightBoundsMin, _lightBoundsMax);
			_demoSampleLights = true;
		}
		ImGui::SameLine();
		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.78f);
		ImGui::SliderInt("##numlights", &_randomLightsToCreate, 1, MaxLights, (_randomLightsToCreate == 1) ? "%d light" : "%d lights");
		ImGui::PopItemWidth();

		if (ImGui::Button("Remove all lights (r)"))
			RemoveAllLights();
	}
	ImGui::End();
}

void DeferredRenderer::ImGuiDemoDlg()
{
	ImGui::SetNextWindowPos(ImVec2(_imGuiIO->DisplaySize.x * 0.5f, _imGuiIO->DisplaySize.y * 0.5f), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	if (ImGui::Begin("Demos", &_demosDlgVisible, ImGuiWindowFlags_AlwaysAutoResize))
	{
		if (ImGui::IsWindowAppearing())
			_demoPlayer.UpdateDemoList();

		const std::vector<std::string>& demoList = _demoPlayer.GetDemoList();
		ImGui::SetNextItemWidth(300);
		ImGui::ListBoxHeader("##demos", static_cast<int>(demoList.size()), 10);
		static int currentDemoInd = -1;
		if (demoList.empty())
			currentDemoInd = -1;
		for (size_t i = 0; i < demoList.size(); ++i)
		{
			bool itemSelected = (static_cast<int>(i) == currentDemoInd);
			ImGui::PushID(static_cast<int>(i));
			if (ImGui::Selectable(demoList[i].c_str(), itemSelected))
			{
				currentDemoInd = static_cast<int>(i);
			}
			if (itemSelected)
				ImGui::SetItemDefaultFocus();
			ImGui::PopID();
		}
		ImGui::ListBoxFooter();

		ImGui::Checkbox("Loop demo playback", &_loopDemoPlayback);

		if (ImGui::Button("New"))
			_newDemoDlgVisible = true;

		ImGui::SameLine();
		if (ImGui::Button("Delete"))
			if (currentDemoInd >= 0)
				_deleteDemoDlgVisible = true;

		ImGui::SameLine();
		if (ImGui::Button("Play"))
		{
			if (currentDemoInd >= 0)
			{
				PlayDemo(demoList[currentDemoInd].c_str());
				_demosDlgVisible = false;
			}
		}

		ImGui::SameLine();
		if (ImGui::Button("Benchmark"))
		{
			if (currentDemoInd >= 0)
			{
				StartBenchmark(demoList[currentDemoInd].c_str());
				_demosDlgVisible = false;
			}
		}

		if (_newDemoDlgVisible)
		{
			constexpr size_t bufSize = 100;
			static char demoName[bufSize] = {};

			if (!ImGui::IsPopupOpen("New demo"))
			{
				ImGui::OpenPopup("New demo");
				demoName[0] = '\0';
			}
			if (ImGui::BeginPopupModal("New demo", &_newDemoDlgVisible, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::Text("Name: "); ImGui::SameLine();

				if (ImGui::IsWindowAppearing())
					ImGui::SetKeyboardFocusHere();

				if (ImGui::InputText("##demoname", demoName, bufSize, ImGuiInputTextFlags_EnterReturnsTrue)
					|| ImGui::Button("Record", ImVec2(80, 0)))
				{
					if (demoName[0] != '\0')
					{
						RecordDemo(demoName);
						_newDemoDlgVisible = false;
						_demosDlgVisible = false;
						ImGui::CloseCurrentPopup();
					}
				}

				ImGui::SameLine();
				if (ImGui::Button("Cancel", ImVec2(80, 0)))
				{
					_newDemoDlgVisible = false;
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
		}

		if (_deleteDemoDlgVisible)
		{
			if (!ImGui::IsPopupOpen("Delete?"))
				ImGui::OpenPopup("Delete?");
			if (ImGui::BeginPopupModal("Delete?", &_deleteDemoDlgVisible, ImGuiWindowFlags_AlwaysAutoResize))
			{
				if (ImGui::Button("Yes", ImVec2(80, 0)))
				{
					if (currentDemoInd >= 0)
					{
						_demoPlayer.DeleteDemo(demoList[currentDemoInd].c_str());
						_deleteDemoDlgVisible = false;
						ImGui::CloseCurrentPopup();
					}
				}

				ImGui::SameLine();
				if (ImGui::Button("No", ImVec2(80, 0)))
				{
					_deleteDemoDlgVisible = false;
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
		}
	}
	ImGui::End();
}

void DeferredRenderer::ImGuiRecordingOverlay(float frameTime)
{
	_demoRecordingTimer += frameTime;

	ImGui::SetNextWindowPos(ImVec2(_imGuiIO->DisplaySize.x * 0.5f, 0.0f), ImGuiCond_Appearing, ImVec2(0.5f, 0.0f));
	if (ImGui::Begin("Recording", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoMouseInputs))
	{
		int minutes = static_cast<int>(_demoRecordingTimer / 60.0f);
		int seconds = static_cast<int>(std::fmod(_demoRecordingTimer, 60.0f));
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Recording %.2d:%.2d", minutes, seconds);
	}
	ImGui::End();
}

void DeferredRenderer::ImGuiBenchmarkResultsDlg()
{
	if (!ImGui::IsPopupOpen("Benchmark results"))
		ImGui::OpenPopup("Benchmark results");
	if (ImGui::BeginPopupModal("Benchmark results", &_bmarkResultsDlgVisible, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImVec4 white(1.0f, 1.0f, 1.0f, 1.0f);
		ImVec4 yellow(1.0f, 1.0f, 0.0f, 1.0f);
		ImVec4 rouge(0.7f, 0.4f, 0.3f, 1.0f);
		ImVec4 lime(0.3f, 0.7f, 0.4f, 1.0f);
		ImVec4 plum(0.3f, 0.4f, 0.7f, 1.0f);

		const char* rpathNames[] = {
			"Forward",
			"Forward SP",
			"Deferred",
		};

		ImGui::TextColored(yellow, "%29s%20s", "FPS", "dt (ms)");
		ImGui::Separator();

		for (int rpathInd = 0; rpathInd < CountOf(rpathNames); ++rpathInd)
		{
			const auto& results = _benchmarkData.results[rpathInd];

			ImGui::TextColored(white, "%-20s", rpathNames[rpathInd]);

			if (results.valid)
			{
				ImGui::TextColored(lime, "    Avg:"); ImGui::SameLine(); ImGui::TextColored(lime, "%20.2f%20.3f", results.averageFPS, results.averageDt);
				ImGui::TextColored(plum, "    Min:"); ImGui::SameLine(); ImGui::TextColored(plum, "%20.2f%20.3f", results.minFPS, results.minDt);
				ImGui::TextColored(rouge, "    Max:"); ImGui::SameLine(); ImGui::TextColored(rouge, "%20.2f%20.3f", results.maxFPS, results.maxDt);
			}
			else
			{
				ImGui::TextColored(lime, "    Avg:"); ImGui::SameLine(); ImGui::TextColored(lime, "%20s%20s", "--", "--");
				ImGui::TextColored(plum, "    Min:"); ImGui::SameLine(); ImGui::TextColored(plum, "%20s%20s", "--", "--");
				ImGui::TextColored(rouge, "    Max:"); ImGui::SameLine(); ImGui::TextColored(rouge, "%20s%20s", "--", "--");
			}

			ImGui::Separator();
		}

		ImGui::Dummy(ImVec2(ImGui::GetContentRegionAvail().x / 2.0f - 50.0f, 0));
		ImGui::SameLine();
		if (ImGui::Button("Close", ImVec2(80.0f, 0)))
		{
			_bmarkResultsDlgVisible = false;
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

void DeferredRenderer::Render()
{
	if (_renderContext == nullptr)
		return;

	if (_renderPath == RenderPath::Deferred)
	{
		RenderGeometryPass();

		if (_showGBuffer)
		{
			RenderGBufferPreview();
		}
		else
		{
			RenderLightingPass();
			if (_showLightSources)
				RenderLightSources();
			if (_showTranspSurfaces)
				RenderForwardTransparentSinglePass();

			_renderContext->BlitFramebuffer(_sceneBuffer, gls::ColorBuffer::Color0, 0, 0, _viewportWidth, _viewportHeight, nullptr, 0, 0, _viewportWidth, _viewportHeight, gls::COLOR_BUFFER_BIT, gls::TexFilter::Nearest);
		}
	}
	else if (_renderPath == RenderPath::ForwardSinglePass)
	{
		RenderForwardSinglePass();
		if (_showLightSources)
			RenderLightSources();
		if (_showTranspSurfaces)
			RenderForwardTransparentSinglePass();

		_renderContext->BlitFramebuffer(_sceneBuffer, gls::ColorBuffer::Color0, 0, 0, _viewportWidth, _viewportHeight, nullptr, 0, 0, _viewportWidth, _viewportHeight, gls::COLOR_BUFFER_BIT, gls::TexFilter::Nearest);
	}
	else
	{
		RenderForward();
		if (_showLightSources)
			RenderLightSources();
		if (_showTranspSurfaces)
			RenderForwardTransparent();

		_renderContext->BlitFramebuffer(_sceneBuffer, gls::ColorBuffer::Color0, 0, 0, _viewportWidth, _viewportHeight, nullptr, 0, 0, _viewportWidth, _viewportHeight, gls::COLOR_BUFFER_BIT, gls::TexFilter::Nearest);
	}

	ImGui::EndFrame();

	if (_settingsDlgVisible || _demosDlgVisible || _bmarkResultsDlgVisible ||
		(_demoPlayer.GetState() == DemoPlayer::State::Recording))
	{
		ImGui::Render();
		RenderImGui();
	}

	_renderContext->SetVertexShader(nullptr);
	_renderContext->SetFragmentShader(nullptr);

	_renderContext->SwapBuffers();
}

void DeferredRenderer::OnResize(int width, int height)
{
	if (!height)
		return;

	CreateFramebuffers(width, height);
	_renderContext->Viewport(0, 0, width, height);
	_viewportWidth = width;
	_viewportHeight = height;
	UpdateProjectionMatrix();

	_imGuiIO->DisplaySize.x = static_cast<float>(width);
	_imGuiIO->DisplaySize.y = static_cast<float>(height);
}

void DeferredRenderer::OnKeyDown(Key key, bool repeat)
{
	_imGuiIO->KeysDown[static_cast<int>(key)] = true;

	// If key is repeated or any of the modal dialogs are open, we do not process the key further.
	if (repeat || _newDemoDlgVisible || _deleteDemoDlgVisible || _bmarkResultsDlgVisible)
		return;

	if (_demoPlayer.GetState() == DemoPlayer::State::Ready ||
		_demoPlayer.GetState() == DemoPlayer::State::Recording)
	{
		switch (key)
		{
		case Key::W:
		case Key::UpArrow:
			_moveForward = true;
			break;

		case Key::S:
		case Key::DownArrow:
			_moveBack = true;
			break;

		case Key::A:
		case Key::LeftArrow:
			_moveLeft = true;
			break;

		case Key::D:
		case Key::RightArrow:
			_moveRight = true;
			break;

		case Key::E:
			_moveUp = true;
			break;

		case Key::Q:
			_moveDown = true;
			break;

		case Key::Tab:
			_showGBuffer = !_showGBuffer;
			break;

		case Key::M:
			_moveLights = !_moveLights;
			break;

		case Key::L:
			_showLightSources = !_showLightSources;
			break;

		case Key::T:
			_showTranspSurfaces = !_showTranspSurfaces;
			break;

		case Key::V:
			_vsync = !_vsync;
			break;

		case Key::F1:
			_settingsDlgVisible = !_settingsDlgVisible;
			break;

		case Key::F2:
			if (_demoPlayer.GetState() == DemoPlayer::State::Ready)
				_demosDlgVisible = !_demosDlgVisible;
			break;

		case Key::Enter: case Key::Escape:
			if (_demoPlayer.GetState() == DemoPlayer::State::Recording)
			{
				_demoPlayer.StopRecording();
				_demoPlayer.UpdateDemoList();
				_demosDlgVisible = true;
			}
			break;

		case Key::Space:
			CreateNewLight(_cameraPosition + _cameraForwardVector * 50.0f, _cameraForwardVector);
			_demoSampleLights = true;
			break;

		case Key::R:
			RemoveAllLights();
			_demoSampleLights = true;
			break;

		case Key::Shift:
			_shiftKeyPressed = true;
			break;
		}
	}
	else // Demo player is playing.
	{
		switch (key)
		{
		case Key::Escape:
			_demoPlaybackCanceled = true;
			_demoPlayer.StopPlaying();
			break;
		}
	}
}

void DeferredRenderer::OnKeyUp(Key key)
{
	_imGuiIO->KeysDown[static_cast<int>(key)] = false;

	switch (key)
	{
	case Key::W:
	case Key::UpArrow:
		_moveForward = false;
		break;

	case Key::S:
	case Key::DownArrow:
		_moveBack = false;
		break;

	case Key::A:
	case Key::LeftArrow:
		_moveLeft = false;
		break;

	case Key::D:
	case Key::RightArrow:
		_moveRight = false;
		break;

	case Key::E:
		_moveUp = false;
		break;

	case Key::Q:
		_moveDown = false;
		break;

	case Key::Shift:
		_shiftKeyPressed = false;
		break;
	}
}

void DeferredRenderer::OnChar(unsigned short ch)
{
	if (_imGuiIO->WantTextInput)
		_imGuiIO->AddInputCharacterUTF16(ch);
}

void DeferredRenderer::OnMouseMove(int x, int y)
{
	_imGuiIO->MousePos = ImVec2(static_cast<float>(x), static_cast<float>(y));
}

void DeferredRenderer::OnMouseDrag(int dx, int dy)
{
	const float MOUSE_SPEED = 0.004f;

	if (!_imGuiIO->WantCaptureMouse && _demoPlayer.GetState() != DemoPlayer::State::Playing)
	{
		_rotX -= dy * MOUSE_SPEED;
		_rotY -= dx * MOUSE_SPEED;

		_rotX = math3d::clamp(_rotX, math3d::deg2rad(-85.0f), math3d::deg2rad(85.0f));
		math3d::wrap(_rotY, 0.0f, math3d::TWO_PI);

		math3d::mat3f rotMat;
		rotMat.set_rotation_x(_rotX);
		rotMat.rotate_y(_rotY);
		_cameraForwardVector = math3d::vec3f(0.0f, 0.0f, -1.0f) * rotMat;
		_cameraRightVector = math3d::vec3f(1.0f, 0.0f, 0.0f) * rotMat;
	}
}

void DeferredRenderer::OnLBtnDown(int x, int y)
{
	_imGuiIO->MouseDown[0] = true;
}

void DeferredRenderer::OnRBtnDown(int x, int y)
{
	_imGuiIO->MouseDown[1] = true;
}

void DeferredRenderer::OnLBtnUp(int x, int y)
{
	_imGuiIO->MouseDown[0] = false;
}

void DeferredRenderer::OnRBtnUp(int x, int y)
{
	_imGuiIO->MouseDown[1] = false;
}

void DeferredRenderer::OnLostKeyboardFocus()
{
	_moveForward = false;
	_moveBack = false;
	_moveLeft = false;
	_moveRight = false;
	_moveUp = false;
	_moveDown = false;
	_shiftKeyPressed = false;
}

void DeferredRenderer::CreateRandomLights(int count, const math3d::vec3f& minPt, const math3d::vec3f& maxPt)
{
	_lights.clear();
	std::random_device rd;
	std::mt19937 gen { rd() };
	std::uniform_real_distribution<float> dist { 0.0f, 1.0f };
	std::uniform_real_distribution<float> distNeg { -1.0f, 1.0f };

	for (int i = 0; i < count; ++i)
	{
		math3d::vec3f position {
			math3d::lerp(minPt.x, maxPt.x, dist(gen)),
			math3d::lerp(minPt.y, maxPt.y, dist(gen)),
			math3d::lerp(minPt.z, maxPt.z, dist(gen))
		};

		math3d::vec3f moveDir {
			distNeg(gen),
			distNeg(gen),
			distNeg(gen)
		};
		moveDir.normalize();

		CreateNewLight(position, moveDir);
	}
}

void DeferredRenderer::CreateNewLight(const math3d::vec3f& position, const math3d::vec3f& moveDir)
{
	if (_lights.size() + 1 > MaxLights)
		return;

	PointLight light;

	light.position = position;
	light.moveDir = moveDir;

	if (_newLightRandomParams)
	{
		std::random_device rd;
		std::mt19937 gen { rd() };
		std::uniform_real_distribution<float> dist { 0.0f, 1.0f };
		std::uniform_real_distribution<float> distBrightClr { LightClrBrightThreshold, 1.0f };
		std::uniform_int_distribution<int> distClrChan { 0, 2 };

		light.color.x = dist(gen);
		light.color.y = dist(gen);
		light.color.z = dist(gen);
		if (light.color.x < LightClrBrightThreshold && light.color.y < LightClrBrightThreshold && light.color.z < LightClrBrightThreshold)
		{
			light.color[distClrChan(gen)] = distBrightClr(gen);
		}

		light.radius = math3d::lerp(_newLightMinRandomRadius, _newLightMaxRandomRadius, dist(gen));
		light.falloffExponent = _newLightMinRandomFalloffExp + dist(gen) * (_newLightMaxRandomFalloffExp - _newLightMinRandomFalloffExp);
	}
	else
	{
		light.color = _newLightColor;
		light.radius = _newLightRadius;
		light.falloffExponent = _newLightFalloffExponent;
	}

	_lights.push_back(light);
}

void DeferredRenderer::RemoveAllLights()
{
	_lights.clear();
}

void DeferredRenderer::CreateSphere(float radius, int slices, int stacks)
{
	if (slices < 4)
		slices = 4;
	if (stacks < 4)
		stacks = 4;

	int vert_per_row = slices + 1;

	_sphereVertCount = vert_per_row * (stacks - 1) + 2 * slices;
	int num_tris = slices * 2 * (stacks - 1);
	_sphereIndexCount = num_tris * 3;

	_sphereVertBuf = _renderContext->CreateBuffer(_sphereVertCount * sizeof(math3d::vec3f), nullptr, gls::BUFFER_MAP_WRITE_BIT);
	math3d::vec3f* vertices = (math3d::vec3f*)_sphereVertBuf->Map(gls::MAP_WRITE_BIT);
	_sphereIndexBuf = _renderContext->CreateBuffer(_sphereIndexCount * sizeof(gls::ushort), nullptr, gls::BUFFER_MAP_WRITE_BIT);
	gls::ushort* indices = (gls::ushort*)_sphereIndexBuf->Map(gls::MAP_WRITE_BIT);

	int i;
	for (i = 1; i < stacks; ++i)
	{
		float elev = float(i) / stacks * math3d::PI;
		float sin_el = sin(elev);
		float cos_el = cos(elev);

		for (int j = 0; j <= slices; ++j)
		{
			float azimuth = float(j) / slices * math3d::TWO_PI;
			float sin_az = sin(azimuth);
			float cos_az = cos(azimuth);

			math3d::vec3f& vert = vertices[(i - 1) * vert_per_row + j];
			vert.set(cos_az * sin_el * radius, cos_el * radius, sin_az * sin_el * radius);
		}
	}

	int n_pole_i = vert_per_row * (stacks - 1);
	int s_pole_i = vert_per_row * (stacks - 1) + slices;

	for (i = 0; i < slices; ++i)
	{
		math3d::vec3f& vert_n = vertices[n_pole_i + i];
		vert_n.set(0, radius, 0);
	}

	for (i = 0; i < slices; ++i)
	{
		math3d::vec3f& vert_s = vertices[s_pole_i + i];
		vert_s.set(0, -radius, 0);
	}

	// calc indices
	gls::ushort* ind = indices;
	for (i = 0; i < stacks - 2; ++i)
	{
		int first_i = i * vert_per_row;
		for (int j = 0; j < slices; ++j)
		{
			gls::ushort v_index = first_i + j;

			ind[0] = v_index;
			ind[1] = v_index + 1;
			ind[2] = v_index + vert_per_row;

			ind[3] = v_index + 1;
			ind[4] = v_index + vert_per_row + 1;
			ind[5] = v_index + vert_per_row;

			ind += 6;
		}
	}

	for (i = 0; i < slices; ++i)
	{
		ind[0] = i;
		ind[1] = n_pole_i + i;
		ind[2] = i + 1;

		ind += 3;
	}

	int last_row_i = vert_per_row * (stacks - 2);
	for (i = 0; i < slices; ++i)
	{
		ind[0] = s_pole_i + i;
		ind[1] = last_row_i + i;
		ind[2] = ind[1] + 1;

		ind += 3;
	}

	_sphereIndexBuf->Unmap();
	_sphereVertBuf->Unmap();

	gls::VertexAttribDesc desc[] =
	{
		{ 0, 0, 3, gls::DataType::Float, false, false, 0 },
	};
	_vertFmtSphere = _renderContext->CreateVertexFormat(desc, CountOf(desc));
}

void DeferredRenderer::DestroySphere()
{
	_renderContext->DestroyBuffer(_sphereVertBuf);
	_sphereVertBuf = nullptr;
	_renderContext->DestroyBuffer(_sphereIndexBuf);
	_sphereIndexBuf = nullptr;
	_renderContext->DestroyVertexFormat(_vertFmtSphere);
	_vertFmtSphere = nullptr;
}

void DeferredRenderer::UpdateCamera(float frameTime)
{
	// Calculate the movement vector and update the view and view-projection matrices.

	float forwardMovement = 0.0f;
	float sideMovement = 0.0f;
	float upMovement = 0.0f;

	if (_moveForward && !_moveBack)
		forwardMovement = 1.0f;
	else if (_moveBack && !_moveForward)
		forwardMovement = -1.0f;

	if (_moveLeft && !_moveRight)
		sideMovement = -1.0f;
	else if (_moveRight && !_moveLeft)
		sideMovement = 1.0f;

	if (_moveUp && !_moveDown)
		upMovement = 1.0f;
	else if (_moveDown && !_moveUp)
		upMovement = -1.0f;

	float ds = (_shiftKeyPressed ? FastMovementSpeed : MovementSpeed) * frameTime;
	math3d::vec3f upVector = math3d::cross(_cameraRightVector, _cameraForwardVector);
	_cameraPosition +=
		_cameraForwardVector * forwardMovement * ds +
		_cameraRightVector * sideMovement * ds +
		upVector * upMovement * ds;

	_viewMat.look_at(_cameraPosition, _cameraPosition + _cameraForwardVector, upVector);
	math3d::mul(_viewProjMat, _viewMat, _projMat);
}

void DeferredRenderer::UpdateProjectionMatrix()
{
	_projMat.perspective(math3d::deg2rad(_fovAngleDeg), float(_viewportWidth) / _viewportHeight, 10.0f, 4000.0f);
	ExtractFrustumPlanes(_projMat, _frustumPlanes);
}

void DeferredRenderer::UpdateLights(float frameTime)
{
	_visibleLights.clear();

	for (PointLight& light :_lights)
	{
		if (_moveLights)
		{
			light.position += light.moveDir * _lightSourceSpeed * frameTime;
			if (light.position.x < _lightBoundsMin.x)
			{
				light.position.x = _lightBoundsMin.x + (_lightBoundsMin.x - light.position.x);
				light.moveDir.x = -light.moveDir.x;
			}
			if (light.position.x > _lightBoundsMax.x)
			{
				light.position.x = _lightBoundsMax.x - (light.position.x - _lightBoundsMax.x);
				light.moveDir.x = -light.moveDir.x;
			}

			if (light.position.y < _lightBoundsMin.y)
			{
				light.position.y = _lightBoundsMin.y + (_lightBoundsMin.y - light.position.y);
				light.moveDir.y = -light.moveDir.y;
			}
			if (light.position.y > _lightBoundsMax.y)
			{
				light.position.y = _lightBoundsMax.y - (light.position.y - _lightBoundsMax.y);
				light.moveDir.y = -light.moveDir.y;
			}

			if (light.position.z < _lightBoundsMin.z)
			{
				light.position.z = _lightBoundsMin.z + (_lightBoundsMin.z - light.position.z);
				light.moveDir.z = -light.moveDir.z;
			}
			if (light.position.z > _lightBoundsMax.z)
			{
				light.position.z = _lightBoundsMax.z - (light.position.z - _lightBoundsMax.z);
				light.moveDir.z = -light.moveDir.z;
			}
		}

		if (ViewSpaceSphereInsideFrustum(light.position * _viewMat, light.radius * _lightRadiusScale, _frustumPlanes))
			_visibleLights.push_back(&light);
	}
}

void DeferredRenderer::UpdateVisibleObjects()
{
	_visibleObjects.clear();
	_visibleTranspObjects.clear();

	for (int meshInd = 0; meshInd < _sponzaScene.GetMeshCount(); ++meshInd)
	{
		const ObjScene::Mesh& mesh = _sponzaScene.GetMesh(meshInd);

		if (mesh.materialIndex >= 0)
		{
			const ObjScene::Material& material = _sponzaScene.GetMaterial(mesh.materialIndex);

			if (material.diffuseTexture != nullptr && material.normalTexture != nullptr &&
				(!material.transparent || _showTranspSurfaces))
			{
				auto centerPt = (mesh.maxPt + mesh.minPt) * 0.5f;
				auto vec = (mesh.maxPt - mesh.minPt) * 0.5f;

				if (//std::max(vec.x, std::max(vec.y, vec.z)) > 500.0f ||
					ViewSpaceBBoxInsideFrustum(
						centerPt * _viewMat,
						math3d::transform_dir(math3d::vec3f(vec.x, 0.0f, 0.0f), _viewMat),
						math3d::transform_dir(math3d::vec3f(0.0f, vec.y, 0.0f), _viewMat),
						math3d::transform_dir(math3d::vec3f(0.0f, 0.0f, vec.z), _viewMat),
						_frustumPlanes))
				{
					if (material.transparent)
						_visibleTranspObjects.push_back(&mesh);
					else
						_visibleObjects.push_back(&mesh);
				}
			}
		}
	}	
}

void DeferredRenderer::UpdateLightObjectInteractions()
{
	if (_renderPath != RenderPath::Deferred)
	{
		_interactions.resize(_visibleObjects.size());

		for (size_t objInd = 0; objInd < _visibleObjects.size(); ++objInd)
		{
			const ObjScene::Mesh* obj = _visibleObjects[objInd];
			_interactions[objInd].clear();

			for (size_t lightInd = 0; lightInd < _visibleLights.size(); ++lightInd)
			{
				if (AABBOverlapsSphere(obj->minPt, obj->maxPt, _visibleLights[lightInd]->position, _visibleLights[lightInd]->radius * _lightRadiusScale))
					_interactions[objInd].push_back(static_cast<int32_t>(lightInd));
			}
		}
	}

	if (_showTranspSurfaces)
	{
		_transpInteractions.resize(_visibleTranspObjects.size());

		for (size_t objInd = 0; objInd < _visibleTranspObjects.size(); ++objInd)
		{
			const ObjScene::Mesh* obj = _visibleTranspObjects[objInd];
			_transpInteractions[objInd].clear();

			for (size_t lightInd = 0; lightInd < _visibleLights.size(); ++lightInd)
			{
				if (AABBOverlapsSphere(obj->minPt, obj->maxPt, _visibleLights[lightInd]->position, _visibleLights[lightInd]->radius * _lightRadiusScale))
					_transpInteractions[objInd].push_back(static_cast<int32_t>(lightInd));
			}
		}
	}
}

void DeferredRenderer::RecordDemo(const char* demoName)
{
	_demoSampleLights = true;
	_demoRecordingTimer = 0.0f;
	_demoPlayer.StartRecording(demoName);
}

void DeferredRenderer::PlayDemo(const char* demoName)
{
	if (_demoPlayer.LoadDemo(demoName))
	{
		_demoSampleLights = true;
		_demoPlayer.TakeSamplesSnapshot();
		_demoPlayer.StartPlaying();
		_settingsDlgVisible = false;
		_demoPlaybackCanceled = false;
		_runMode = RunMode::Demo;
	}
}

void DeferredRenderer::UpdateDemo()
{
	if (_demoPlayer.GetState() == DemoPlayer::State::Ready)
	{
		if (_loopDemoPlayback && !_demoPlaybackCanceled)
		{
			_demoPlayer.StartPlaying();
		}
		else
		{
			_demoPlayer.UnloadDemo();
			_demoPlayer.RestoreSamplesSnapshot();
			_runMode = RunMode::Normal;
		}
	}
}

void DeferredRenderer::PlayerSampleUpdate(int32_t id, size_t size, const void* data)
{
	auto copyDataTo = [data, size](auto& destVar) {
		auto srcBegin = reinterpret_cast<const char*>(data);
		auto srcEnd = reinterpret_cast<const char*>(data) + size;
		auto dest = reinterpret_cast<char*>(&destVar);
		std::copy(srcBegin, srcEnd, dest);
	};

	switch (id)
	{
	case DemoDataId::CamPosition:
		copyDataTo(_cameraPosition);
		break;

	case DemoDataId::CamForwardVec:
		copyDataTo(_cameraForwardVector);
		break;

	case DemoDataId::CamRightVec:
		copyDataTo(_cameraRightVector);
		break;

	case DemoDataId::LightMovement:
		copyDataTo(_moveLights);
		break;

	case DemoDataId::LightMovementSpeed:
		copyDataTo(_lightSourceSpeed);
		break;

	case DemoDataId::LightRadiusScale:
		copyDataTo(_lightRadiusScale);
		break;

	case DemoDataId::Lights:
		_lights.resize(size / sizeof(PointLight));
		copyDataTo(*_lights.data());
		break;

	case DemoDataId::ShowTranspSurfaces:
		copyDataTo(_showTranspSurfaces);
		break;

	case DemoDataId::FOVAngle:
		copyDataTo(_fovAngleDeg);
		UpdateProjectionMatrix();
		break;
	}
}

void DeferredRenderer::RecorderSamplingFunc()
{
	_demoPlayer.RecordSample(DemoDataId::CamPosition, DemoPlayer::RecCond::IfChanged,  sizeof(_cameraPosition), _cameraPosition);
	_demoPlayer.RecordSample(DemoDataId::CamForwardVec, DemoPlayer::RecCond::IfChanged, sizeof(_cameraForwardVector), _cameraForwardVector);
	_demoPlayer.RecordSample(DemoDataId::CamRightVec, DemoPlayer::RecCond::IfChanged, sizeof(_cameraRightVector), _cameraRightVector);
	_demoPlayer.RecordSample(DemoDataId::LightMovement, DemoPlayer::RecCond::IfChanged, sizeof(_moveLights), &_moveLights);
	_demoPlayer.RecordSample(DemoDataId::LightMovementSpeed, DemoPlayer::RecCond::IfChanged, sizeof(_lightSourceSpeed), &_lightSourceSpeed);
	_demoPlayer.RecordSample(DemoDataId::LightRadiusScale, DemoPlayer::RecCond::IfChanged, sizeof(_lightRadiusScale), &_lightRadiusScale);
	_demoPlayer.RecordSample(DemoDataId::ShowTranspSurfaces, DemoPlayer::RecCond::IfChanged, sizeof(_showTranspSurfaces), &_showTranspSurfaces);
	_demoPlayer.RecordSample(DemoDataId::FOVAngle, DemoPlayer::RecCond::IfChanged, sizeof(_fovAngleDeg), &_fovAngleDeg);

	if (_demoSampleLights)
	{
		_demoPlayer.RecordSample(DemoDataId::Lights, DemoPlayer::RecCond::Always, _lights.size() * sizeof(PointLight), _lights.data());
		_demoSampleLights = false;
	}
}

void DeferredRenderer::StartBenchmark(const char* demoName)
{
	if (_demoPlayer.LoadDemo(demoName))
	{
		_benchmarkData.currentRenderPath = 0;
		_benchmarkData.oldRenderPath = _renderPath;
		_benchmarkData.oldShowLightSources = _showLightSources;
		_benchmarkData.oldVsync = _vsync;
		_benchmarkData.framesToSkip = BenchmarkData::NumStartFramesToSkip;
		_demoPlaybackCanceled = false;

		_runMode = RunMode::Benchmark;
		_showLightSources = false;
		_renderPath = RenderPath::Forward;
		_vsync = false;
		_settingsDlgVisible = false;

		_demoSampleLights = true;
		_demoPlayer.TakeSamplesSnapshot();
		_demoPlayer.StartPlaying();
	}
}

void DeferredRenderer::UpdateBenchmark(float frameTime)
{
	if (_demoPlayer.GetState() == DemoPlayer::State::Playing)
	{
		if (_benchmarkData.framesToSkip == 0)
		{
			// Add current frame time in milliseconds.
			auto& results = _benchmarkData.results[_benchmarkData.currentRenderPath];
			results.frameTimes.push_back(frameTime * 1000.0f);
		}
		else
		{
			_benchmarkData.framesToSkip--;
		}
	}
	else if (_demoPlayer.GetState() == DemoPlayer::State::Ready)
	{
		if (_benchmarkData.currentRenderPath == 2 || _demoPlaybackCanceled)
		{
			// Benchmark is finished. Play the demo once more in a loop with a fixed step to
			// get numbers of visible objects, lights and interactions.

			struct LightObjectVisAndInteractions
			{
				size_t numVisibleLights;
				size_t numVisibleObjects;
				size_t numInteractions;
			};

			std::vector<LightObjectVisAndInteractions> lightObjVisAndInteractions;
			_renderPath = RenderPath::Forward;	// Set to forward so that interactions are calculated.
			constexpr float FixedDtStep = 1.0f / 60.0f;
			
			_demoPlayer.StartPlaying();

			auto accumOp = [](const auto& a, const auto& b) { return a + b.size(); };

			while (_demoPlayer.GetState() == DemoPlayer::State::Playing)
			{
				_demoPlayer.Update(FixedDtStep);
				UpdateCamera(FixedDtStep);
				UpdateLights(FixedDtStep);
				UpdateVisibleObjects();
				UpdateLightObjectInteractions();

				lightObjVisAndInteractions.push_back({
					_visibleLights.size(),
					_visibleObjects.size() + (_showTranspSurfaces ? _visibleTranspObjects.size() : 0),
					std::accumulate(_interactions.begin(), _interactions.end(), static_cast<size_t>(0), accumOp) +
						(_showTranspSurfaces ? std::accumulate(_transpInteractions.begin(), _transpInteractions.end(), static_cast<size_t>(0), accumOp) : 0)
				});
			}

			_demoPlayer.UnloadDemo();
			_demoPlayer.RestoreSamplesSnapshot();

			// Calculate statistical data for the benchmark results dialog.

			for (auto& results : _benchmarkData.results)
			{
				results.valid = !results.frameTimes.empty();

				if (!results.valid)
					continue;

				// Calculate minimum, maximum and average frame times;
				// calculate frames per second rolling average values over 60 frames.
				float rcpCount = 1.0f / results.frameTimes.size();
				std::vector<float> fpsValues;
				std::array<float, 60> rollingBuf = { };
				float timeAccum = 0.0f;
				int numFrames = 0;
				bool addFrames = false;

				results.minDt = results.maxDt = results.frameTimes[0];
				results.averageDt = 0.0f;

				for (float dt : results.frameTimes)
				{
					results.averageDt += dt * rcpCount;

					if (dt < results.minDt)
						results.minDt = dt;

					if (dt > results.maxDt)
						results.maxDt = dt;

					timeAccum += dt - rollingBuf[numFrames];
					rollingBuf[numFrames] = dt;
					numFrames = (numFrames + 1) % static_cast<int>(rollingBuf.size());

					// Start adding FPS values after we wrap the rolling buffer for the first time.
					if (addFrames)
						fpsValues.push_back(1000.0f / (timeAccum / rollingBuf.size()));
					else if (numFrames == 0)
						addFrames = true;
				}

				// Calculate minimum, maximum and average FPS.

				if (fpsValues.empty())
				{
					results.minFPS = 0.0f;
					results.maxFPS = 0.0f;
					results.averageFPS = 0.0f;
				}
				else
				{
					float rcpCount = 1.0f / fpsValues.size();
					results.minFPS = results.maxFPS = fpsValues[0];
					results.averageFPS = 0.0f;

					for (float fps : fpsValues)
					{
						results.averageFPS += fps * rcpCount;

						if (fps < results.minFPS)
							results.minFPS = fps;

						if (fps > results.maxFPS)
							results.maxFPS = fps;
					}
				}				
			}

			// Write frame data, numbers of visible lights, objects and interactions to CSV file.

			std::string dirPath = GetFullPath("../Benchmarks/");
			if (!std::filesystem::exists(dirPath))
			{
				std::error_code ec;
				std::filesystem::create_directory(dirPath, ec);
			}
			auto now = std::chrono::system_clock::now();
			auto ttNow = std::chrono::system_clock::to_time_t(now);
			std::stringstream csvFileName;
			csvFileName << dirPath << "results_" << std::put_time(std::localtime(&ttNow), "%F_%H-%M-%S") << "__" << _viewportWidth << "x" << _viewportHeight << ".bmark";

			struct Punct : std::numpunct<char>
			{
			protected:
				virtual char_type do_decimal_point() const override { return '.'; }
				virtual std::string do_grouping() const override { return ""; }
			};

			std::ofstream csvFile(csvFileName.str(), std::ios::binary);

			if (csvFile.good())
			{
				csvFile.imbue(std::locale(csvFile.getloc(), new Punct));
				csvFile << "ForwardTime,ForwardDt,ForwardSPTime,ForwardSPDt,DeferredTime,DeferredDt,"
					"VisInterTime,NumVisObjects,NumVisLights,NumInteractions\n";

				float intrTime = 0.0f, rndrTimes[3] = {};
				bool somethingToWrite = true;
				for (size_t i = 0; somethingToWrite; ++i)
				{
					somethingToWrite = false;
					for (size_t rpInd = 0; rpInd < 3; ++rpInd)
					{
						if (i < _benchmarkData.results[rpInd].frameTimes.size())
						{
							csvFile << rndrTimes[rpInd] << "," << _benchmarkData.results[rpInd].frameTimes[i] << ",";
							rndrTimes[rpInd] += _benchmarkData.results[rpInd].frameTimes[i] / 1000.0f;
							somethingToWrite = true;
						}
						else
						{
							csvFile << ",,";
						}
					}

					if (i < lightObjVisAndInteractions.size())
					{
						csvFile << intrTime << ","
							<< lightObjVisAndInteractions[i].numVisibleObjects << ","
							<< lightObjVisAndInteractions[i].numVisibleLights << ","
							<< lightObjVisAndInteractions[i].numInteractions << "\n";
						intrTime += FixedDtStep;
						somethingToWrite = true;
					}
					else
					{
						csvFile << ",,,\n";
					}
				}
			}

			// We can now discard frame times.
			for (auto& results : _benchmarkData.results)
				results.frameTimes.clear();

			// Revert old settings and activate the benchmark results popup dialog.

			_showLightSources = _benchmarkData.oldShowLightSources;
			_renderPath = _benchmarkData.oldRenderPath;
			_vsync = _benchmarkData.oldVsync;

			_bmarkResultsDlgVisible = true;
			_runMode = RunMode::Normal;
		}
		else
		{
			// Set next rendering path and restart the demo player.
			_benchmarkData.currentRenderPath++;
			_benchmarkData.framesToSkip = BenchmarkData::NumStartFramesToSkip;
			_renderPath = static_cast<RenderPath>(_benchmarkData.currentRenderPath);
			_demoPlayer.StartPlaying();
		}
	}
}
