
#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include <cstdio>
#include <cstdarg>
#include "GLSlayer/RenderContext.h"


class Console : public gls::IDebugLogger
{
public:
	void DebugMessage(gls::DebugMessageSource source, gls::DebugMessageType type, gls::uint id, gls::DebugMessageSeverity severity, const char* message)
	{
		//if(type != gls::DEBUG_TYPE_OTHER)
			printf("%s\n", message);
	}

	void PrintLn(const char* str, ...)
	{
		va_list args;
		va_start(args, str);

		vprintf(str, args);
		printf("\n");

		va_end(args);
	}

	void Print(const char* str, ...)
	{
		va_list args;
		va_start(args, str);

		vprintf(str, args);

		va_end(args);
	}
};

#endif // _CONSOLE_H_
