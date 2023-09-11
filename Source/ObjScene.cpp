
#include "ObjScene.h"
#include <limits>
#include <algorithm>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobjloader/tiny_obj_loader.h>
#include "TgaLoader.h"
#include "Utils.h"


static math3d::vec3f CalculateTangent(
	const math3d::vec3f& p0, const math3d::vec3f& p1, const math3d::vec3f& p2,
	const math3d::vec2f& t0, const math3d::vec2f& t1, const math3d::vec2f& t2)
{
	//(v1 - v0)(P2 - P0) - (v2 - v0)(P1 - P0)
	//---------------------------------------
	//(v1 - v0)(u2 - u0) - (v2 - v0)(u1 - u0)

	math3d::vec3f vec = (t1.y - t0.y) * (p2 - p0) - (t2.y - t0.y) * (p1 - p0);
	float denom = (t1.y - t0.y) * (t2.x - t0.x) - (t2.y - t0.y) * (t1.x - t0.x);
	if (std::abs(denom) < 0.0001f)
		denom = 1.0f;
	if (vec.length() < 0.0001f)
		vec = math3d::vec3_x_axis<float>;

	return vec / denom;
}


ObjScene::~ObjScene()
{
	Unload();
}

bool ObjScene::Load(gls::IRenderContext* renderContext, const char* objFileName)
{
	if (renderContext == nullptr)
		return false;

	Unload();

	std::string relFilePath = std::string("../Models/") + objFileName;
	std::string fullFilePath = GetFullPath(relFilePath.c_str());
	std::string fullDirPath = fullFilePath.substr(0, fullFilePath.find_last_of("/\\"));
	std::vector<ObjScene::MaterialData> materials;
	std::vector<ObjScene::Vertex> vertices;
	std::vector<int32_t> indices;

	if (!LoadCache(fullFilePath, _meshes, materials, vertices, indices))
	{
		if (LoadObj(fullFilePath, _meshes, materials, vertices, indices))
			SaveCache(fullFilePath, _meshes, materials, vertices, indices);
		else
			return false;
	}

	auto cmpFunc = [](const auto& a, const auto& b) -> bool {
		return a.materialIndex < b.materialIndex;
	};
	std::sort(_meshes.begin(), _meshes.end(), cmpFunc);

	_renderContext = renderContext;

	// Create vertex and index buffers.
	
	_vertexBuffer = _renderContext->CreateBuffer(vertices.size() * sizeof(Vertex), vertices.data(), 0);
	_indexBuffer = _renderContext->CreateBuffer(indices.size() * 4, indices.data(), 0);

	// Create textures.

	size_t numMats = materials.size();
	_materials.resize(numMats);

	for (size_t matIndex = 0; matIndex < numMats; ++matIndex)
	{
		if (materials[matIndex].diffuseTexture.empty() == false)
		{
			TgaLoader tgaLoader;
			std::string fullTexPath = fullDirPath + "/" + materials[matIndex].diffuseTexture;
			if (tgaLoader.Load(fullTexPath.c_str()))
			{
				const TgaLoader::ImageData& imgData = tgaLoader.GetImageData();
				if (imgData.bits >= 24)
				{
					gls::PixelFormat internalFormat = (imgData.format == TgaLoader::ImagePixelFormat::RGB) ? gls::PixelFormat::RGB8 : gls::PixelFormat::RGBA8;
					gls::ImageFormat imageFormat = (imgData.format == TgaLoader::ImagePixelFormat::RGB) ? gls::ImageFormat::RGB : gls::ImageFormat::RGBA;
					gls::PixelStore ps;
					ps.rowLength = imgData.bytesPerScanline;
					int numMipLevels = GetNumMipLevels(imgData.width, imgData.height);
					_materials[matIndex].diffuseTexture = _renderContext->CreateTexture2D(numMipLevels, internalFormat, imgData.width, imgData.height);
					_materials[matIndex].diffuseTexture->TexSubImage(0, 0, 0, imgData.width, imgData.height, imageFormat, gls::DataType::UnsignedByte, nullptr, imgData.pixels);
					_materials[matIndex].diffuseTexture->GenerateMipmap();
				}
			}
		}

		if (materials[matIndex].normalTexture.empty() == false)
		{
			TgaLoader tgaLoader;
			std::string fullTexPath = fullDirPath + "/" + materials[matIndex].normalTexture;
			if (tgaLoader.Load(fullTexPath.c_str()))
			{
				const TgaLoader::ImageData& imgData = tgaLoader.GetImageData();
				if (imgData.bits >= 24)
				{
					gls::ImageFormat imageFormat = (imgData.format == TgaLoader::ImagePixelFormat::RGB) ? gls::ImageFormat::RGB : gls::ImageFormat::RGBA;
					gls::PixelStore ps;
					ps.rowLength = imgData.bytesPerScanline;
					int numMipLevels = GetNumMipLevels(imgData.width, imgData.height);
					_materials[matIndex].normalTexture = _renderContext->CreateTexture2D(numMipLevels, gls::PixelFormat::RGB8, imgData.width, imgData.height);
					_materials[matIndex].normalTexture->TexSubImage(0, 0, 0, imgData.width, imgData.height, imageFormat, gls::DataType::UnsignedByte, nullptr, imgData.pixels);
					_materials[matIndex].normalTexture->GenerateMipmap();
				}
			}
		}

		_materials[matIndex].name = materials[matIndex].name;
		_materials[matIndex].transparent = materials[matIndex].transparent;
	}

	return true;
}

void ObjScene::Unload()
{
	if (_renderContext != nullptr)
	{
		_renderContext->DestroyBuffer(_vertexBuffer);
		_vertexBuffer = nullptr;

		_renderContext->DestroyBuffer(_indexBuffer);
		_indexBuffer = nullptr;

		for (auto& mat : _materials)
		{
			_renderContext->DestroyTexture(mat.diffuseTexture);
			_renderContext->DestroyTexture(mat.normalTexture);
		}

		_meshes.clear();
		_materials.clear();
		_renderContext = nullptr;
	}
}

void ObjScene::GetBounds(math3d::vec3f& minPt, math3d::vec3f& maxPt)
{
	if (_meshes.empty())
	{
		minPt.set_null();
		maxPt.set_null();
		return;
	}

	minPt = _meshes[0].minPt;
	maxPt = _meshes[0].maxPt;

	for (auto& mesh : _meshes)
	{
		ExpandBounds(minPt, maxPt, mesh.minPt);
		ExpandBounds(minPt, maxPt, mesh.maxPt);
	}
}

template<typename _DestT, typename _SrcT>
void WriteInt(FILE* file, _SrcT intVar)
{
	static_assert(std::is_integral_v<_SrcT>);
	static_assert(std::is_integral_v<_DestT>);

	_DestT tmp = static_cast<_DestT>(intVar);
	fwrite(&tmp, sizeof(_DestT), 1, file);
}

static void WriteString(FILE* file, const std::string& str)
{
	uint32_t strLen = static_cast<uint32_t>(str.size());
	fwrite(&strLen, 4, 1, file);
	fwrite(str.c_str(), 1, strLen, file);
}

static void WriteVec3(FILE* file, const math3d::vec3f& vec)
{
	fwrite(vec, 4, 3, file);
}

bool ObjScene::LoadObj(
	const std::string& objFilePath,
	std::vector<ObjScene::Mesh>& meshes,
	std::vector<ObjScene::MaterialData>& materials,
	std::vector<ObjScene::Vertex>& vertices,
	std::vector<int32_t>& indices)
{
	std::string fullDirPath = objFilePath.substr(0, objFilePath.find_last_of("/\\"));

	tinyobj::ObjReaderConfig config;
	config.triangulate = false;
	config.vertex_color = false;
	config.mtl_search_path = fullDirPath;

	tinyobj::ObjReader objReader;
	if (!objReader.ParseFromFile(objFilePath, config))
		return false;

	const tinyobj::attrib_t& attrib = objReader.GetAttrib();
	const std::vector<tinyobj::shape_t>& shapes = objReader.GetShapes();
	const std::vector<tinyobj::material_t>& objMaterials = objReader.GetMaterials();

	size_t numMeshes = shapes.size();

	int indexOffset = 0;
	int numVertices = 0;

	meshes.resize(numMeshes);

	for (size_t meshInd = 0; meshInd < numMeshes; ++meshInd)
	{
		const tinyobj::shape_t& shape = shapes[meshInd];
		size_t numIndices = shape.mesh.indices.size();
		Mesh& mesh = meshes[meshInd];
		mesh.name = shape.name;
		mesh.indexOffset = indexOffset;
		mesh.numIndices = static_cast<int>(numIndices);
		mesh.materialIndex = shape.mesh.material_ids[0];
		constexpr float lowflt = std::numeric_limits<float>::lowest();
		constexpr float maxflt = std::numeric_limits<float>::max();
		mesh.minPt.set(maxflt, maxflt, maxflt);
		mesh.maxPt.set(lowflt, lowflt, lowflt);

		indexOffset += static_cast<int>(numIndices);
		std::vector<tinyobj::index_t> refdVerts;

		// Copy vertex attributes and mesh indices.

		for (auto& index : shape.mesh.indices)
		{
			auto it = std::find_if(refdVerts.begin(), refdVerts.end(), [&index](const auto& prevIndex) {
				return (
					index.vertex_index == prevIndex.vertex_index &&
					index.normal_index == prevIndex.normal_index &&
					index.texcoord_index == prevIndex.texcoord_index);
				});

			if (it == refdVerts.end())
			{
				Vertex vertex;

				vertex.position.x = attrib.vertices[3 * index.vertex_index + 0];
				vertex.position.y = attrib.vertices[3 * index.vertex_index + 1];
				vertex.position.z = attrib.vertices[3 * index.vertex_index + 2];

				vertex.normal.x = attrib.normals[3 * index.normal_index + 0];
				vertex.normal.y = attrib.normals[3 * index.normal_index + 1];
				vertex.normal.z = attrib.normals[3 * index.normal_index + 2];
				vertex.normal.normalize();

				vertex.texcoords.x = attrib.texcoords[2 * index.texcoord_index + 0];
				vertex.texcoords.y = attrib.texcoords[2 * index.texcoord_index + 1];

				vertices.push_back(vertex);
				refdVerts.push_back(index);
				indices.push_back(static_cast<int32_t>(vertices.size() - 1));

				// Update mesh bounds.
				ExpandBounds(mesh.minPt, mesh.maxPt, vertex.position);
			}
			else
			{
				indices.push_back(static_cast<int32_t>(numVertices + std::distance(refdVerts.begin(), it)));
			}
		}

		numVertices = static_cast<int>(vertices.size());
	}

	// Calculate vertex tangents.

	size_t numIndices = indices.size();

	for (size_t i = 0; i < numIndices; i += 3)
	{
		Vertex& v1 = vertices[indices[i + 0]];
		Vertex& v2 = vertices[indices[i + 1]];
		Vertex& v3 = vertices[indices[i + 2]];

		math3d::vec3f tangent = CalculateTangent(v1.position, v2.position, v3.position, v1.texcoords, v2.texcoords, v3.texcoords);

		math3d::vec3f bitangent;

		bitangent = math3d::cross(v1.normal, tangent);
		v1.tangent = math3d::normalize(math3d::cross(bitangent, v1.normal));

		bitangent = math3d::cross(v2.normal, tangent);
		v2.tangent = math3d::normalize(math3d::cross(bitangent, v2.normal));

		bitangent = math3d::cross(v3.normal, tangent);
		v3.tangent = math3d::normalize(math3d::cross(bitangent, v3.normal));
	}

	// Material data.

	size_t numMats = objMaterials.size();
	materials.resize(numMats);

	for (size_t matIndex = 0; matIndex < numMats; ++matIndex)
	{
		materials[matIndex].name = objMaterials[matIndex].name;
		materials[matIndex].diffuseTexture = objMaterials[matIndex].diffuse_texname;
		materials[matIndex].normalTexture = objMaterials[matIndex].bump_texname;
		materials[matIndex].transparent = objMaterials[matIndex].dissolve > 0.0f;
	}

	return true;
}

void ObjScene::SaveCache(
	const std::string& objFilePath,
	const std::vector<ObjScene::Mesh>& meshes,
	const std::vector<ObjScene::MaterialData>& materials,
	const std::vector<ObjScene::Vertex>& vertices,
	const std::vector<int32_t>& indices)
{
	std::string cacheFilePath = objFilePath + ".cache";
	FILE* file = fopen(cacheFilePath.c_str(), "wb");
	if (file == nullptr)
		return;

	WriteInt<uint32_t>(file, meshes.size());

	for (auto& mesh : meshes)
	{
		WriteString(file, mesh.name);
		WriteInt<uint32_t>(file, mesh.indexOffset);
		WriteInt<uint32_t>(file, mesh.numIndices);
		WriteInt<uint32_t>(file, mesh.materialIndex);
		WriteVec3(file, mesh.minPt);
		WriteVec3(file, mesh.maxPt);
	}


	WriteInt<uint32_t>(file, materials.size());

	for (auto& mat : materials)
	{
		WriteString(file, mat.name);
		WriteString(file, mat.diffuseTexture);
		WriteString(file, mat.normalTexture);
		WriteInt<uint8_t>(file, mat.transparent);
	}

	WriteInt<uint32_t>(file, vertices.size());
	fwrite(vertices.data(), sizeof(Vertex), vertices.size(), file);

	WriteInt<uint32_t>(file, indices.size());
	fwrite(indices.data(), 4, indices.size(), file);

	fclose(file);
}

template<typename _IntT>
_IntT ReadInt(FILE* file)
{
	_IntT tmp;
	fread(&tmp, sizeof(_IntT), 1, file);
	return tmp;
}

static std::string ReadString(FILE* file)
{
	uint32_t strLen;
	fread(&strLen, 4, 1, file);
	std::string str(strLen, ' ');
	fread(str.data(), 1, strLen, file);
	return str;
}

static math3d::vec3f ReadVec3(FILE* file)
{
	math3d::vec3f vec;
	fread(vec, 4, 3, file);
	return vec;
}

bool ObjScene::LoadCache(
	const std::string& objFilePath,
	std::vector<ObjScene::Mesh>& meshes,
	std::vector<ObjScene::MaterialData>& materials,
	std::vector<ObjScene::Vertex>& vertices,
	std::vector<int32_t>& indices)
{
	std::string cacheFilePath = objFilePath + ".cache";
	FILE* file = fopen(cacheFilePath.c_str(), "rb");
	if (file == nullptr)
		return false;

	uint32_t numMeshes = ReadInt<uint32_t>(file);
	meshes.resize(numMeshes);

	for (uint32_t i = 0; i < numMeshes; ++i)
	{
		meshes[i].name = ReadString(file);
		meshes[i].indexOffset = ReadInt<uint32_t>(file);
		meshes[i].numIndices = ReadInt<uint32_t>(file);
		meshes[i].materialIndex = ReadInt<uint32_t>(file);
		meshes[i].minPt = ReadVec3(file);
		meshes[i].maxPt = ReadVec3(file);
	}

	uint32_t numMats = ReadInt<uint32_t>(file);
	materials.resize(numMats);

	for (uint32_t i = 0; i < numMats; ++i)
	{
		materials[i].name = ReadString(file);
		materials[i].diffuseTexture = ReadString(file);
		materials[i].normalTexture = ReadString(file);
		materials[i].transparent = static_cast<bool>(ReadInt<uint8_t>(file));
	}

	uint32_t numVerts = ReadInt<uint32_t>(file);
	vertices.resize(numVerts);
	fread(vertices.data(), sizeof(Vertex), numVerts, file);

	uint32_t numInds = ReadInt<uint32_t>(file);
	indices.resize(numInds);
	fread(indices.data(), 4, numInds, file);

	fclose(file);
	return true;
}
