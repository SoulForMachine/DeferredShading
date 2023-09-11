
#ifndef _IRENDERER_H_
#define _IRENDERER_H_

namespace gls
{
struct CreateContextInfo;
struct FramebufferFormat;
}

enum class Key
{
	None,

	Tab,
	LeftArrow,
	RightArrow,
	UpArrow,
	DownArrow,
	PageUp,
	PageDown,
	Home,
	End,
	Insert,
	Delete,
	Backspace,
	Space,
	Enter,
	Escape,
	Shift,
	F1,
	F2,
	A,
	B,
	C,
	D,
	E,
	G,
	L,
	M,
	Q,
	R,
	S,
	T,
	V,
	W,
	X,
	Y,
	Z,
};

class IRenderer
{
public:
	virtual ~IRenderer() = default;

	virtual bool Init(gls::CreateContextInfo& info) = 0;
	virtual void Deinit() = 0;
	virtual gls::FramebufferFormat GetFramebufferFormat() = 0;
	virtual void Update(float frameTime) = 0;
	virtual void Render() = 0;
	virtual void OnResize(int width, int height) = 0;
	virtual void OnKeyDown(Key key, bool repeat){ }
	virtual void OnKeyUp(Key key)				{ }
	virtual void OnChar(unsigned short ch)		{ }
	virtual void OnMouseMove(int x, int y)		{ }
	virtual void OnMouseDrag(int dx, int dy)	{ }
	virtual void OnLBtnDown(int x, int y)		{ }
	virtual void OnRBtnDown(int x, int y)		{ }
	virtual void OnMBtnDown(int x, int y)		{ }
	virtual void OnLBtnUp(int x, int y)			{ }
	virtual void OnRBtnUp(int x, int y)			{ }
	virtual void OnMBtnUp(int x, int y)			{ }
	virtual void OnLostKeyboardFocus()			{ }
};

#endif // _IRENDERER_H_
