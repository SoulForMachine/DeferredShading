#ifndef _APPLICATION_LINUX_H_
#define _APPLICATION_LINUX_H_

#include <string>
#include <X11/Xlib.h>
#include <GLSlayer/FixXlibMacros.h>


class IRenderer;
enum class Key;


class Application
{
public:
	Application(const char* name, int wndWidth, int wndHeight);
	~Application();
	int Run(IRenderer* renderer);

private:
	void OnKeyPress(XKeyEvent* event, bool repeat);
	void OnKeyRelease(XKeyEvent* event);
	void OnMotionNotify(XMotionEvent* event);
	void OnButtonPress(XButtonEvent* event);
	void OnButtonRelease(XButtonEvent* event);
	void ProcessEvent(XEvent* event);
	Key MapKey(KeySym keysym);

	Display* _display;
	Atom _wmDeleteMessage;
	int _wndWidth;
	int _wndHeight;
	std::string _appName;
	int _prevMouseX, _prevMouseY;
	bool _lbtnDown, _rbtnDown, _mbtnDown;
	IRenderer* _renderer;
	bool _quit;
};


#endif // _APPLICATION_LINUX_H_

