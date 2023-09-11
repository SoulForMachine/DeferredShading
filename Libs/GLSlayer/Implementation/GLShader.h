
#ifndef _GL_SHADER_H_
#define _GL_SHADER_H_

#include "GLSlayer/Shader.h"
#include "GLSlayer/TransformFeedback.h"
#include "GLResource.h"


#define IMPLEMENT_ISHADER \
	virtual ShaderType GetShaderType() override { return GLShader::GetShaderType(); } \
	virtual const char* GetInfoLog() override { return GLShader::GetInfoLog(); } \
	virtual int GetInfoLogLength() override { return GLShader::GetInfoLogLength(); } \
	virtual bool Validate() override { return GLShader::Validate(); } \
	virtual uint GetSubroutineIndex(const char* name) override { return GLShader::GetSubroutineIndex(name); } \
	virtual bool GetBinary(uint& format, sizei buffer_size, void* buffer) override { return GLShader::GetBinary(format, buffer_size, buffer); } \
	virtual int GetBinarySize() override { return GLShader::GetBinarySize(); } \
	virtual const ShaderBlockInfo* GetUniformBlockInfo(const char* blockName) override { return GLShader::GetUniformBlockInfo(blockName); } \
	virtual const ShaderBlockInfo* GetStorageBlockInfo(const char* blockName) override { return GLShader::GetStorageBlockInfo(blockName); }


namespace gls::internals
{

class GLShader : public GLResource
{
public:
	static const int typeID = TYPE_ID_SHADER;

	GLShader();
	~GLShader() = 0;

	GLShader(const GLShader&) = delete;
	GLShader& operator = (const GLShader&) = delete;

	bool Create(sizei count, const char** source);
	bool Create(sizei size, const void* binary, uint format);
	bool CreateWithTransformFeedback(sizei count, const char** source, sizei attrib_count, const char** attrib_names);
	bool CreateWithTransformFeedback(sizei size, const void* binary, uint format, sizei attrib_count, const char** attrib_names);
	void Destroy();

	void* DynamicCast(int type_id) { return (type_id == TYPE_ID_SHADER) ? this : GLResource::DynamicCast(type_id); }
	ShaderType GetShaderType();
	const char* GetInfoLog();
	int GetInfoLogLength();
	bool Validate();
	uint GetSubroutineIndex(const char* name);
	bool GetBinary(uint& format, sizei buffer_size, void* buffer);
	int GetBinarySize();
	const ShaderBlockInfo* GetUniformBlockInfo(const char* blockName);
	const ShaderBlockInfo* GetStorageBlockInfo(const char* blockName);

protected:
	ShaderType _shaderType;

private:
	void RetrieveLog();
	void RetrieveLog2(GLuint program, GLuint shader);

	char* _logInfo;
	int _logInfoLength;

	sizei _uniformBlockCount;
	sizei _uniformBlockArraySize;
	ShaderBlockInfo* _uniformBlockInfos;

	sizei _storageBlockCount;
	sizei _storageBlockArraySize;
	ShaderBlockInfo* _storageBlockInfos;
};


class GLVertexShader : public IVertexShader, public GLShader
{
public:
	IMPLEMENT_IRESOURCE
	IMPLEMENT_ISHADER

	static const int typeID = TYPE_ID_VERTEX_SHADER;

	GLVertexShader();

	GLVertexShader(const GLVertexShader&) = delete;
	GLVertexShader& operator = (const GLVertexShader&) = delete;

	virtual void* DynamicCast(int type_id) override { return (type_id == TYPE_ID_VERTEX_SHADER) ? this : GLShader::DynamicCast(type_id); }
	virtual void TransformFeedbackVaryings(sizei count, const char** varyings, TransformFeedbackBufferMode mode) override;
};


class GLTessControlShader : public ITessControlShader, public GLShader
{
public:
	IMPLEMENT_IRESOURCE
	IMPLEMENT_ISHADER

	static const int typeID = TYPE_ID_TESS_CONTROL_SHADER;

	GLTessControlShader();

	GLTessControlShader(const GLTessControlShader&) = delete;
	GLTessControlShader& operator = (const GLTessControlShader&) = delete;

	virtual void* DynamicCast(int type_id) override { return (type_id == TYPE_ID_TESS_CONTROL_SHADER) ? this : GLShader::DynamicCast(type_id); }
	virtual int GetOutputVertexCount() override;
};


class GLTessEvaluationShader : public ITessEvaluationShader, public GLShader
{
public:
	IMPLEMENT_IRESOURCE
	IMPLEMENT_ISHADER

	static const int typeID = TYPE_ID_TESS_EVAL_SHADER;

	GLTessEvaluationShader();

	GLTessEvaluationShader(const GLTessEvaluationShader&) = delete;
	GLTessEvaluationShader& operator = (const GLTessEvaluationShader&) = delete;

	virtual void* DynamicCast(int type_id) override { return (type_id == TYPE_ID_TESS_EVAL_SHADER) ? this : GLShader::DynamicCast(type_id); }
	virtual TessGenPrimitiveType GetMode() override;
	virtual TessGenSpacing GetSpacing() override;
	virtual VertexWinding GetVertexOrder() override;
	virtual bool GetPointMode() override;
};


class GLGeometryShader : public IGeometryShader, public GLShader
{
public:
	IMPLEMENT_IRESOURCE
	IMPLEMENT_ISHADER

	static const int typeID = TYPE_ID_GEOMETRY_SHADER;

	GLGeometryShader();

	GLGeometryShader(const GLGeometryShader&) = delete;
	GLGeometryShader& operator = (const GLGeometryShader&) = delete;

	virtual void* DynamicCast(int type_id) override { return (type_id == TYPE_ID_GEOMETRY_SHADER) ? this : GLShader::DynamicCast(type_id); }
	virtual void TransformFeedbackVaryings(sizei count, const char** varyings, TransformFeedbackBufferMode mode) override;
	virtual int GetInvocations() override;
};


class GLFragmentShader : public IFragmentShader, public GLShader
{
public:
	IMPLEMENT_IRESOURCE
	IMPLEMENT_ISHADER

	static const int typeID = TYPE_ID_FRAGMENT_SHADER;

	GLFragmentShader();

	GLFragmentShader(const GLFragmentShader&) = delete;
	GLFragmentShader& operator = (const GLFragmentShader&) = delete;

	virtual void* DynamicCast(int type_id) override { return (type_id == TYPE_ID_FRAGMENT_SHADER) ? this : GLShader::DynamicCast(type_id); }
};


class GLComputeShader : public IComputeShader, public GLShader
{
public:
	IMPLEMENT_IRESOURCE
	IMPLEMENT_ISHADER

	static const int typeID = TYPE_ID_FRAGMENT_SHADER;

	GLComputeShader();

	GLComputeShader(const GLComputeShader&) = delete;
	GLComputeShader& operator = (const GLComputeShader&) = delete;

	virtual void* DynamicCast(int type_id) override { return (type_id == TYPE_ID_COMPUTE_SHADER) ? this : GLShader::DynamicCast(type_id); }
	virtual void GetWorkGroupSize(int work_group_size[3]) override;
};

} // namespace gls::internals

#endif // _GL_SHADER_H_
