#ifndef _OBJ_SCENE_H_
#define _OBJ_SCENE_H_

#include <vector>
#include <string>
#include <GLSlayer/RenderContext.h>
#include "Math/math3d.h"


class ObjScene
{
public:
	ObjScene() = default;
	~ObjScene();

#pragma pack(push, 1)
	struct Vertex
	{
		math3d::vec3f position;
		math3d::vec3f normal;
		math3d::vec3f tangent;
		math3d::vec2f texcoords;
	};
#pragma pack(pop)

	struct Material
	{
		std::string name;
		gls::ITexture2D* diffuseTexture = nullptr;
		gls::ITexture2D* normalTexture = nullptr;
		bool transparent = false;
	};

	struct Mesh
	{
		std::string name;
		int indexOffset;
		int numIndices;
		int materialIndex;
		math3d::vec3f minPt;
		math3d::vec3f maxPt;
	};

	bool Load(gls::IRenderContext* renderContext, const char* objFileName);
	void Unload();

	int GetMeshCount() const { return (int)_meshes.size(); }
	const Mesh& GetMesh(int index) const { return _meshes[index]; }
	
	int GetMaterialCount() const { return (int)_materials.size(); }
	const Material& GetMaterial(int index) const { return _materials[index]; }

	gls::IBuffer* GetVertexBuffer() const { return _vertexBuffer; }
	gls::IBuffer* GetIndexBuffer() const { return _indexBuffer; }

	void GetBounds(math3d::vec3f& minPt, math3d::vec3f& maxPt);

private:
	struct MaterialData
	{
		std::string name;
		std::string diffuseTexture;
		std::string normalTexture;
		bool transparent = false;
	};

	static bool LoadObj(
		const std::string& objFilePath,
		std::vector<ObjScene::Mesh>& meshes,
		std::vector<ObjScene::MaterialData>& materials,
		std::vector<ObjScene::Vertex>& vertices,
		std::vector<int32_t>& indices);

	static void SaveCache(
		const std::string& objFilePath,
		const std::vector<ObjScene::Mesh>& meshes,
		const std::vector<ObjScene::MaterialData>& materials,
		const std::vector<ObjScene::Vertex>& vertices,
		const std::vector<int32_t>& indices);

	static bool LoadCache(
		const std::string& objFilePath,
		std::vector<ObjScene::Mesh>& meshes,
		std::vector<ObjScene::MaterialData>& materials,
		std::vector<ObjScene::Vertex>& vertices,
		std::vector<int32_t>& indices);

	gls::IRenderContext* _renderContext = nullptr;
	gls::IBuffer* _vertexBuffer = nullptr;
	gls::IBuffer* _indexBuffer = nullptr;
	std::vector<Mesh> _meshes;
	std::vector<Material> _materials;
};


#endif // _OBJ_SCENE_H_
