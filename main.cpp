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

GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
       uniformSpecularIntensity = 0, uniformShininess = 0;

Window mainWindow; // 主窗口
GLsizei windowWidth = 1920, windowHeight = 1080; // 窗口宽高

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

unsigned int pointLightCount = 0;
unsigned int spotLightCount = 0;

GLfloat deltaTime = 0.0f; // 帧间隔时间
GLfloat lastTime = 0.0f;

GLfloat uh60Angle = 0.0f; // 黑鹰旋转角度

std::vector<Mesh*> meshList; // 网格列表

std::vector<Shader*> shaderList; // 着色器列表

Shader directionalShadowShader; // 方向光源阴影着色器

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
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1],
		             vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1],
		             vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = cross(v1, v2);
		normal = normalize(normal);

		in0 += normalOffset;
		in1 += normalOffset;
		in2 += normalOffset;
		vertices[in0] += normal.x;
		vertices[in0 + 1] += normal.y;
		vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x;
		vertices[in1 + 1] += normal.y;
		vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x;
		vertices[in2 + 1] += normal.y;
		vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = normalize(vec);
		vertices[nOffset] = vec.x;
		vertices[nOffset + 1] = vec.y;
		vertices[nOffset + 2] = vec.z;
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
		-1.0f, -1.0f, -0.6f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 1.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -0.6f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		0, 3, 2
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f, 10.0f, 0.0f, 0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f, 10.0f, 10.0f, 0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f, 0.0f, 10.0f, 0.0f, -1.0f, 0.0f
	};

	CalcAverageNormals(indices, 12, vertices, 32, 8, 5);

	auto obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	auto obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	auto obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);
}

void CreateShaders()
{
	auto shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(shader1);

	directionalShadowShader = Shader();
	directionalShadowShader.CreateFromFiles("Shaders/directional_shadow_map.vert",
	                                        "Shaders/directional_shadow_map.frag");
}

void RenderScene()
{
	glm::mat4 model(1.0f); // 创建模型矩阵

	//model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 1.0f)); // 旋转
	model = translate(model, glm::vec3(0.0f, 0.0f, -2.0f)); // 平移
	//model = glm::scale(model, glm::vec3(0.4, 0.4, 1.0f)); // 缩放
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, value_ptr(model)); // 设置 uniform 变量 model
	brickTexture.UseTexture();
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess); // 使用高光材质
	meshList[0]->RenderMesh();

	model = glm::mat4(1.0f); // 重置模型矩阵
	model = translate(model, glm::vec3(0.0f, 4.0f, -2.0f)); // 平移
	//model = glm::scale(model, glm::vec3(0.4, 0.4, 1.0f)); // 缩放
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, value_ptr(model)); // 设置 uniform 变量 model
	dirtTexture.UseTexture();
	dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess); // 使用低光材质
	meshList[1]->RenderMesh();

	model = glm::mat4(1.0f); // 重置模型矩阵
	model = translate(model, glm::vec3(0.0f, -2.0f, 0.0f)); // 平移
	//model = glm::scale(model, glm::vec3(0.4, 0.4, 1.0f)); // 缩放
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, value_ptr(model)); // 设置 uniform 变量 model
	plainTexture.UseTexture();
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess); // 使用低光材质
	meshList[2]->RenderMesh();

	// 渲染spaceship
	model = glm::mat4(1.0f); // 重置模型矩阵
	model = translate(model, glm::vec3(4.0f, 0.0f, 0.0f)); // 平移
	model = scale(model, glm::vec3(0.4, 0.4, 0.4)); // 缩放
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, value_ptr(model)); // 设置 uniform 变量 model
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess); // 使用低光材质
	spaceship.RenderModel();

	// 渲染nanosuit
	model = glm::mat4(1.0f); // 重置模型矩阵
	model = translate(model, glm::vec3(-4.0f, 0.0f, 0.0f)); // 平移
	model = scale(model, glm::vec3(0.2, 0.2, 0.2)); // 缩放
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, value_ptr(model)); // 设置 uniform 变量 model
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess); // 使用低光材质
	nanosuit.RenderModel();

	// 渲染xwing
	model = glm::mat4(1.0f); // 重置模型矩阵
	model = translate(model, glm::vec3(-4.0f, 0.0f, 2.0f)); // 平移
	model = scale(model, glm::vec3(0.005, 0.005, 0.005)); // 缩放
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, value_ptr(model)); // 设置 uniform 变量 model
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess); // 使用低光材质
	xwing.RenderModel();

	// 渲染uh60
	uh60Angle += 0.1f;
	if (uh60Angle > 360.0f)
	{
		uh60Angle = 0.1f;
	}

	model = glm::mat4(1.0f);
	model = rotate(model, -uh60Angle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	model = translate(model, glm::vec3(-8.0f, 2.0f, 0.0f));
	model = rotate(model, -20.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = rotate(model, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, value_ptr(model));
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	uh60.RenderModel();
}

void DirectionalShadowMapPass(DirectionalLight* light)
{
	directionalShadowShader.UseShader();

	glViewport(0, 0, light->getShadowMap()->GetShadowWidth(), light->getShadowMap()->GetShadowHeight());

	light->getShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	uniformModel = directionalShadowShader.GetModelLocation();
	glm::mat4 tmp = light->CalculateLightTransform();
	directionalShadowShader.SetDirectionalLightTransform(&tmp);

	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderPass(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
	shaderList[0]->UseShader();

	uniformModel = shaderList[0]->GetModelLocation();
	uniformProjection = shaderList[0]->GetProjectionLocation();
	uniformView = shaderList[0]->GetViewLocation();
	uniformSpecularIntensity = shaderList[0]->GetSpecularIntensityLocation();
	uniformShininess = shaderList[0]->GetShininessLocation();
	uniformEyePosition = shaderList[0]->GetEyePositionLocation();

	glViewport(0, 0, windowWidth, windowHeight); // 设置视口大小

	// 清除窗口
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清除颜色缓冲和深度缓冲

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, value_ptr(projectionMatrix)); // 设置 uniform 变量 projection
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, value_ptr(viewMatrix)); // 设置 uniform 变量 view
	glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y,
	            camera.getCameraPosition().z); // 设置 uniform 变量 eyePosition

	shaderList[0]->SetDirectionalLight(&mainLight);
	shaderList[0]->SetPointLights(pointLights, pointLightCount);
	shaderList[0]->SetSpotLights(spotLights, spotLightCount);
	glm::mat4 tmp = mainLight.CalculateLightTransform();
	shaderList[0]->SetDirectionalLightTransform(&tmp); // 设置平行光矩阵

	mainLight.getShadowMap()->Read(GL_TEXTURE1);
	shaderList[0]->SetTexture(0); // 设置纹理
	shaderList[0]->SetDirectionalShadowMap(1); // 设置平行光纹理

	// 设置手电筒
	glm::vec3 lowerLight = camera.getCameraPosition();
	lowerLight.y -= 0.3f;
	//spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

	RenderScene();
}

int main()
{
	mainWindow = Window(windowWidth, windowHeight);
	mainWindow.Initialise();

	CreateObjects(); // 创建三角形
	CreateShaders(); // 创建着色器

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -103.0f, 0.0f, 5.0f, 1.0f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA(); // 加载纹理
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();

	shinyMaterial = Material(4.0f, 256);
	dullMaterial = Material(0.3f, 4);

	spaceship = Model();
	spaceship.LoadModel("Models/Intergalactic_Spaceship-(Wavefront).obj");

	nanosuit = Model();
	nanosuit.LoadModel("Models/nanosuit.obj");

	xwing = Model();
	xwing.LoadModel("Models/x-wing.obj");

	uh60 = Model();
	uh60.LoadModel("Models/uh60.obj");

	mainLight = DirectionalLight(2048, 2048,
	                             1.0f, 1.0f, 1.0f,
	                             0.1f, 0.3f,
	                             0.0f, -15.0f, -10.0f);

	// 创建点光源
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

	glm::mat4 projection = glm::perspective(
		45.0f, static_cast<GLfloat>(mainWindow.getBufferWidth()) / static_cast<GLfloat>(mainWindow.getBufferHeight()),
		0.1f, 100.0f); // 创建投影矩阵

	// 循环，直到窗口关闭
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime(); // SDL_GetPerformanceCounter();
		deltaTime = now - lastTime; // (now - lastTime) * 1000 / SDL_GetPerformanceFrequency();
		lastTime = now;

		// 获取并处理事件
		glfwPollEvents();

		camera.keyControl(mainWindow.getsKeys(), deltaTime); // 键盘控制
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange()); // 鼠标控制

		DirectionalShadowMapPass(&mainLight); // 平行光阴影映射
		RenderPass(camera.calculateViewMatrix(), projection); // 渲染

		//glUseProgram(0); // 不使用着色器

		mainWindow.swapBuffers();
	}

	return 0;
}
