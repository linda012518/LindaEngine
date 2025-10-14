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
#include "Material.h"
#include "MaterialPass.h"

using namespace LindaEngine;

int main()
{
	//YamlSerializer::SerializeGraphicsConfig("Assets/Config/GraphicsConfig");
	//YamlSerializer::SerializeMaterial("Assets/Materials/Unlit.mat");

	//std::string path = "Assets/Shaders/Unlit.shader";
	//ShaderManager::GetShaderSource(path.c_str());

	//Ref<Material> mat = CreateRef<Material>();
	//mat->_renderType = RenderType::Transparent;

	//mat->_depthPass = CreateRef<MaterialPass>();
	//mat->_depthPass->_lightMode = "Depth";
	//mat->_depthNormalPass = CreateRef<MaterialPass>();
	//mat->_depthNormalPass->_lightMode = "DepthNormal";
	//mat->_shadowCasterPass = CreateRef<MaterialPass>();
	//mat->_shadowCasterPass->_lightMode = "ShadowCaster";
	//mat->_colorPasses.push_back(CreateRef<MaterialPass>());
	//mat->_colorPasses.push_back(CreateRef<MaterialPass>());

	//mat->_colorPasses[1]->_renderQueue = 3000;
	//mat->_colorPasses[1]->_lightMode = "customLightMode";
	//mat->_colorPasses[1]->_keywords.push_back("NORMAL_MAP");
	//mat->_colorPasses[1]->_keywords.push_back("LogDepth");
	//mat->_colorPasses[1]->_renderState.colorMask.A = false;
	//mat->_colorPasses[1]->_renderState.blendState.enableBlend = true;
	//mat->_colorPasses[1]->_renderState.blendState.blendSrc = BlendFunc::ONE_MINUS_DST_ALPHA;
	//mat->_colorPasses[1]->_renderState.cullFaceFunc = CullFaceFunc::None;
	//mat->_colorPasses[1]->_renderState.depthState.depthTest = false;
	//mat->_colorPasses[1]->_renderState.polygonModeState.polygonMode = PolygonMode::LINE;
	//mat->_colorPasses[1]->_renderState.stencilState.stencilTest = true;
	//mat->_colorPasses[1]->_renderState.stencilState.stencilRef = 6;
	//Ref<TextureUniformData> d1 = CreateRef<TextureUniformData>();
	//d1->textureURL = "texturl/aa/bb";
	//d1->offsetScale.y = 2.6f;
	//d1->name = "MainTex";
	//mat->_colorPasses[1]->_uniformNameMap[d1->name] = d1;
	//Ref<FloatUniformData> d2 = CreateRef<FloatUniformData>();
	//d2->value = 3.6f;
	//d2->name = "Range";
	//mat->_colorPasses[1]->_uniformNameMap[d2->name] = d2;
	//Ref<Int4UniformData> d3 = CreateRef<Int4UniformData>();
	//d3->value.z = 8;
	//d3->name = "Vector";
	//mat->_colorPasses[1]->_uniformNameMap[d3->name] = d3;

	//Material::overrideMat = mat;
	//YamlSerializer::SerializeMaterial("Assets/Materials/test.mat");

	YamlSerializer::DeSerializeMaterial("Assets/Materials/test.mat");
	return 0;
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
	app.Tick();
	app.Finalize();

	return 0;
}