
#include "GLVertexFormat.h"
#include <algorithm>
#include "GLContextInfo.h"

namespace gls::internals
{

GLVertexFormat::GLVertexFormat()
{
	_descriptors = 0;
	_count = 0;
}

GLVertexFormat::~GLVertexFormat()
{
	delete[] _descriptors;
	_count = 0;
}

bool GLVertexFormat::Create(const GLContextInfo& ctxInfo, const VertexAttribDesc* descriptors, int count)
{
	if (count <= 0 || !descriptors)
		return false;

	// Check if descriptor data is valid and copy them into an internal array.
	_descriptors = new VertexAttribDesc[count];
	_count = count;

	for (int i = 0; i < count; ++i)
	{
		auto& desc = descriptors[i];
		if (desc.stream < 0 || desc.stream >= ctxInfo.maxVertexAttribBindings ||
			static_cast<int>(desc.attribute) >= ctxInfo.maxVertexAttribs ||
			desc.numComponents < 1 || desc.numComponents > 4 ||
			desc.type == DataType::None ||
			(ctxInfo.featuresGL.ARB_vertex_attrib_binding && static_cast<int>(desc.offset) > ctxInfo.maxVertexAttribRelativeOffset))
		{
			delete[] _descriptors;
			_descriptors = nullptr;
			_count = 0;
			return false;
		}

		_descriptors[i] = descriptors[i];
	}

	// Sort descriptors by attribute index and check if any of them appear more than once.
	std::sort(_descriptors, _descriptors + count, [](const auto& d1, const auto& d2) { return d1.attribute < d2.attribute; });

	for (int i = 1; i < count; ++i)
	{
		if (descriptors[i].attribute == descriptors[i - 1].attribute)
		{
			delete[] _descriptors;
			_descriptors = nullptr;
			_count = 0;
			return false;
		}
	}
	
	return true;
}

} // namespace gls::internals
