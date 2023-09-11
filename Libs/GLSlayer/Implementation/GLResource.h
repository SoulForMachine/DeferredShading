
#ifndef _GL_RESOURCE_H_
#define _GL_RESOURCE_H_

#include "GLCommon.h"


#define IMPLEMENT_IRESOURCE \
	virtual ResourceType GetType() const override { return GLResource::GetType(); }


namespace gls::internals
{

class GLResource
{
public:
	static const int typeID = TYPE_ID_RESOURCE;

	GLResource()
		{ _id = 0; }
	virtual ~GLResource() = 0;

	GLResource(const GLResource&) = delete;
	GLResource& operator = (const GLResource&) = delete;

	void* DynamicCast(int type_id)			{ return (type_id == TYPE_ID_RESOURCE) ? this : 0; }
	ResourceType GetType() const			{ return _resType; }
	GLuint GetID() const					{ return _id; }
	GLenum GetTarget() const				{ return _target; }

protected:
	GLuint _id;
	GLenum _target;
	ResourceType _resType;
};

inline GLResource::~GLResource()
{
}


} // namespace gls::internals

#endif // _GL_RESOURCE_H_
