#include "Mesh.h"

Mesh::Mesh()
{
	VAO = 0;
	VBO = 0;
	IBO = 0;
	indexCount = 0;
}

void Mesh::CreateMesh(GLfloat* vertices, unsigned* indices, unsigned numOfVertices, unsigned numOfIndices)
{
	indexCount = numOfIndices;

	glGenVertexArrays(1, &VAO); // 生成一个 VAO
	glBindVertexArray(VAO); // 绑定 VAO

	glGenBuffers(1, &IBO); // 生成一个 IBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO); // 绑定 IBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW); // 把索引数组复制到缓冲中供 OpenGL 使用

	glGenBuffers(1, &VBO); // 生成一个 VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // 绑定 VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW); // 把顶点数组复制到缓冲中供 OpenGL 使用

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, 0); // 设置顶点属性指针
	glEnableVertexAttribArray(0); // 启用顶点属性
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 3)); // 设置材质属性指针
	glEnableVertexAttribArray(1); // 启用材质属性
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 5)); // 设置法线属性指针
	glEnableVertexAttribArray(2); // 启用法线属性

	glBindBuffer(GL_ARRAY_BUFFER, 0); // 解绑 VBO

	glBindVertexArray(0); // 解绑 VAO

	// IBO、EBO的解绑应该在VAO的解绑之后
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // 解绑 IBO
}

void Mesh::RenderMesh()
{
	glBindVertexArray(VAO); // 绑定 VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO); // 绑定 IBO
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0); // 绘制三角形
	glBindVertexArray(0); // 解绑 VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // 解绑 IBO
}

void Mesh::ClearMesh()
{
	// 先清空IBO、VBO，再清空VAO
	// 检查IBO是否为空，不为空的话清空
	if (IBO != 0)
	{
		glDeleteBuffers(1, &IBO);
		IBO = 0;
	}

	// 检查VBO是否为空，不为空的话清空
	if (VBO != 0)
	{
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}

	// 检查VAO是否为空，不为空的话清空
	if (VAO != 0)
	{
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}

	indexCount = 0;
}

Mesh::~Mesh()
{
	ClearMesh();
}
