
#include <cassert>
#include "GLTransformFeedback.h"
#include "GLBuffer.h"


#define STATE_MACHINE_HACK \
	if(_id != _glState->transformFeedback) \
	{ \
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, _id); \
		_glState->transformFeedback = _id; \
	}

namespace gls::internals
{

bool GLTransformFeedback::Create(GLState* gl_state)
{
	_glState = gl_state;

	glGenTransformFeedbacks(1, &_id);

	if (!_id)
		return false;

	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, _id);

	if (glGetError() != GL_NO_ERROR)
		return false;

	_target = GL_TRANSFORM_FEEDBACK;
	_resType = ResourceType::TransformFeedback;

	return true;
}

void GLTransformFeedback::Destroy()
{
	if (_id)
	{
		if (_id == _glState->transformFeedback)
		{
			glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
			_glState->transformFeedback = 0;
		}
		glDeleteTransformFeedbacks(1, &_id);
		_id = 0;
	}
}

void GLTransformFeedback::BindBuffer(uint index, IBuffer* buffer)
{
	STATE_MACHINE_HACK
		GLuint buf_id = dyn_cast_ptr<GLBuffer*>(buffer)->GetID();
	_glState->transfFeedbackBuf = buf_id;
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, index, buf_id);
}

void GLTransformFeedback::BindBuffer(uint index, IBuffer* buffer, intptr offset, sizeiptr size)
{
	STATE_MACHINE_HACK
		GLuint buf_id = dyn_cast_ptr<GLBuffer*>(buffer)->GetID();
	_glState->transfFeedbackBuf = buf_id;
	glBindBufferRange(GL_TRANSFORM_FEEDBACK_BUFFER, index, buf_id, offset, size);
}

} // namespace gls::internals
