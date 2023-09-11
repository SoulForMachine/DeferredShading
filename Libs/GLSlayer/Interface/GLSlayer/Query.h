
#ifndef _GLSLAYER_QUERY_H_
#define _GLSLAYER_QUERY_H_

#include "Resource.h"

namespace gls
{

	class IQuery
	{
	public:
		virtual ~IQuery() = default;

		virtual void BeginQuery(QueryType type) = 0;
		virtual void BeginQueryIndexed(QueryType type, uint index) = 0;
		virtual void EndQuery() = 0;
		virtual void QueryCounter(QueryType type) = 0;
		virtual bool ResultAvailable() = 0;
		virtual uint GetResultUI() = 0;
		virtual uint GetResultNoWaitUI() = 0;
		virtual uint64 GetResultUI64() = 0;
		virtual uint64 GetResultNoWaitUI64() = 0;
		virtual QueryType GetQueryType() = 0;
	};

}


#endif // _GLSLAYER_QUERY_H_
