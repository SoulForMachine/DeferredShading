
#include <cassert>
#include "GLQuery.h"

namespace gls::internals
{

void GLQuery::Create()
{
	glGenQueries(1, &_id);
	_type = QueryType::Undefined;
	_target = 0;
}

void GLQuery::Destroy()
{
	if (_id)
	{
		glDeleteQueries(1, &_id);
		_id = 0;
	}
}

void GLQuery::BeginQuery(QueryType type)
{
	_type = type;
	_target = GetGLEnum(type);
	glBeginQuery(_target, _id);
}

void GLQuery::BeginQueryIndexed(QueryType type, uint index)
{
	_type = type;
	_target = GetGLEnum(type);
	glBeginQueryIndexed(_target, index, _id);
}

void GLQuery::EndQuery()
{
	glEndQuery(_target);
}

void GLQuery::QueryCounter(QueryType type)
{
	_type = type;
	_target = GetGLEnum(type);
	glQueryCounter(_id, _target);
}

bool GLQuery::ResultAvailable()
{
	GLint available;
	glGetQueryObjectiv(_id, GL_QUERY_RESULT_AVAILABLE, &available);
	return (available != 0);
}

uint GLQuery::GetResultUI()
{
	GLuint result;
	glGetQueryObjectuiv(_id, GL_QUERY_RESULT, &result);
	return (uint)result;
}

uint GLQuery::GetResultNoWaitUI()
{
	GLuint result;
	glGetQueryObjectuiv(_id, GL_QUERY_RESULT_NO_WAIT, &result);
	return (uint)result;
}

uint64 GLQuery::GetResultUI64()
{
	GLuint64 result;
	glGetQueryObjectui64v(_id, GL_QUERY_RESULT, &result);
	return (uint64)result;
}

uint64 GLQuery::GetResultNoWaitUI64()
{
	GLuint64 result;
	glGetQueryObjectui64v(_id, GL_QUERY_RESULT_NO_WAIT, &result);
	return (uint64)result;
}

QueryType GLQuery::GetQueryType()
{
	return _type;
}

} // namespace gls::internals
