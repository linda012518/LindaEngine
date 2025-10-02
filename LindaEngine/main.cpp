#pragma comment(lib, "opengl32.lib")

#include <iostream>

#include "Application.h"

#include "Entity.h"
#include "Transform.h"
#include "TransformSystem.h"
#include "Scene.h"

using namespace LindaEngine;

int main()
{
	Scene scene;
	Entity* e1 = scene.CreateEntity("test1");
	Transform* temp1 = e1->GetComponent<Transform>();

	Entity* e2 = scene.CreateEntity("test2", false);
	Transform* temp2 = e2->GetComponent<Transform>();

	Entity* e3 = scene.CreateEntity("test3", false);
	Transform* temp3 = e3->GetComponent<Transform>();
	temp3->SetEnable(false);

	Entity* e4 = scene.CreateEntity("test4");
	Transform* temp4 = e4->GetComponent<Transform>();
	temp4->SetEnable(false);

	temp3->SetParent(temp1);
	temp4->SetParent(temp3);
	temp2->SetParent(temp3);
	
	scene.DestroyEntity(e3);


	TransformSystem::OnAwake();
	TransformSystem::OnEnable();
	TransformSystem::OnStart();

	int count = 0;

	while (true)
	{
		count++;
		TransformSystem::Tick();
		//if (count == 1)
		//	entity1.SetActive(true);
		//else if (count == 2)
		//	temp3->SetEnable(true);
		//else if (count == 3)
		//	entity2.SetActive(true);
		//else if (count == 4)
		//	temp2->SetEnable(true);
		//else if (count == 5)
		//	entity.SetActive(false);
		//else if (count == 6)
		//	entity.SetActive(true);
		//else if (count == 7)
		//	temp->SetEnable(false);
		//else if (count == 8)
		//	temp->SetEnable(true);
	}


	//Application app;
	//app.Initialize();
	//app.Tick();
	//app.Finalize();

	return 0;
}