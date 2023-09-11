
#ifndef _APPLICATION_WINDOWS_H_
#define _APPLICATION_WINDOWS_H_

#include <string>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#ifdef MemoryBarrier
	#undef MemoryBarrier
#endif


class IRenderer;
enum class Key;


class Application
{
public:
	Application(const char* name, int wndWidth, int wndHeight);
	~Application();
	int Run(IRenderer* renderer);

private:
	void OnSize(int width, int height);
	void OnMouseMove(int x, int y);
	void OnLBtnDown(int x, int y);
	void OnRBtnDown(int x, int y);
	void OnMBtnDown(int x, int y);
	void OnLBtnUp(int x, int y);
	void OnRBtnUp(int x, int y);
	void OnMBtnUp(int x, int y);
	void OnKeyDown(int key, bool repeat);
	void OnKeyUp(int key);
	void OnChar(unsigned short ch);
	bool MyCreateWindow();
	bool WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	Key MapKey(int key);

	static LRESULT	WINAPI	WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	HINSTANCE _hinstance;
	HWND _hwnd;
	int _wndWidth;
	int _wndHeight;
	std::string _appName;
	int _prevMouseX, _prevMouseY;
	bool _lbtnDown, _rbtnDown, _mbtnDown;
	IRenderer* _renderer;
};


#endif // _APPLICATION_WINDOWS_H_
