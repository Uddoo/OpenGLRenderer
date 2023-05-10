#include "Model.h"

Model::Model()
{
}

void Model::LoadModel(const std::string& fileName)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices); // �����λ���תUV���ꡢƽ�����ߡ�ͬλ������ϲ�

	if (!scene)
	{
		printf("Model (%s) failed to load: %s", fileName.c_str(), importer.GetErrorString());
		return;
	}

	LoadNode(scene->mRootNode, scene); // �ݹ���ؽڵ�

	LoadMaterials(scene); // ���ز���
}

void Model::RenderModel()
{
	for (size_t i = 0; i < meshList.size(); i++)
	{
		unsigned int materialIndex = meshToTex[i];
		if (materialIndex < textureList.size() && textureList[materialIndex])
		{
			textureList[materialIndex]->UseTexture();
		}
		meshList[i]->RenderMesh();
	}
}

void Model::ClearModel()
{
	for (size_t i = 0; i < meshList.size(); i++)
	{
		if (meshList[i])
		{
			delete meshList[i];
			meshList[i] = nullptr;
		}
	}

	for (size_t i = 0; i < textureList.size(); i++)
	{
		if (textureList[i])
		{
			delete textureList[i];
			textureList[i] = nullptr;
		}
	}
}

Model::~Model()
{
}

void Model::LoadNode(aiNode* node, const aiScene* scene)
{
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);
	}

	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		LoadNode(node->mChildren[i], scene);
	}
}

void Model::LoadMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<GLfloat> vertices; // ����
	std::vector<unsigned int> indices; // ����

	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		vertices.insert(vertices.end(), { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z }); // ��������

		// ��mesh��ĳһ������û������Ҳ�������������������Ҫ���
		if (mesh->mTextureCoords[0])
		{
			vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
		}
		else
		{
			vertices.insert(vertices.end(), { 0.0f, 0.0f });
		}

		vertices.insert(vertices.end(), { -mesh->mNormals->x, -mesh->mNormals->y, -mesh->mNormals->z }); // ���뷨����������
		// ��Ϊ��shader.frag�м��� diffuseFactor = max(dot(normalize(Normal), normalize(direction)), 0.0f); ʱ��û�ж�normalize(direction)��ת����
	}

	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	Mesh* newMesh = new Mesh();
	newMesh->CreateMesh(&vertices[0], &indices[0], vertices.size(), indices.size());
	meshList.push_back(newMesh);
	meshToTex.push_back(mesh->mMaterialIndex);
}

void Model::LoadMaterials(const aiScene* scene)
{
	textureList.resize(scene->mNumMaterials);

	for (size_t i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* material = scene->mMaterials[i];

		textureList[i] = nullptr;

		if (material->GetTextureCount(aiTextureType_DIFFUSE)) // �����������������������assimp���У�GetTextureCount(aiTextureType_DIFFUSE)�����ڻ�ȡģ����ָ�����͵����������ĺ��������У�aiTextureType_DIFFUSE��ʾ����������
		{
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				// ����·��
				int idx = std::string(path.data).rfind("\\");
				std::string filename = std::string(path.data).substr(idx + 1); // ����ַ��Ϣ����һ��\\

				std::string texPath = std::string("Textures/") + filename;

				// ��������
				textureList[i] = new Texture(texPath.c_str());

				// ���Լ�������������ɹ���ӡ������Ϣ������
				// ����ԭ��ʹ�õ�LoadTexture()�������ڼ�������ʱ���ὫͼƬ����ɫͨ��ת��ΪRGBA�������ǵ�ͼƬ��RGB����˻��������
				if (!textureList[i]->LoadTextureA()) 
				{
					printf("Failed to load texture at: %s", texPath.c_str());
					delete textureList[i];
					textureList[i] = nullptr;
				}
			}
		}

		if (!textureList[i])
		{
			textureList[i] = new Texture("Textures/plain.png");
			textureList[i]->LoadTextureA();
		}
	}
}
