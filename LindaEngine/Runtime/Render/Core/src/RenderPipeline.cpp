#include "RenderPipeline.h"
#include "Application.h"



//#include "glad/glad.h"
//#include "glm/glm.hpp"
//#include <glm\gtc\matrix_transform.hpp>
//#include <glm\gtc\type_ptr.hpp>
//#include "Core/Shader.h"
//#include "stb_image.h"
//#include "Loader/TextLoader.h"
//#include "Core/Entity.hpp"
//#include "Component/Transform.h"
//#include "Component/Camera.h"
//#include "Core/Scene.h"
//#include "Core/Drawable.h"
//#include "Component/Renderer.h"
//#include "Core/Material.h"

using namespace LindaEngine;

int RenderPipeline::Initialize()
{
	//_scene = CreateRef<Scene>();
    return 0;
}

void RenderPipeline::Finalize()
{

}

void RenderPipeline::Tick()
{
	//ComponentSystem::Instance().Tick();

	//std::map<int, std::vector<Ref<Drawable>>, std::less<int>> opaque;
	//std::map<int, std::vector<Ref<Drawable>>, std::greater<int>> transparent;

	//std::vector<Ref<Entity>>& entitys = _scene->GetEntitys();
	//for (auto& entity : entitys) {
	//	Ref<Transform> t = entity->GetComponent<Transform>();
	//	Ref<MeshRenderer> r = entity->GetComponent<MeshRenderer>();
	//	if (r->materialList.size() <= 0)
	//		continue; //可以给error材质

	//	for (auto& mat : r->materialList) {
	//		RenderType& type = mat->GetRenderType();
	//		switch (type)
	//		{
	//		case RenderType::Opaque:
	//			{
	//				for (auto& matPass : mat->GetColorPass())
	//				{
	//					Ref<Drawable> d = CreateRef<Drawable>();
	//					d->mesh = r->mesh.get();
	//					d->pass = matPass.get();
	//					d->visible = true;
	//					d->modelMat = t->worldMatrix;
	//					if (opaque.find(mat->GetRenderQueue()) != opaque.end())
	//					{
	//						std::vector<Ref<Drawable>>& list = opaque[mat->GetRenderQueue()];
	//						list.push_back(d);
	//					}
	//					else
	//					{
	//						opaque[mat->GetRenderQueue()] = {d};
	//					}
	//				}
	//			}
	//			break;
	//		case RenderType::Transparent:
	//			{
	//				for (auto& matPass : mat->GetColorPass())
	//				{
	//					Ref<Drawable> d = CreateRef<Drawable>();
	//					d->mesh = r->mesh.get();
	//					d->pass = matPass.get();
	//					d->visible = true;
	//					d->modelMat = t->worldMatrix;
	//					if (transparent.find(mat->GetRenderQueue()) != transparent.end())
	//					{
	//						std::vector<Ref<Drawable>>& list = transparent[mat->GetRenderQueue()];
	//						list.push_back(d);
	//					}
	//					else
	//					{
	//						transparent[mat->GetRenderQueue()] = { d };
	//					}
	//				}
	//			}
	//			break;
	//		}

	//	}
	//}

 //   CheckLightList();
 //   CheckCameraList();
 //   Render();
}

void RenderPipeline::SetRenderAPIContext(Ref<RenderAPIContext> rc)
{
    _renderContext = rc;
}

Scope<RenderPipeline> RenderPipeline::Create()
{
    return CreateScope<RenderPipeline>();
}

void RenderPipeline::CheckCameraList()
{
    //1 查找可用阴影相机
    //2 查找可用颜色相机
    //3 相机排序
}

void RenderPipeline::CheckLightList()
{
    //1 查找可用灯光
    //2 设置灯光参数到shader
    //3 设置阴影相机
}

void RenderPipeline::Render()
{
    //1 遍历相机
    //2 设置相机参数到shader
    //3 配置相机 HDR 后处理等等
    //4 视锥剔除，排序
    //5 设置清屏相关
    //6 渲染不透明物体
    //7 渲染天空
    //8 渲染透明物体

    //GfxConfiguration& config = Application::GetConfig();
    //_renderContext->SetViewport(0, 0, config.screenWidth, config.screenHeight);
    //_renderContext->SetClearColor(0, 0.3, 0, 0);
    //_renderContext->Clear(true, true, true);

	//test();
}

//void RenderPipeline::test()
//{
//
//	Shader ourShader(TextLoader::Load("Assets/Shaders/texture.vs").c_str(), TextLoader::Load("Assets/Shaders/texture.fs").c_str());
//
//	// 设置vbo vao
//	// ------------------------------------------------------------------
//	float vertices[] = {
//		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
//		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
//		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
//		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
//
//		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
//		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
//		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
//		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//
//		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//
//		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//
//		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
//		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//
//		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
//		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
//		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
//	};
//	// 
//	glm::vec3 cubePositions[] = {
//		glm::vec3(0.0f,  0.0f,  0.0f),
//		glm::vec3(2.0f,  5.0f, -15.0f),
//		glm::vec3(-1.5f, -2.2f, -2.5f),
//		glm::vec3(-3.8f, -2.0f, -12.3f),
//		glm::vec3(2.4f, -0.4f, -3.5f),
//		glm::vec3(-1.7f,  3.0f, -7.5f),
//		glm::vec3(1.3f, -2.0f, -2.5f),
//		glm::vec3(1.5f,  2.0f, -2.5f),
//		glm::vec3(1.5f,  0.2f, -1.5f),
//		glm::vec3(-1.3f,  1.0f, -1.5f)
//	};
//
//	//顶点数组对象：Vertex Array Object，VAO
//	//顶点缓冲对象：Vertex Buffer Object，VBO
//	//索引缓冲对象：Element Buffer Object，EBO或Index Buffer Object，IBO
//	//顶点着色器			转换3D坐标	标准化设备坐标	
//	//图元装配			所有的点装配成指定图元的形状
//	//几何着色器			重新装配图元
//	//光栅化阶段			把图元映射为最终屏幕上相应的像素
//	//裁切				丢弃超出你的视图以外的所有像素
//	//片段着色器			计算一个像素的最终颜色
//	//Alpha测试和混合		检测片段的对应的深度（和模板(Stencil)）检查alpha值
//	unsigned int VBO, VAO;
//	glGenVertexArrays(1, &VAO);
//	glGenBuffers(1, &VBO);
//	//当配置顶点属性指针时，你只需要将那些调用执行一次，之后再绘制物体的时候只需要绑定相应的VAO就行了
//	glBindVertexArray(VAO);
//
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//	//GL_STATIC_DRAW ：数据不会或几乎不会改变。	三角形的位置数据不会改变，每次渲染调用时都保持原样
//	//GL_DYNAMIC_DRAW：数据会被改变很多。
//	//GL_STREAM_DRAW ：数据每次绘制时都会改变。
//	//数据将频繁被改变，那么使用的类型就是GL_DYNAMIC_DRAW或GL_STREAM_DRAW，这样就能确保显卡把数据放在能够高速写入的内存部分
//
//	//第一个0		配置的顶点属性layout(location = 0)位置值
//	//最后个0		位置数据在缓冲中起始位置的偏移量(Offset)。由于位置数据在数组的开头，所以这里是0
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(0);
//	// uv属性
//	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//	glEnableVertexAttribArray(2);
//
//
//	/*
//	GL_REPEAT 			对纹理的默认行为。重复纹理图像。
//	GL_MIRRORED_REPEAT 	和GL_REPEAT一样，但每次重复图片是镜像放置的。
//	GL_CLAMP_TO_EDGE 	纹理坐标会被约束在0到1之间，超出的部分会重复纹理坐标的边缘，产生一种边缘被拉伸的效果。
//	GL_CLAMP_TO_BORDER 	超出的坐标为用户指定的边缘颜色。
//
//
//	多级渐远纹理主要是使用在纹理被缩小的情况下
//	过滤方式 					描述		多级渐远纹理
//	GL_NEAREST_MIPMAP_NEAREST 	使用最邻近的多级渐远纹理来匹配像素大小，并使用邻近插值进行纹理采样
//	GL_LINEAR_MIPMAP_NEAREST 	使用最邻近的多级渐远纹理级别，并使用线性插值进行采样
//	GL_NEAREST_MIPMAP_LINEAR 	在两个最匹配像素大小的多级渐远纹理之间进行线性插值，使用邻近插值进行采样
//	GL_LINEAR_MIPMAP_LINEAR 	在两个邻近的多级渐远纹理之间使用线性插值，并使用线性插值进行采样
//
//	GL_NEAREST	产生了颗粒状的图案
//	GL_LINEAR	能够产生更平滑的图案
//	*/
//	unsigned int texture, texture2;
//	glGenTextures(1, &texture);
//	glBindTexture(GL_TEXTURE_2D, texture);
//	// 设置包装uv两个方向的参数
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	// 设置滤波器
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//缩小(Minify)
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//放大(Magnify)
//
//	int width, height, nrChannels;
//	unsigned char* data = stbi_load("Assets/Map/wall.jpg", &width, &height, &nrChannels, 0);
//	if (data)
//	{
//		//当前绑定的纹理对象就会被附加上纹理图像
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//		//当前绑定的纹理自动生成所有需要的多级渐远纹理
//		glGenerateMipmap(GL_TEXTURE_2D);
//	}
//	else
//	{
//		std::cout << "Failed to load texture" << std::endl;
//	}
//	stbi_image_free(data);
//
//	glGenTextures(1, &texture2);
//	glBindTexture(GL_TEXTURE_2D, texture2);
//	// 设置包装uv两个方向的参数
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	// 设置滤波器
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//缩小(Minify)
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//放大(Magnify)
//
//	stbi_set_flip_vertically_on_load(true);//翻转y轴
//	data = stbi_load("Assets/Map/awesomeface.png", &width, &height, &nrChannels, 0);
//	if (data)
//	{
//		//当前绑定的纹理对象就会被附加上纹理图像
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
//		//当前绑定的纹理自动生成所有需要的多级渐远纹理
//		glGenerateMipmap(GL_TEXTURE_2D);
//	}
//	else
//	{
//		std::cout << "Failed to load texture" << std::endl;
//	}
//	stbi_image_free(data);
//
//
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, texture);
//	glActiveTexture(GL_TEXTURE1);
//	glBindTexture(GL_TEXTURE_2D, texture2);
//
//
//
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//	glBindVertexArray(0);
//	//线框模式
//	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//
//	glEnable(GL_DEPTH_TEST);
//
//
//	ourShader.Begin();
//
//	ourShader.SetInt("texture1", 0);
//	ourShader.SetInt("texture2", 1);
//	ourShader.SetFloat("mixValue", 0.2);
//
//	GfxConfiguration& config = Application::GetConfig();
//	Entity cam("camear");
//	Ref<Transform> cam_trans = cam.GetComponent<Transform>();
//	ComponentSystem::Instance().GetTransformSystem().SetWorldPosition(cam_trans, glm::vec3(0.0f, 0.0f, 3.0f));
//
//	Ref< PerspectiveCamera> camera = cam.AddComponent< PerspectiveCamera>();
//	ComponentSystem::Instance().GetCameraSystem().SetPerspectivePara(camera, 60.0f, (float)config.screenNewWidth / (float)config.screenNewHeight, 0.1f, 100.0f);
//	//三大矩阵
//	glm::mat4 view = glm::mat4(1.0f);
//	glm::mat4 projection = glm::mat4(1.0f);
//	projection = glm::perspective(glm::radians(60.0f), (float)config.screenNewWidth / (float)config.screenNewHeight, 0.1f, 100.0f);
//	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
//
//
//	Entity e("test");
//	Ref<Transform> trans = e.GetComponent<Transform>();
//	// 
//	glBindVertexArray(VAO);
//	for (unsigned int i = 0; i < 10; i++)
//	{
//
//		glm::mat4 model = glm::mat4(1.0f);
//		////if (i % 3 == 0 || i == 0)
//		////	model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0, 0, 1));
//		//model = glm::translate(model, cubePositions[i]);
//		float angle = 20.0f * i;
//		//model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
//
//		ComponentSystem::Instance().GetTransformSystem().SetWorldPosition(trans, cubePositions[i]);
//		ComponentSystem::Instance().GetTransformSystem().SetWorldEulerAngles(trans, glm::vec3(-angle, 0, 0));
//
//		ComponentSystem::Instance().Tick();
//
//		view = camera->viewMatrix;
//		projection = camera->projectMatrix;
//
//		ourShader.SetMat4("projection", projection); // 
//		ourShader.SetMat4("view", view);
//
//		model = trans->worldMatrix;
//
//		ourShader.SetMat4("model", model);
//
//		auto go = projection * view * model;
//
//		glDrawArrays(GL_TRIANGLES, 0, 36);
//	}
//
//}