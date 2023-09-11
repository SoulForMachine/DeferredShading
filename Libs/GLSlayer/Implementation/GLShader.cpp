
#include "GLShader.h"
#include <cassert>
#include <cstring>
#include <algorithm>


namespace gls::internals
{

GLShader::GLShader()
{
	_logInfo = 0;
	_logInfoLength = 0;

	_uniformBlockCount = 0;
	_uniformBlockArraySize = 0;
	_uniformBlockInfos = nullptr;

	_storageBlockCount = 0;
	_storageBlockArraySize = 0;
	_storageBlockInfos = nullptr;
}

GLShader::~GLShader()
{
	delete[] _logInfo;

	for (sizei i = 0; i < _uniformBlockCount; ++i)
	{
		delete[] _uniformBlockInfos[i].name;

		for (sizei u = 0; u < _uniformBlockInfos[i].variableCount; ++u)
			delete[] _uniformBlockInfos[i].variables[u].name;

		delete[] _uniformBlockInfos[i].variables;
	}

	delete[] _uniformBlockInfos;

	for (sizei i = 0; i < _storageBlockCount; ++i)
	{
		delete[] _storageBlockInfos[i].name;

		for (sizei u = 0; u < _storageBlockInfos[i].variableCount; ++u)
			delete[] _storageBlockInfos[i].variables[u].name;

		delete[] _storageBlockInfos[i].variables;
	}

	delete[] _storageBlockInfos;
}

bool GLShader::Create(sizei count, const char** source)
{
	GLint linked = GL_FALSE;
	_id = glCreateShaderProgramv(_target, count, source);

	if (_id)
	{
		glGetProgramiv(_id, GL_LINK_STATUS, &linked);

		RetrieveLog();
	}

	return (glGetError() == GL_NO_ERROR && linked == GL_TRUE);
}

bool GLShader::Create(sizei size, const void* binary, uint format)
{
	GLint linked = GL_FALSE;
	_id = glCreateProgram();
	if (_id)
	{
		glProgramParameteri(_id, GL_PROGRAM_SEPARABLE, GL_TRUE);
		glProgramBinary(_id, format, binary, size);

		glGetProgramiv(_id, GL_LINK_STATUS, &linked);

		RetrieveLog();
	}

	return (glGetError() == GL_NO_ERROR && linked == GL_TRUE);
}

bool GLShader::CreateWithTransformFeedback(sizei count, const char** source, sizei attrib_count, const char** attrib_names)
{
	GLint linked = GL_FALSE;
	GLuint shader = glCreateShader(_target);
	if (shader)
	{
		glShaderSource(shader, count, source, 0);
		glCompileShader(shader);
		GLuint program = glCreateProgram();
		if (program)
		{
			GLint compiled = GL_FALSE;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
			glProgramParameteri(program, GL_PROGRAM_SEPARABLE, GL_TRUE);
			if (compiled)
			{
				glAttachShader(program, shader);

				glTransformFeedbackVaryings(program, attrib_count, attrib_names, GL_INTERLEAVED_ATTRIBS);

				glLinkProgram(program);
				glDetachShader(program, shader);
				glGetProgramiv(_id, GL_LINK_STATUS, &linked);
			}
			RetrieveLog2(program, shader);
		}
		glDeleteShader(shader);
		_id = program;
	}

	return (glGetError() == GL_NO_ERROR && linked == GL_TRUE);
}

bool GLShader::CreateWithTransformFeedback(sizei size, const void* binary, uint format, sizei attrib_count, const char** attrib_names)
{
	GLint linked = GL_FALSE;
	_id = glCreateProgram();
	if (_id)
	{
		glProgramParameteri(_id, GL_PROGRAM_SEPARABLE, GL_TRUE);
		glTransformFeedbackVaryings(_id, attrib_count, attrib_names, GL_INTERLEAVED_ATTRIBS);

		glProgramBinary(_id, format, binary, size);
		glGetProgramiv(_id, GL_LINK_STATUS, &linked);

		RetrieveLog();
	}

	return (glGetError() == GL_NO_ERROR && linked == GL_TRUE);
}

void GLShader::Destroy()
{
	if (_id)
	{
		glDeleteProgram(_id);
		_id = 0;
	}
}


ShaderType GLShader::GetShaderType()
{
	return _shaderType;
}

const char* GLShader::GetInfoLog()
{
	return _logInfo;
}

int GLShader::GetInfoLogLength()
{
	return _logInfoLength;
}

bool GLShader::Validate()
{
	glValidateProgram(_id);

	GLint status;
	glGetProgramiv(_id, GL_VALIDATE_STATUS, &status);

	RetrieveLog();

	return (status == GL_TRUE);
}

uint GLShader::GetSubroutineIndex(const char* name)
{
	uint index = glGetSubroutineIndex(_id, _target, name);
	return index;
}

bool GLShader::GetBinary(uint& format, sizei buffer_size, void* buffer)
{
	/*glProgramParameteri(_id, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_TRUE);

	glLinkProgram(_id);

	bool result = (glGetError() == GL_NO_ERROR);
	GLint status;
	glGetProgramiv(_id, GL_LINK_STATUS, &status);
	result = result && (status == GL_TRUE);

	RetrieveLog();

	if(!result)
		return false;*/

	GLsizei length;
	GLenum gl_fmt;
	glGetProgramBinary(_id, buffer_size, &length, &gl_fmt, buffer);

	format = gl_fmt;
	return (glGetError() == GL_NO_ERROR);
}

int GLShader::GetBinarySize()
{
	GLint size;
	glGetProgramiv(_id, GL_PROGRAM_BINARY_LENGTH, &size);
	return size;
}

const ShaderBlockInfo* GLShader::GetUniformBlockInfo(const char* blockName)
{
	if (!_id || !blockName || !*blockName)
		return nullptr;

	for (sizei i = 0; i < _uniformBlockCount; ++i)
	{
		if (strcmp(blockName, _uniformBlockInfos[i].name) == 0)
			return &_uniformBlockInfos[i];
	}

	GLuint blockIndex = glGetUniformBlockIndex(_id, blockName);

	if (blockIndex == GL_INVALID_INDEX)
		return nullptr;

	GLint blockDataSize;
	GLint uniformCount;
	GLint* uniformIndices;

	glGetActiveUniformBlockiv(_id, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockDataSize);
	glGetActiveUniformBlockiv(_id, blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &uniformCount);

	uniformIndices = new GLint[uniformCount];

	glGetActiveUniformBlockiv(_id, blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, uniformIndices);

	GLint* uniformTypes = new GLint[uniformCount];
	GLint* uniformArraySizes = new GLint[uniformCount];
	GLint* uniformNameLengths = new GLint[uniformCount];
	GLint* uniformOffsets = new GLint[uniformCount];
	GLint* uniformArrayStrides = new GLint[uniformCount];
	GLint* uniformMatrixStrides = new GLint[uniformCount];
	GLint* uniformIsRowMaj = new GLint[uniformCount];

	glGetActiveUniformsiv(_id, uniformCount, reinterpret_cast<GLuint*>(uniformIndices), GL_UNIFORM_TYPE, uniformTypes);
	glGetActiveUniformsiv(_id, uniformCount, reinterpret_cast<GLuint*>(uniformIndices), GL_UNIFORM_SIZE, uniformArraySizes);
	glGetActiveUniformsiv(_id, uniformCount, reinterpret_cast<GLuint*>(uniformIndices), GL_UNIFORM_NAME_LENGTH, uniformNameLengths);
	glGetActiveUniformsiv(_id, uniformCount, reinterpret_cast<GLuint*>(uniformIndices), GL_UNIFORM_OFFSET, uniformOffsets);
	glGetActiveUniformsiv(_id, uniformCount, reinterpret_cast<GLuint*>(uniformIndices), GL_UNIFORM_ARRAY_STRIDE, uniformArrayStrides);
	glGetActiveUniformsiv(_id, uniformCount, reinterpret_cast<GLuint*>(uniformIndices), GL_UNIFORM_MATRIX_STRIDE, uniformMatrixStrides);
	glGetActiveUniformsiv(_id, uniformCount, reinterpret_cast<GLuint*>(uniformIndices), GL_UNIFORM_IS_ROW_MAJOR, uniformIsRowMaj);

	ShaderBlockInfo blockInfo;

	blockInfo.name = new char[strlen(blockName) + 1];
	strcpy(blockInfo.name, blockName);

	blockInfo.dataSize = static_cast<sizei>(blockDataSize);
	blockInfo.variableCount = static_cast<sizei>(uniformCount);
	blockInfo.variables = new ShaderBlockInfo::VariableInfo[uniformCount];

	for (sizei i = 0; i < blockInfo.variableCount; ++i)
	{
		blockInfo.variables[i].name = new char[uniformNameLengths[i]];
		glGetActiveUniformName(_id, uniformIndices[i], uniformNameLengths[i], nullptr, blockInfo.variables[i].name);

		blockInfo.variables[i].type = GetFromGLEnum<ShaderDataType>(uniformTypes[i]);
		blockInfo.variables[i].index = uniformIndices[i];
		blockInfo.variables[i].offset = uniformOffsets[i];
		blockInfo.variables[i].arraySize = uniformArraySizes[i];
		blockInfo.variables[i].arrayStride = uniformArrayStrides[i];
		blockInfo.variables[i].matrixStride = uniformMatrixStrides[i];
		blockInfo.variables[i].isRowMajor = (uniformIsRowMaj[i] == GL_TRUE);
	}

	// Resize the info array if necessary.
	if (_uniformBlockInfos == nullptr || _uniformBlockCount == _uniformBlockArraySize)
	{
		sizei newSize = _uniformBlockArraySize + 5;
		ShaderBlockInfo* newArr = new ShaderBlockInfo[newSize];

		if (_uniformBlockInfos != nullptr)
			memcpy(newArr, _uniformBlockInfos, sizeof(ShaderBlockInfo) * _uniformBlockCount);

		_uniformBlockInfos = newArr;
		_uniformBlockArraySize = newSize;
	}

	_uniformBlockInfos[_uniformBlockCount++] = blockInfo;

	delete[] uniformIndices;
	delete[] uniformTypes;
	delete[] uniformArraySizes;
	delete[] uniformNameLengths;
	delete[] uniformOffsets;
	delete[] uniformArrayStrides;
	delete[] uniformMatrixStrides;
	delete[] uniformIsRowMaj;

	return &_uniformBlockInfos[_uniformBlockCount - 1];
}

const ShaderBlockInfo* GLShader::GetStorageBlockInfo(const char* blockName)
{
	return nullptr;
}

void GLShader::RetrieveLog()
{
	GLint log_len;
	glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &log_len);

	_logInfoLength = log_len;
	delete[] _logInfo;
	_logInfo = new char[std::max(log_len, 1)];
	_logInfo[0] = '\0';
	if (log_len > 1)
	{
		glGetProgramInfoLog(_id, log_len, 0, _logInfo);
	}
}

void GLShader::RetrieveLog2(GLuint program, GLuint shader)
{
	GLint prog_log_len;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &prog_log_len);

	GLint shader_log_len;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &shader_log_len);

	_logInfoLength = prog_log_len + shader_log_len;
	delete[] _logInfo;
	_logInfo = new char[std::max(_logInfoLength, 1)];
	_logInfo[0] = '\0';

	if (prog_log_len > 1)
	{
		glGetProgramInfoLog(program, prog_log_len, 0, _logInfo);
	}

	if (shader_log_len > 1)
	{
		GLint append_index = std::max(prog_log_len - 1, 0);
		glGetShaderInfoLog(shader, shader_log_len, 0, &_logInfo[append_index]);
	}
}

//================================ GLVertexShader ==============================================

GLVertexShader::GLVertexShader()
{
	_shaderType = ShaderType::Vertex;
	_resType = ResourceType::Shader;
	_target = GL_VERTEX_SHADER;
}

void GLVertexShader::TransformFeedbackVaryings(sizei count, const char** varyings, TransformFeedbackBufferMode mode)
{
	glTransformFeedbackVaryings(_id, count, varyings, GetGLEnum(mode));
}

//================================ GLTessControlShader ==============================================

GLTessControlShader::GLTessControlShader()
{
	_shaderType = ShaderType::TessControl;
	_resType = ResourceType::Shader;
	_target = GL_TESS_CONTROL_SHADER;
}

int GLTessControlShader::GetOutputVertexCount()
{
	GLint count;
	glGetProgramiv(_id, GL_TESS_CONTROL_OUTPUT_VERTICES, &count);
	return count;
}

//================================ GLTessEvaluationShader ==============================================

GLTessEvaluationShader::GLTessEvaluationShader()
{
	_shaderType = ShaderType::TessEvaluation;
	_resType = ResourceType::Shader;
	_target = GL_TESS_EVALUATION_SHADER;
}

TessGenPrimitiveType GLTessEvaluationShader::GetMode()
{
	GLint mode;
	glGetProgramiv(_id, GL_TESS_GEN_MODE, &mode);
	return GetFromGLEnum<TessGenPrimitiveType>(mode);
}

TessGenSpacing GLTessEvaluationShader::GetSpacing()
{
	GLint spacing;
	glGetProgramiv(_id, GL_TESS_GEN_SPACING, &spacing);
	return GetFromGLEnum<TessGenSpacing>(spacing);
}

VertexWinding GLTessEvaluationShader::GetVertexOrder()
{
	GLint order;
	glGetProgramiv(_id, GL_TESS_GEN_VERTEX_ORDER, &order);
	return GetFromGLEnum<VertexWinding>(order);
}

bool GLTessEvaluationShader::GetPointMode()
{
	GLint mode;
	glGetProgramiv(_id, GL_TESS_GEN_POINT_MODE, &mode);
	return (mode == GL_TRUE);
}

//================================ GLGeometryShader ==============================================

GLGeometryShader::GLGeometryShader()
{
	_shaderType = ShaderType::Geometry;
	_resType = ResourceType::Shader;
	_target = GL_GEOMETRY_SHADER;
}

void GLGeometryShader::TransformFeedbackVaryings(sizei count, const char** varyings, TransformFeedbackBufferMode mode)
{
	glTransformFeedbackVaryings(_id, count, varyings, GetGLEnum(mode));
}

int GLGeometryShader::GetInvocations()
{
	int invocations;
	glGetProgramiv(_id, GL_GEOMETRY_SHADER_INVOCATIONS, &invocations);
	return invocations;
}

//================================ GLFragmentShader ==============================================

GLFragmentShader::GLFragmentShader()
{
	_shaderType = ShaderType::Fragment;
	_resType = ResourceType::Shader;
	_target = GL_FRAGMENT_SHADER;
}

//================================ GLComputeShader ==============================================

GLComputeShader::GLComputeShader()
{
	_shaderType = ShaderType::Compute;
	_resType = ResourceType::Shader;
	_target = GL_COMPUTE_SHADER;
}

void GLComputeShader::GetWorkGroupSize(int work_group_size[3])
{
	glGetProgramiv(_id, GL_COMPUTE_WORK_GROUP_SIZE, work_group_size);
}

} // namespace gls::internals
