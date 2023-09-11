
#ifndef _GL_VERTEX_FORMAT_H_
#define _GL_VERTEX_FORMAT_H_

#include "GLSlayer/VertexFormat.h"
#include "GLCommon.h"


namespace gls::internals
{

struct GLContextInfo;

class GLVertexFormat : public IVertexFormat
{
public:
	GLVertexFormat();
	~GLVertexFormat();

	GLVertexFormat(const GLVertexFormat&) = delete;
	GLVertexFormat& operator = (const GLVertexFormat&) = delete;

	bool Create(const GLContextInfo& ctxInfo, const VertexAttribDesc* descriptors, int count);

	const VertexAttribDesc* GetDescriptors() const { return _descriptors; }
	int GetDescriptorCount() const { return _count; }

private:
	VertexAttribDesc* _descriptors;
	int _count;
};

} // namespace gls::internals

#endif // _GL_VERTEX_FORMAT_H_
