
#ifndef _GL_SLAYER_MESSAGES_H_
#define _GL_SLAYER_MESSAGES_H_

namespace gls::internals
{

enum class ErrorMessageId : int
{
	UnsupportedVersion,
	CreateContext,
	ExtensionMissingEntries,
	UnsupportedExtension,

#if defined (_WIN32)
	CreateTemporaryContext,
#endif

	Count
};


const char* GetMessageString(ErrorMessageId message_id);

} // namespace gls::internals

#endif // _GL_SLAYER_MESSAGES_H_
