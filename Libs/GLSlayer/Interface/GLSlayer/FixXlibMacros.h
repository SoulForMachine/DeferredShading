#ifndef _GLSLAYER_FIX_XLIB_MACROS_H_
#define _GLSLAYER_FIX_XLIB_MACROS_H_

// Include this header after including X11/Xlib.h and X11/Xutil.h.

// These xlib macros clash with names of GLSlayer enum constants, so we need to get rid of them.
#ifdef Bool
	using _dummy_xlib_Bool = Bool;
	#undef Bool
	using Bool = _dummy_xlib_Bool;
#endif
#ifdef None
	constexpr auto _dummy_xlib_None = None;
	#undef None
	constexpr auto None = _dummy_xlib_None;
#endif
#ifdef Always
	constexpr auto _dummy_xlib_Always = Always;
	#undef Always
	constexpr auto Always = _dummy_xlib_Always;
#endif

#endif // _GLSLAYER_FIX_XLIB_MACROS_H_
