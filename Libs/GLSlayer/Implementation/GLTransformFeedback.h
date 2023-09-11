
#ifndef _GL_TRANSFORM_FEEDBACK_H_
#define _GL_TRANSFORM_FEEDBACK_H_

#include "GLSlayer/TransformFeedback.h"
#include "GLResource.h"


namespace gls
{
	class IBuffer;
}

namespace gls::internals
{

class GLTransformFeedback : public ITransformFeedback, public GLResource
{
public:
	IMPLEMENT_IRESOURCE

	static const int typeID = TYPE_ID_TRANSFORM_FEEDBACK;

	GLTransformFeedback() = default;

	GLTransformFeedback(const GLTransformFeedback&) = delete;
	GLTransformFeedback& operator = (const GLTransformFeedback&) = delete;

	bool Create(GLState* gl_state);
	void Destroy();

	virtual void* DynamicCast(int type_id) override { return (type_id == TYPE_ID_TRANSFORM_FEEDBACK) ? this : GLResource::DynamicCast(type_id); }
	virtual void BindBuffer(uint index, IBuffer* buffer) override;
	virtual void BindBuffer(uint index, IBuffer* buffer, intptr offset, sizeiptr size) override;

private:
	GLState* _glState;
};

} // namespace gls::internals

#endif // _GL_TRANSFORM_FEEDBACK_H_
