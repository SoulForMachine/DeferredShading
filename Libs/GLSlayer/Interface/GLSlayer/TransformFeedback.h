
#ifndef _GLSLAYER_TRANSFORM_FEEDBACK_H_
#define _GLSLAYER_TRANSFORM_FEEDBACK_H_

#include "Resource.h"


namespace gls
{

	class IBuffer;


	class ITransformFeedback : public IResource
	{
	public:
		virtual void BindBuffer(uint index, IBuffer* buffer) = 0;
		virtual void BindBuffer(uint index, IBuffer* buffer, intptr offset, sizeiptr size) = 0;
	};

}

#endif // _GLSLAYER_TRANSFORM_FEEDBACK_H_
