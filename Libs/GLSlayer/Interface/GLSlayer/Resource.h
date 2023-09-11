
#ifndef _GLSLAYER_RESOURCE_H_
#define _GLSLAYER_RESOURCE_H_

#include "Common.h"



namespace gls
{

	class IResource
	{
	public:
		virtual ~IResource() = default;

		virtual void* DynamicCast(int type_id) = 0;
		virtual ResourceType GetType() const = 0;
	};

}

#endif // _GLSLAYER_RESOURCE_H_
