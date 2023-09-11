
#include <time.h>
#include <cmath>
#include <X11/Xutil.h>
#include "GLSlayer/RenderContextInit.h"
#include "Application_Linux.h"
#include "IRenderer.h"

using namespace std;

Application::Application(const char* name, int wndWidth, int wndHeight)
	: _display(nullptr)
	, _wndWidth(wndWidth)
	, _wndHeight(wndHeight)
	, _appName(name)
	, _prevMouseX(0)
	, _prevMouseY(0)
	, _lbtnDown(false)
	, _rbtnDown(false)
	, _mbtnDown(false)
	, _renderer(nullptr)
	, _quit(false)
{

}

Application::~Application()
{
	delete _renderer;
}

int Application::Run(IRenderer* renderer)
{
	_display = XOpenDisplay(nullptr);

	if (!_display)
		return 1;

	_renderer = renderer;

	gls::FramebufferFormat fbufFormat = _renderer->GetFramebufferFormat();

	XVisualInfo visualInfo;
	if (!gls::GetXVisualInfo(_display, fbufFormat, visualInfo))
		return 1;
	Window rootWindow = XRootWindow(_display, visualInfo.screen);

	XSetWindowAttributes swa;
	swa.colormap = XCreateColormap(_display, rootWindow, visualInfo.visual, AllocNone);
	swa.background_pixmap = None;
	swa.border_pixel      = 0;
	swa.event_mask        = StructureNotifyMask | KeyPressMask | KeyReleaseMask | PointerMotionMask | ButtonPressMask | ButtonReleaseMask;

	Window window = XCreateWindow(_display, rootWindow, 0, 0, _wndWidth, _wndHeight, 0, visualInfo.depth,
								  InputOutput, visualInfo.visual, CWBorderPixel|CWColormap|CWEventMask, &swa);

	if (!window)
		return 1;

	XClassHint* classHint = XAllocClassHint();
	classHint->res_class = classHint->res_name = _appName.data();
	XSetClassHint(_display, window, classHint);
	XFree(classHint);

	_wmDeleteMessage = XInternAtom(_display, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(_display, window, &_wmDeleteMessage, 1);
	XStoreName(_display, window, _appName.c_str());
	XMapWindow(_display, window);

	gls::CreateContextInfo info;
	info.version = 440;
	info.debugContext = false;
	info.format = &fbufFormat;
	info.display = _display;
	info.window = window;
	info.logger = nullptr;
	info.shareContext = nullptr;

	if (!_renderer->Init(info))
	{
		XDestroyWindow(_display, window);
		XFreeColormap(_display, swa.colormap);
		XCloseDisplay(_display);
		return 1;
	}

	_renderer->OnResize(_wndWidth, _wndHeight);

	timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	float prev_time = static_cast<float>(t.tv_sec) + t.tv_nsec / 1.0e9f;
	XEvent event;

	while (!_quit)
	{
		while (XEventsQueued(_display, QueuedAfterFlush) > 0)
		{
			XNextEvent(_display, &event);
			ProcessEvent(&event);
		}

		clock_gettime(CLOCK_MONOTONIC, &t);
		float cur_time = static_cast<float>(t.tv_sec) + t.tv_nsec / 1.0e9f;
		float frame_time = cur_time - prev_time;
		prev_time = cur_time;

		_renderer->Update(frame_time);
		_renderer->Render();
	}

	_renderer->Deinit();

	XDestroyWindow(_display, window);
	XFreeColormap(_display, swa.colormap);
	XCloseDisplay(_display);

	return 0;
}

void Application::OnKeyPress(XKeyEvent* event, bool repeat)
{
	char ch[2] = {};
	KeySym keysym;
	XLookupString(event, ch, 2, &keysym, nullptr);
	if ((keysym >= 0x20 && keysym <= 0x7E) ||
		(keysym >= 0xA0 && keysym <= 0xFF))
	{
		_renderer->OnChar(static_cast<unsigned short>(ch[0]));
	}
	_renderer->OnKeyDown(MapKey(keysym), repeat);
}

void Application::OnKeyRelease(XKeyEvent* event)
{
	KeySym keysym = XLookupKeysym(event, 0);
	_renderer->OnKeyUp(MapKey(keysym));
}

void Application::OnMotionNotify(XMotionEvent* event)
{
	int x = event->x;
	int y = event->y;
	_renderer->OnMouseMove(x, y);

	if (_lbtnDown)
	{
		_renderer->OnMouseDrag(x - _prevMouseX, y - _prevMouseY);
		_prevMouseX = x;
		_prevMouseY = y;
	}
}

void Application::OnButtonPress(XButtonEvent* event)
{
	int x = event->x;
	int y = event->y;

	if (event->button == Button1)
	{
		_lbtnDown = true;
		_prevMouseX = x;
		_prevMouseY = y;
		_renderer->OnLBtnDown(x, y);
	}
	else if (event->button == Button2)
	{
		_rbtnDown = true;
		_prevMouseX = x;
		_prevMouseY = y;
		_renderer->OnRBtnDown(x, y);
	}
	else if (event->button == Button3)
	{
		_mbtnDown = true;
		_prevMouseX = x;
		_prevMouseY = y;
		_renderer->OnMBtnDown(x, y);
	}
}

void Application::OnButtonRelease(XButtonEvent* event)
{
	int x = event->x;
	int y = event->y;

	if (event->button == Button1)
	{
		_lbtnDown = false;
		_renderer->OnLBtnUp(x, y);
	}
	else if (event->button == Button2)
	{
		_rbtnDown = false;
		_renderer->OnRBtnUp(x, y);
	}
	else if (event->button == Button3)
	{
		_mbtnDown = false;
		_renderer->OnMBtnUp(x, y);
	}
}

void Application::ProcessEvent(XEvent* event)
{
	switch (event->type)
	{
	case MotionNotify:
		OnMotionNotify(&event->xmotion);
		break;
	case ButtonPress:
		OnButtonPress(&event->xbutton);
		break;
	case ButtonRelease:
		OnButtonRelease(&event->xbutton);
		break;
	case KeyPress:
		OnKeyPress(&event->xkey, false);
		break;
	case KeyRelease:
		if (XEventsQueued(_display, QueuedAfterReading))
		{
			XEvent nextEvent;
			XPeekEvent(_display, &nextEvent);

			if (nextEvent.type == KeyPress && nextEvent.xkey.time == event->xkey.time &&
				nextEvent.xkey.keycode == event->xkey.keycode)
			{
				// The key press was repeated.
				XNextEvent(_display, &nextEvent);
				OnKeyPress(&nextEvent.xkey, true);
				break;
			}
		}

		OnKeyRelease(&event->xkey);
		break;
	case ConfigureNotify:
		if(	event->xconfigure.width != _wndWidth ||
			event->xconfigure.height != _wndHeight)
		{
			_wndWidth = event->xconfigure.width;
			_wndHeight = event->xconfigure.height;
			_renderer->OnResize(_wndWidth, _wndHeight);
		}
		break;
	case CreateNotify:
		break;
	case DestroyNotify:
		break;
	case ClientMessage:
		if((Atom)event->xclient.data.l[0] == _wmDeleteMessage)
			_quit = true;
		break;
	}
}

Key Application::MapKey(KeySym keysym)
{
	switch (keysym)
	{
	case XK_Tab: return Key::Tab;
	case XK_Left: return Key::LeftArrow;
	case XK_Right: return Key::RightArrow;
	case XK_Up: return Key::UpArrow;
	case XK_Down: return Key::DownArrow;
	case XK_Prior: return Key::PageUp;
	case XK_Next: return Key::PageDown;
	case XK_Home: return Key::Home;
	case XK_End: return Key::End;
	case XK_Insert: return Key::Insert;
	case XK_Delete: return Key::Delete;
	case XK_BackSpace: return Key::Backspace;
	case XK_space: return Key::Space;
	case XK_Return: return Key::Enter;
	case XK_Escape: return Key::Escape;
	case XK_KP_Enter: return Key::Enter;
	case XK_Shift_L: case XK_Shift_R: return Key::Shift;
	case XK_F1: return Key::F1;
	case XK_F2: return Key::F2;
	case XK_a: case XK_A: return Key::A;
	case XK_b: case XK_B: return Key::B;
	case XK_c: case XK_C: return Key::C;
	case XK_d: case XK_D: return Key::D;
	case XK_e: case XK_E: return Key::E;
	case XK_g: case XK_G: return Key::G;
	case XK_l: case XK_L: return Key::L;
	case XK_m: case XK_M: return Key::M;
	case XK_q: case XK_Q: return Key::Q;
	case XK_r: case XK_R: return Key::R;
	case XK_s: case XK_S: return Key::S;
	case XK_t: case XK_T: return Key::T;
	case XK_v: case XK_V: return Key::V;
	case XK_w: case XK_W: return Key::W;
	case XK_x: case XK_X: return Key::X;
	case XK_y: case XK_Y: return Key::Y;
	case XK_z: case XK_Z: return Key::Z;
	default: return Key::None;
	}
}
