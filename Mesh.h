#pragma once

#include <GL\glew.h>

class Mesh
{
public:
	Mesh();

	void CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices); // ��������
	void RenderMesh(); // ��Ⱦ����
	void ClearMesh(); // �������

	~Mesh();

private:
	GLuint VAO, VBO, IBO; // VAO = Vertex Array Object, VBO = Vertex Buffer Object, IBO = Index Buffer Object
	GLsizei indexCount; // ������������
};

