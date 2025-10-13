#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "assimp-vc143-mtd.lib")

#include <iostream>

#include "Application.h"

#include "Entity.h"
#include "Transform.h"
#include "TransformSystem.h"
#include "Camera.h"
#include "CameraSystem.h"
#include "Scene.h"
#include "ShaderManager.h"
#include "YamlSerializer.h"

using namespace LindaEngine;

int main()
{
	//YamlSerializer::SerializeGraphicsConfig("Assets/Config/GraphicsConfig");
	//YamlSerializer::SerializeMaterial("Assets/Materials/Unlit.mat");

	//std::string path = "Assets/Shaders/Unlit.shader";
	//ShaderManager::GetShaderSource(path.c_str());


	//Scene scene;
	//Entity* e1 = scene.CreateEntity("test1");
	////e1->AddComponent< OrthoCamera>();
	//Transform* temp1 = e1->GetComponent<Transform>();

	//Entity* e2 = scene.CreateEntity("test2", false);
	//Transform* temp2 = e2->GetComponent<Transform>();

	//Entity* e3 = scene.CreateEntity("test3", false);
	//Transform* temp3 = e3->GetComponent<Transform>();
	//temp3->SetEnable(false);

	//Entity* e4 = scene.CreateEntity("test4");
	//Transform* temp4 = e4->GetComponent<Transform>();
	//temp4->SetEnable(false);

	//temp3->SetParent(temp1);
	//temp4->SetParent(temp3);
	//temp2->SetParent(temp3);
	//
	//scene.DestroyEntity(e3);


	////TransformSystem::OnAwake();
	////TransformSystem::OnEnable();
	////TransformSystem::OnStart();

	//int count = 0;

	//while (true)
	//{
	//	//LifeCycleFuncSystem::DoComponentAwake();
	//	//LifeCycleFuncSystem::DoComponentStart();

	//	count++;
	//	TransformSystem::Tick();
	//	if (count == 1)
	//		e2->SetActive(true);
	//	else if (count == 2)
	//		temp3->SetEnable(true);
	//	else if (count == 3)
	//		e3->SetActive(true);
	//	else if (count == 4)
	//		temp4->SetEnable(true);
	//	else if (count == 5)
	//		e1->SetActive(false);
	//	else if (count == 6)
	//		e1->SetActive(true);
	//	else if (count == 7)
	//		temp1->SetEnable(false);
	//	else if (count == 8)
	//		temp1->SetEnable(true);
	//}


	Application app;
	app.Initialize();
	//app.Tick();
	//app.Finalize();

	return 0;
}