#pragma once

#include <GL\glew.h>

class Mesh
{
public:
	Mesh();

	void CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices); // 创建网格
	void RenderMesh(); // 渲染网格
	void ClearMesh(); // 清除网格

	~Mesh();

private:
	GLuint VAO, VBO, IBO; // VAO = Vertex Array Object, VBO = Vertex Buffer Object, IBO = Index Buffer Object
	GLsizei indexCount; // 顶点索引数量
};

