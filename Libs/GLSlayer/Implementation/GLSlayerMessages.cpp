
#include "GLSlayerMessages.h"

namespace gls::internals
{

const char* messageTable[] =
{
	"Version number must be at least 330.",
	"Failed to create rendering context (version %u): %s.",
	"The driver does not have an entry for function %s.",
	"Extension %s is not supported.",

#if defined (_WIN32)
	"Failed to create temporary rendering context: %s.",
#endif
};

const char* GetMessageString(ErrorMessageId message_id)
{
	return messageTable[static_cast<int>(message_id)];
}

} // namespace gls::internals
