
#include "h_glext.h"

// Define empty EXTPTR macro to insert definitions of extension function pointers
//=====================================================================
#undef  EXTPTR
#define EXTPTR
#if defined(_WIN32)
	#include "glext_windows_ptrs.h"
	#include "wglext_ptrs.h"
#elif defined(__linux__)
	#include "glext_linux_ptrs.h"
	#include "glxext_ptrs.h"
#endif
//=====================================================================
