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

	glGenVertexArrays(1, &VAO); // ����һ�� VAO
	glBindVertexArray(VAO); // �� VAO

	glGenBuffers(1, &IBO); // ����һ�� IBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO); // �� IBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW); // ���������鸴�Ƶ������й� OpenGL ʹ��

	glGenBuffers(1, &VBO); // ����һ�� VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // �� VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW); // �Ѷ������鸴�Ƶ������й� OpenGL ʹ��

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, 0); // ���ö�������ָ��
	glEnableVertexAttribArray(0); // ���ö�������
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 3)); // ���ò�������ָ��
	glEnableVertexAttribArray(1); // ���ò�������
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 5)); // ���÷�������ָ��
	glEnableVertexAttribArray(2); // ���÷�������

	glBindBuffer(GL_ARRAY_BUFFER, 0); // ��� VBO

	glBindVertexArray(0); // ��� VAO

	// IBO��EBO�Ľ��Ӧ����VAO�Ľ��֮��
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // ��� IBO
}

void Mesh::RenderMesh()
{
	glBindVertexArray(VAO); // �� VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO); // �� IBO
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0); // ����������
	glBindVertexArray(0); // ��� VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // ��� IBO
}

void Mesh::ClearMesh()
{
	// �����IBO��VBO�������VAO
	// ���IBO�Ƿ�Ϊ�գ���Ϊ�յĻ����
	if (IBO != 0)
	{
		glDeleteBuffers(1, &IBO);
		IBO = 0;
	}

	// ���VBO�Ƿ�Ϊ�գ���Ϊ�յĻ����
	if (VBO != 0)
	{
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}

	// ���VAO�Ƿ�Ϊ�գ���Ϊ�յĻ����
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
