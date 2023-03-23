#pragma once

#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> // 后处理

#include "Mesh.h"
#include "Texture.h"

class Model
{
public:
	Model();

	void LoadModel(const std::string& fileName); // 加载模型
	void RenderModel(); // 渲染模型
	void ClearModel(); // 清除模型

	~Model();

private:
	void LoadNode(aiNode* node, const aiScene* scene); // 加载节点
	void LoadMesh(aiMesh* mesh, const aiScene* scene); // 加载网格
	void LoadMaterials(const aiScene* scene); // 加载场景中所有网格的材质

	std::vector<Mesh*> meshList;
	std::vector<Texture*> textureList;
	std::vector<unsigned int> meshToTex; // 网格到纹理的映射
};

