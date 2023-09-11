
#include "Application_Windows.h"
#include <cassert>
#include <chrono>
#include <memory>
#include <windowsx.h>
#include "GLSlayer/RenderContextInit.h"
#include "IRenderer.h"

#pragma comment(lib, "GLSlayer.lib")



Application::Application(const char* name, int wndWidth, int wndHeight)
	: _appName(name)
	, _wndWidth(wndWidth)
	, _wndHeight(wndHeight)
	, _hinstance(NULL)
	, _hwnd(NULL)
	, _prevMouseX(0)
	, _prevMouseY(0)
	, _lbtnDown(false), _rbtnDown(false), _mbtnDown(false)
	, _renderer(nullptr)
{

}

Application::~Application()
{
	delete _renderer;
}

int Application::Run(IRenderer* renderer)
{
	_hinstance = (HINSTANCE)GetModuleHandle(0);
	_renderer = renderer;

	if(!MyCreateWindow())
		return 1;

	auto prevTime = std::chrono::high_resolution_clock::now();

	MSG msg = { 0 };
	bool running = true;
	float frameTime = 1.0f / 60.0f;

	while (running)
	{
		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				running = false;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		_renderer->Update(frameTime);
		_renderer->Render();

		auto currTime = std::chrono::high_resolution_clock::now();
		frameTime = std::chrono::duration<float, std::chrono::seconds::period>(currTime - prevTime).count();
		prevTime = currTime;
	}

	return (int)msg.wParam;
}

void Application::OnSize(int width, int height)
{
	if (width != _wndWidth || height != _wndHeight)
	{
		_renderer->OnResize(width, height);
		_wndWidth = width;
		_wndHeight = height;
	}
}

void Application::OnMouseMove(int x, int y)
{
	_renderer->OnMouseMove(x, y);

	if(_lbtnDown)
	{
		_renderer->OnMouseDrag(x - _prevMouseX, y - _prevMouseY);
		_prevMouseX = x;
		_prevMouseY = y;
	}
}

void Application::OnLBtnDown(int x, int y)
{
	_lbtnDown = true;
	_prevMouseX = x;
	_prevMouseY = y;
	SetCapture(_hwnd);

	_renderer->OnLBtnDown(x, y);
}

void Application::OnRBtnDown(int x, int y)
{
	_rbtnDown = true;
	_prevMouseX = x;
	_prevMouseY = y;
	SetCapture(_hwnd);

	_renderer->OnRBtnDown(x, y);
}

void Application::OnMBtnDown(int x, int y)
{
	_mbtnDown = true;
	_prevMouseX = x;
	_prevMouseY = y;
	SetCapture(_hwnd);

	_renderer->OnMBtnDown(x, y);
}

void Application::OnLBtnUp(int x, int y)
{
	_lbtnDown = false;
	ReleaseCapture();

	_renderer->OnLBtnUp(x, y);
}

void Application::OnRBtnUp(int x, int y)
{
	_rbtnDown = false;
	ReleaseCapture();

	_renderer->OnRBtnUp(x, y);
}

void Application::OnMBtnUp(int x, int y)
{
	_mbtnDown = false;
	ReleaseCapture();

	_renderer->OnMBtnUp(x, y);
}

void Application::OnKeyDown(int key, bool repeat)
{
	_renderer->OnKeyDown(MapKey(key), repeat);
}

void Application::OnKeyUp(int key)
{
	_renderer->OnKeyUp(MapKey(key));
}

void Application::OnChar(unsigned short ch)
{
	_renderer->OnChar(ch);
}

bool Application::MyCreateWindow()
{
	TCHAR wndClass[] = TEXT("Deferred shading wnd class");

	WNDCLASS wc = { };
	wc.hInstance = _hinstance;
	wc.style = CS_OWNDC;
	wc.lpszClassName = wndClass;
	wc.lpfnWndProc = WndProc;
	wc.hCursor = LoadCursor(0, IDC_ARROW);

	if(RegisterClass(&wc))
	{
		UINT dpi = GetDpiForSystem();
		float scalingFactor = dpi / 96.0f;
		RECT rect = {
			0, 0,
			static_cast<LONG>(_wndWidth * scalingFactor),
			static_cast<LONG>(_wndHeight * scalingFactor)
		};
		DWORD wndStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
		AdjustWindowRectExForDpi(&rect, wndStyle, FALSE, 0, dpi);

		int nameLen = MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, _appName.c_str(), -1, nullptr, 0);
		std::wstring wAppName;
		if (nameLen > 0)
		{
			wAppName.resize(nameLen);
			MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, _appName.c_str(), -1, wAppName.data(), nameLen);
		}

		_hwnd = CreateWindowEx(
			0,
			wndClass,
			wAppName.c_str(),
			wndStyle,
			50, 50, rect.right - rect.left, rect.bottom - rect.top,
			0, 0, _hinstance, 0);

		if (!_hwnd)
			return false;

		gls::FramebufferFormat format = _renderer->GetFramebufferFormat();
		gls::CreateContextInfo info;
		info.version = 440;
		info.debugContext = false;
		info.format = &format;
		info.instanceHandle = _hinstance;
		info.windowHandle = _hwnd;
		info.logger = nullptr;
		info.shareContext = nullptr;

		if(!_renderer->Init(info))
		{
			getc(stdin);
			return false;
		}

		// Initial resize; we need to query for the client rectangle because it may be actually
		// smaller if part of it is outside of screen.
		RECT clrect;
		GetClientRect(_hwnd, &clrect);
		_renderer->OnResize(clrect.right - clrect.left, clrect.bottom - clrect.top);

		// So that the WndProc can access Application pointer.
		SetWindowLongPtr(_hwnd, GWLP_USERDATA, (LONG_PTR)this);

		return _hwnd != 0;
	}
	else
	{
		return false;
	}
}

bool Application::WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_CREATE:
		return true;

	case WM_DESTROY:
		_renderer->Deinit();
		PostQuitMessage(0);
		return true;

	case WM_SIZE:
		OnSize(LOWORD(lParam), HIWORD(lParam));
		return true;

	case WM_KEYDOWN:
		OnKeyDown((int)wParam, (lParam & (1 << 30)) != 0);
		return true;

	case WM_KEYUP:
		OnKeyUp((int)wParam);
		return true;

	case WM_CHAR:
		if (wParam > 0 && wParam < 0x10000)
			OnChar((unsigned short)wParam);
		return true;

	case WM_MOUSEMOVE:
		OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return true;

	case WM_LBUTTONDOWN:
		OnLBtnDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return true;

	case WM_RBUTTONDOWN:
		OnRBtnDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return true;

	case WM_MBUTTONDOWN:
		OnMBtnDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return true;

	case WM_LBUTTONUP:
		OnLBtnUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return true;

	case WM_RBUTTONUP:
		OnRBtnUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return true;

	case WM_MBUTTONUP:
		OnMBtnUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return true;

	case WM_KILLFOCUS:
		_renderer->OnLostKeyboardFocus();
		return true;
	}

	return false;
}

Key Application::MapKey(int key)
{
	switch (key)
	{
	case VK_TAB: return Key::Tab;
	case VK_LEFT: return Key::LeftArrow;
	case VK_RIGHT: return Key::RightArrow;
	case VK_UP: return Key::UpArrow;
	case VK_DOWN: return Key::DownArrow;
	case VK_PRIOR: return Key::PageUp;
	case VK_NEXT: return Key::PageDown;
	case VK_HOME: return Key::Home;
	case VK_END: return Key::End;
	case VK_INSERT: return Key::Insert;
	case VK_DELETE: return Key::Delete;
	case VK_BACK: return Key::Backspace;
	case VK_SPACE: return Key::Space;
	case VK_RETURN: return Key::Enter;
	case VK_ESCAPE: return Key::Escape;
	case VK_SHIFT: return Key::Shift;
	case VK_F1: return Key::F1;
	case VK_F2: return Key::F2;
	case 'A': return Key::A;
	case 'B': return Key::B;
	case 'C': return Key::C;
	case 'D': return Key::D;
	case 'E': return Key::E;
	case 'G': return Key::G;
	case 'L': return Key::L;
	case 'M': return Key::M;
	case 'Q': return Key::Q;
	case 'R': return Key::R;
	case 'S': return Key::S;
	case 'T': return Key::T;
	case 'V': return Key::V;
	case 'W': return Key::W;
	case 'X': return Key::X;
	case 'Y': return Key::Y;
	case 'Z': return Key::Z;
	default: return Key::None;
	}
}

LRESULT	WINAPI	Application::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Application* framework = (Application*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	if(framework)
	{
		if(framework->WindowProc(hwnd, message, wParam, lParam))
			return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}
