#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CommonValues.h"

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "Material.h"
#include "Model.h"

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

#include "assimp/Importer.hpp"

const float toRadians = 3.14159265f / 180.0f; // 角度转弧度

Window mainWindow; // 主窗口

Camera camera; // 相机

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;

Material shinyMaterial; // 高光材质
Material dullMaterial; // 低光材质

Model spaceship;
Model nanosuit;
Model xwing;
Model uh60;

DirectionalLight mainLight; // 光源
PointLight pointLights[MAX_POINT_LIGHTS]; // 点光源
SpotLight spotLights[MAX_SPOT_LIGHTS]; // 聚光灯

GLfloat deltaTime = 0.0f; // 帧间隔时间
GLfloat lastTime = 0.0f;

std::vector<Mesh*> meshList; // 网格列表

std::vector<Shader*> shaderList; // 着色器列表

// Vertex Shader
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "Shaders/shader.frag";

void CalcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	// 顶点索引
	unsigned int indices[] = {
			0, 3, 1,
			1, 3, 2,
			2, 3, 0,
			0, 1, 2
	};

	// 顶点数组
	GLfloat vertices[] = {
		//  x      y     z      u     v      nx    ny    nz
		-1.0f, -1.0f, -0.6f,  0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 1.0f,   0.5f, 0.0f,   0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -0.6f,   1.0f, 0.0f,   0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,    0.5f, 1.0f,   0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		0, 3, 2
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,   0.0f, 0.0f,   0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,   10.0f, 0.0f,   0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,   10.0f, 10.0f,   0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,   0.0f, 10.0f,   0.0f, -1.0f, 0.0f
	};

	CalcAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(shader1);
}

int main()
{
	mainWindow = Window(1920, 1080);
	mainWindow.Initialise();

	CreateObjects(); // 创建三角形
	CreateShaders(); // 创建着色器

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 1.0f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA(); // 加载纹理
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();

	shinyMaterial = Material(1.0f, 256);
	dullMaterial = Material(0.3f, 4);

	spaceship = Model();
	spaceship.LoadModel("Models/Intergalactic_Spaceship-(Wavefront).obj");

	nanosuit = Model();
	nanosuit.LoadModel("Models/nanosuit.obj");

	xwing = Model();
	xwing.LoadModel("Models/x-wing.obj");

	uh60 = Model();
	uh60.LoadModel("Models/uh60.obj");

	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, /*环境光光源*/ 0.6f,
		0.0f, -1.0f, -2.0f); // 漫反射光源

	// 创建点光源
	unsigned int pointLightCount = 0;
	// 绿色点光源
	pointLights[0] = PointLight(0.0f, 1.0f, 0.0f,
		0.1f, 0.1f,
		-4.0f, 0.0f, 0.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;
	// 红色点光源
	pointLights[1] = PointLight(1.0f, 0.0f, 0.0f,
		0.1f, 0.1f,
		4.0f, 0.0f, 0.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;
	// 蓝色点光源
	pointLights[2] = PointLight(0.0f, 0.0f, 1.0f,
		0.1f, 1.0f,
		0.0f, 0.0f, 4.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	// 创建聚光灯
	unsigned int spotLightCount = 0;
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 1.0f,
		-4.0f, 0.0f, -2.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		30.f);
	spotLightCount++;
	spotLights[1] = SpotLight(1.0f, 0.0f, 1.0f,
		0.1f, 1.0f,
		4.0f, 1.5f, 0.0f,
		0.0f, -1.0f, -1.0f,
		0.2f, 0.1f, 0.1f,
		20.f);
	spotLightCount++;


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.GetBufferWidth() / (GLfloat)mainWindow.GetBufferHeight(), 0.1f, 100.0f); // 创建投影矩阵

	// 循环，直到窗口关闭
	while (!mainWindow.GetShouldClose())
	{
		GLfloat now = glfwGetTime(); // SDL_GetPerformanceCounter();
		deltaTime = now - lastTime; // (now - lastTime) * 1000 / SDL_GetPerformanceFrequency();
		lastTime = now;

		// 获取并处理事件
		glfwPollEvents();

		camera.KeyControl(mainWindow.GetKeys(), deltaTime); // 键盘控制
		camera.MouseControl(mainWindow.GetXChange(), mainWindow.GetYChange()); // 鼠标控制

		// 清除窗口
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清除颜色缓冲和深度缓冲

		shaderList[0]->UseShader();
		uniformModel = shaderList[0]->GetModelLocation();
		uniformProjection = shaderList[0]->GetProjectionLocation();
		uniformView = shaderList[0]->GetViewLocation();
		uniformSpecularIntensity = shaderList[0]->GetSpecularIntensityLocation();
		uniformShininess = shaderList[0]->GetShininessLocation();
		uniformEyePosition = shaderList[0]->GetEyePositionLocation();

		// 设置手电筒
		glm::vec3 lowerLight = camera.GetCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.GetCameraDirection());

		shaderList[0]->SetDirectionalLight(&mainLight);
		shaderList[0]->SetPointLights(pointLights, pointLightCount);
		shaderList[0]->SetSpotLights(spotLights, spotLightCount);

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection)); // 设置 uniform 变量 projection
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.CalculateViewMatrix())); // 设置 uniform 变量 view
		glUniform3f(uniformEyePosition, camera.GetCameraPosition().x, camera.GetCameraPosition().y, camera.GetCameraPosition().z); // 设置 uniform 变量 eyePosition

		glm::mat4 model(1.0f); // 创建模型矩阵

		//model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 1.0f)); // 旋转
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.0f)); // 平移
		//model = glm::scale(model, glm::vec3(0.4, 0.4, 1.0f)); // 缩放
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); // 设置 uniform 变量 model
		brickTexture.UseTexture();
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess); // 使用高光材质
		meshList[0]->RenderMesh();

		model = glm::mat4(1.0f); // 重置模型矩阵
		model = glm::translate(model, glm::vec3(0.0f, 4.0f, -2.0f)); // 平移
		//model = glm::scale(model, glm::vec3(0.4, 0.4, 1.0f)); // 缩放
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); // 设置 uniform 变量 model
		dirtTexture.UseTexture();
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess); // 使用低光材质
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0f); // 重置模型矩阵
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f)); // 平移
		//model = glm::scale(model, glm::vec3(0.4, 0.4, 1.0f)); // 缩放
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); // 设置 uniform 变量 model
		plainTexture.UseTexture();
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess); // 使用低光材质
		meshList[2]->RenderMesh();

		// 渲染spaceship
		model = glm::mat4(1.0f); // 重置模型矩阵
		model = glm::translate(model, glm::vec3(4.0f, 0.0f, 0.0f)); // 平移
		model = glm::scale(model, glm::vec3(0.4, 0.4, 0.4)); // 缩放
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); // 设置 uniform 变量 model
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess); // 使用低光材质
		spaceship.RenderModel();

		// 渲染nanosuit
		model = glm::mat4(1.0f); // 重置模型矩阵
		model = glm::translate(model, glm::vec3(-4.0f, 0.0f, 0.0f)); // 平移
		model = glm::scale(model, glm::vec3(0.2, 0.2, 0.2)); // 缩放
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); // 设置 uniform 变量 model
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess); // 使用低光材质
		nanosuit.RenderModel();

		// 渲染xwing
		model = glm::mat4(1.0f); // 重置模型矩阵
		model = glm::translate(model, glm::vec3(-4.0f, 0.0f, 2.0f)); // 平移
		model = glm::scale(model, glm::vec3(0.005, 0.005, 0.005)); // 缩放
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); // 设置 uniform 变量 model
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess); // 使用低光材质
		xwing.RenderModel();

		// 渲染uh60
		model = glm::mat4(1.0f); // 重置模型矩阵
		model = glm::translate(model, glm::vec3(-4.0f, 0.0f, -4.0f)); // 平移
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); // 旋转
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); // 旋转
		model = glm::scale(model, glm::vec3(0.4, 0.4, 0.4)); // 缩放
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); // 设置 uniform 变量 model
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess); // 使用低光材质
		uh60.RenderModel();

		glUseProgram(0); // 不使用着色器

		mainWindow.SwapBuffers();
	}

	return 0;
}