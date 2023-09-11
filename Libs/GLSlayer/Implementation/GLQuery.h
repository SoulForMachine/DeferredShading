
#ifndef _GL_QUERY_H_
#define _GL_QUERY_H_

#include "GLSlayer/Query.h"
#include "GLCommon.h"

namespace gls::internals
{

class GLQuery : public IQuery
{
public:
	GLQuery() = default;
	GLQuery(const GLQuery&) = delete;
	GLQuery& operator = (const GLQuery&) = delete;

	void Create();
	void Destroy();

	virtual void BeginQuery(QueryType type) override;
	virtual void BeginQueryIndexed(QueryType type, uint index) override;
	virtual void EndQuery() override;
	virtual void QueryCounter(QueryType type) override;
	virtual bool ResultAvailable() override;
	virtual uint GetResultUI() override;
	virtual uint GetResultNoWaitUI() override;
	virtual uint64 GetResultUI64() override;
	virtual uint64 GetResultNoWaitUI64() override;
	virtual QueryType GetQueryType() override;

	GLenum GetTarget() const { return _target; }
	GLuint GetID() const { return _id; }

private:
	QueryType _type;
	GLenum _target;
	GLuint _id;
};

} // namespace gls::internals

#endif // _GL_QUERY_H_
