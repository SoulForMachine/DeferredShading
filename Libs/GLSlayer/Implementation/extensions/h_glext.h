
#ifndef _H_GLEXT_H_
#define _H_GLEXT_H_

#if defined(_WIN32)
	#define WIN32_LEAN_AND_MEAN
	#define NOMINMAX
	#include <Windows.h>
	#ifdef MemoryBarrier
		#undef MemoryBarrier
	#endif
#endif

#include <cassert>
#include "glcorearb.h"
#include "glext.h"

#if defined(_WIN32)
	#include "wglext.h"
	#pragma comment(lib, "opengl32.lib")
#elif defined(__linux__)
	#include <GL/glx.h>
	#include "GLSlayer/FixXlibMacros.h"
#endif


// include all headers with declarations of extension function pointers
//=====================================================================

#ifdef __cplusplus
	#define EXTPTR extern "C"
#else
	#define EXTPTR extern
#endif

struct GLContextInfo;

#if defined(_DEBUG)
	// Comment this out to disable checking for error after each GL call.
	#define DEBUG_GL_CHECK_FOR_ERROR
#endif

#if defined(_WIN32)
	#include "glext_windows_ptrs.h"
	#include "glext_windows_funcs.h"
	#include "wglext_ptrs.h"
	#include "wglext_funcs.h"
#elif defined(__linux__)
	#include "glext_linux_ptrs.h"
	#include "glext_linux_funcs.h"
	#include "glxext_ptrs.h"
	#include "glxext_funcs.h"
#endif

//=====================================================================

#endif // _H_GLEXT_H_
