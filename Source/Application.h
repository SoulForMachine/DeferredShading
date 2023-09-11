#ifndef _APPLICATION_H_
#define _APPLICATION_H_


#if defined(_WIN32)
	#include "Application_Windows.h"
#elif defined(__linux__)
	#include "Application_Linux.h"
#endif


#endif // _APPLICATION_H_
