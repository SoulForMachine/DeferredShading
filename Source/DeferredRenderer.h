#ifndef _DEFERRED_RENDERER_H_
#define _DEFERRED_RENDERER_H_

#include <vector>
#include <array>
#include <Math/math3d.h>
#include <GLSlayer/RenderContext.h>
#include <imgui/imgui.h>
#include "IRenderer.h"
#include "Console.h"
#include "ObjScene.h"
#include "DemoPlayer.h"


class DeferredRenderer : public IRenderer
{
public:
	DeferredRenderer() = default;
	~DeferredRenderer();

	virtual bool Init(gls::CreateContextInfo& info) override;
	virtual void Deinit() override;
	virtual gls::FramebufferFormat GetFramebufferFormat() override;
	virtual void Update(float frameTime) override;
	virtual void Render() override;
	virtual void OnResize(int width, int height) override;
	virtual void OnKeyDown(Key key, bool repeat) override;
	virtual void OnKeyUp(Key key) override;
	virtual void OnChar(unsigned short ch) override;
	virtual void OnMouseMove(int x, int y) override;
	virtual void OnMouseDrag(int dx, int dy) override;
	virtual void OnLBtnDown(int x, int y) override;
	virtual void OnRBtnDown(int x, int y) override;
	virtual void OnLBtnUp(int x, int y) override;
	virtual void OnRBtnUp(int x, int y) override;
	virtual void OnLostKeyboardFocus() override;

private:
	static constexpr int MaxLights = 1000;
	static constexpr float MovementSpeed = 200.0f;
	static constexpr float FastMovementSpeed = 400.0f;
	static constexpr float LightMinFalloffExp = 0.9f;
	static constexpr float LightMaxFalloffExp = 1.4f;
	static constexpr float LightClrBrightThreshold = 0.7f;
	static constexpr float LightMinRadius = 100.0f;
	static constexpr float LightMaxRadius = 1000.0f;
	static constexpr float DefaultFOV = 70.0f;
	static constexpr float MinFOV = 30.0f;
	static constexpr float MaxFOV = 120.0f;

	enum class RenderPath : int
	{
		Forward,
		ForwardSinglePass,
		Deferred,
	};

	enum class RunMode
	{
		Normal,
		Demo,
		Benchmark
	};

	enum DemoDataId : int32_t
	{
		CamPosition,
		CamForwardVec,
		CamRightVec,
		Lights,
		LightMovement,
		LightMovementSpeed,
		LightRadiusScale,
		ShowTranspSurfaces,
		FOVAngle,
	};

	struct PointLight
	{
		math3d::vec3f position;
		math3d::vec3f moveDir;
		math3d::vec3f color;
		float radius;
		float falloffExponent;
	};

	struct BenchmarkData
	{
		static constexpr int NumStartFramesToSkip = 5;

		struct Results
		{
			std::vector<float> frameTimes;
			float averageDt;
			float minDt;
			float maxDt;
			float averageFPS;
			float minFPS;
			float maxFPS;
			bool valid;
		};
		
		std::array<Results, 3> results;	// Results for 3 render paths.
		int currentRenderPath;
		int framesToSkip;
		RenderPath oldRenderPath;
		bool oldVsync;
		bool oldShowLightSources;
	};

	gls::IVertexShader* LoadVertexShader(const char* fileName);
	gls::IFragmentShader* LoadFragmentShader(const char* fileName);
	void CreateFramebuffers(int width, int height);
	void DestroyFramebuffers();
	void RenderGeometryPass();
	void RenderLightingPass();
	void RenderGBufferPreview();
	void RenderForward();
	void RenderForwardSinglePass();
	void RenderForwardTransparent();
	void RenderForwardTransparentSinglePass();
	void RenderLightSources();
	void RenderImGui();
	void ImGuiSettingsDlg();
	void ImGuiDemoDlg();
	void ImGuiRecordingOverlay(float frameTime);
	void ImGuiBenchmarkResultsDlg();
	void CreateRandomLights(int count, const math3d::vec3f& min_pt, const math3d::vec3f& max_pt);
	void CreateNewLight(const math3d::vec3f& position, const math3d::vec3f& moveDir);
	void RemoveAllLights();
	void CreateSphere(float radius, int slices, int stacks);
	void DestroySphere();
	void UpdateCamera(float frameTime);
	void UpdateProjectionMatrix();
	void UpdateLights(float frameTime);
	void UpdateVisibleObjects();
	void UpdateLightObjectInteractions();

	void RecordDemo(const char* demoName);
	void PlayDemo(const char* demoName);
	void UpdateDemo();
	void PlayerSampleUpdate(int32_t id, size_t size, const void* data);
	void RecorderSamplingFunc();
	void StartBenchmark(const char* demoName);
	void UpdateBenchmark(float frameTime);

	gls::IRenderContext* _renderContext = nullptr;
	gls::IFramebuffer* _gbuffer = nullptr;
	gls::ITexture2D* _texDiffuse = nullptr;
	gls::ITexture2D* _texNormal = nullptr;
	gls::ITexture2D* _texPosition = nullptr;
	gls::ITexture2D* _depthBuffer = nullptr;
	gls::IFramebuffer* _sceneBuffer = nullptr;
	gls::ITexture2D* _texSceneColor = nullptr;
	gls::ITextureBuffer* _lightInfoTex = nullptr;
	gls::IBuffer* _lightInfoBuf = nullptr;
	gls::ITextureBuffer* _lightIndexTex = nullptr;
	gls::IBuffer* _lightIndexBuf = nullptr;

	gls::IFragmentShader* _fragShaderGeometryPass = nullptr;
	gls::IVertexShader* _vertShaderScreenSpace = nullptr;
	gls::IFragmentShader* _fragShaderVisGBuffer = nullptr;
	gls::IVertexShader* _vertShaderLightingPass = nullptr;
	gls::IFragmentShader* _fragShaderLightingPass = nullptr;
	gls::IVertexShader* _vertShaderLightSource = nullptr;
	gls::IFragmentShader* _fragShaderLightSource = nullptr;
	gls::IVertexShader* _vertShaderImGui = nullptr;
	gls::IFragmentShader* _fragShaderImGui = nullptr;
	gls::IVertexShader* _vertShaderForward = nullptr;
	gls::IFragmentShader* _fragShaderForward = nullptr;
	gls::IFragmentShader* _fragShaderForwardSP = nullptr;
	gls::IFragmentShader* _fragShaderForwardTransp = nullptr;
	gls::IFragmentShader* _fragShaderForwardTranspSP = nullptr;
	gls::IVertexShader* _vertShaderDepthOnly = nullptr;

	gls::IVertexFormat* _vertexFormat = nullptr;
	gls::IVertexFormat* _vertFmtScreenRect = nullptr;
	gls::IVertexFormat* _vertFmtSphere = nullptr;
	gls::IVertexFormat* _vertFmtImGui = nullptr;

	gls::IBuffer* _ubufSceneXformData = nullptr;
	gls::IBuffer* _ubufLightData = nullptr;
	gls::IBuffer* _ubufGbufferTexViewData = nullptr;
	gls::IBuffer* _ubufImGui = nullptr;

	gls::IBuffer* _rectVertBuf = nullptr;
	gls::IBuffer* _sphereVertBuf = nullptr;
	gls::IBuffer* _sphereIndexBuf = nullptr;
	gls::IBuffer* _imGuiVertBuffer = nullptr;
	gls::IBuffer* _imGuiIndexBuffer = nullptr;

	gls::ISamplerState* _samplerSurfaceTex = nullptr;
	gls::ISamplerState* _samplerLinearClamp = nullptr;
	gls::ISamplerState* _samplerGBuffer = nullptr;

	ObjScene _sponzaScene;
	Console _console;
	DemoPlayer _demoPlayer;
	ImGuiIO* _imGuiIO = nullptr;
	RenderPath _renderPath = RenderPath::Deferred;
	RunMode _runMode = RunMode::Normal;
	math3d::mat4f _viewMat, _projMat, _viewProjMat;
	std::vector<PointLight> _lights;
	std::vector<const PointLight*> _visibleLights;
	std::vector<const ObjScene::Mesh*> _visibleObjects;
	std::vector<const ObjScene::Mesh*> _visibleTranspObjects;
	std::vector<std::vector<int32_t>> _interactions;
	std::vector<std::vector<int32_t>> _transpInteractions;
	std::vector<math3d::vec4f> _frustumPlanes;
	std::vector<float> _framerateValues;
	float _framerateValueAddTime = 0.0f;
	float _fovAngleDeg = DefaultFOV;
	int _sphereVertCount = 0;
	int _sphereIndexCount = 0;
	math3d::vec3f _sceneBoundsMin;
	math3d::vec3f _sceneBoundsMax;
	math3d::vec3f _lightBoundsMin;
	math3d::vec3f _lightBoundsMax;
	int _viewportWidth = 0;
	int _viewportHeight = 0;
	math3d::vec3f _cameraPosition;
	math3d::vec3f _cameraForwardVector;
	math3d::vec3f _cameraRightVector;
	bool _moveForward = false;
	bool _moveBack = false;
	bool _moveLeft = false;
	bool _moveRight = false;
	bool _moveUp = false;
	bool _moveDown = false;
	bool _shiftKeyPressed = false;
	float _rotX = 0.0f;
	float _rotY = 0.0f;
	bool _showGBuffer = false;
	bool _moveLights = true;
	float _lightRadiusScale = 1.0f;
	int _curSwapInterval = 1;
	bool _vsync = true;
	bool _settingsDlgVisible = true;
	bool _demosDlgVisible = false;
	bool _newDemoDlgVisible = false;
	bool _deleteDemoDlgVisible = false;
	bool _bmarkResultsDlgVisible = false;
	bool _showLightSources = true;
	bool _showTranspSurfaces = true;

	math3d::vec3f _newLightColor = math3d::vec3f(1.0f, 1.0f, 1.0f);
	float _newLightFalloffExponent = 1.0f;
	float _newLightMinRandomFalloffExp = LightMinFalloffExp;
	float _newLightMaxRandomFalloffExp = LightMaxFalloffExp;
	float _newLightRadius = 1.0f;
	float _newLightMinRandomRadius = 1.0f;
	float _newLightMaxRandomRadius = 2.0f;
	bool _newLightRandomParams = true;
	float _lightSourceSpeed = 70.0f;
	int _randomLightsToCreate = 128;

	float _demoRecordingTimer = 0.0f;
	bool _demoSampleLights = false;
	bool _loopDemoPlayback = false;
	bool _demoPlaybackCanceled = false;
	BenchmarkData _benchmarkData;
};

#endif // _DEFERRED_RENDERER_H_
