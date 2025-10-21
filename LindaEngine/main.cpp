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
#include "Texture.h"
#include "Path.h"
#include "SceneManagerEditor.h"
#include "NodeEditor.h"
#include <typeinfo>
#include "SceneManager.h"

using namespace LindaEngine;
using namespace LindaEditor;

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

	//YamlSerializer::DeSerializeMaterial("Assets/Materials/test.mat");


	//Ref<Texture2D> tex2d = CreateRef<Texture2D>();
	//tex2d->path = "Assets/Maps/wall.jpg";
	//tex2d->isDataSRGB = true;
	//tex2d->mipmapCount = 6;
	//tex2d->filter = FilterMode::Point;
	//tex2d->warpU = TextureWrapMode::Repeat;
	//tex2d->warpV = TextureWrapMode::Repeat;
	//tex2d->warpW = TextureWrapMode::Repeat;
	//Texture::overrideTexture = tex2d;
	//YamlSerializer::DeSerializeTexture("Assets/Maps/Yamls/go.texture");

	//Ref<Cubemap> cubemap = CreateRef<Cubemap>();
	//cubemap->isDataSRGB = false;
	//cubemap->mipmapCount = 6;
	//cubemap->filter = FilterMode::Point;
	//cubemap->warpU = TextureWrapMode::Repeat;
	//cubemap->warpV = TextureWrapMode::Repeat;
	//cubemap->warpW = TextureWrapMode::Repeat;
	//cubemap->left = "left";
	//cubemap->right = "right";
	//cubemap->top = "top";
	//cubemap->bottom = "bottom";
	//cubemap->front = "front";
	//cubemap->back = "back";
	//cubemap->srcType = CubemapSrcType::SixTexture;
	//Texture::overrideTexture = cubemap;
	//YamlSerializer::DeSerializeTexture("Assets/Maps/Yamls/testCube.texture");

	//Ref<Scene> scene = CreateRef<Scene>();
	//Path::overridePath = "Assets/Scenes/NewScene.scene";

	//Entity* entity = scene->CreateEntity("test1");
	////entity->GetTransform()->SetLocalPosition(glm::vec3(12, 13, 14.6f));
	////entity->GetTransform()->SetLocalScale(glm::vec3(2, 6, 8));
	//entity->GetTransform()->Tick();

	//Entity* entity2 = scene->CreateEntity("test2");
	//entity2->GetTransform()->SetParent(entity->GetTransform());
	////entity2->GetTransform()->SetLocalPosition(glm::vec3(1, 3, 66.8f));
	////entity2->GetTransform()->SetLocalScale(glm::vec3(1, 2, 1));
	//entity2->GetTransform()->SetLocalEulerAngles(glm::vec3(10, 10, 10));
	//entity2->GetTransform()->Tick();
	//entity2->SetActive(false);

	//PerspectiveCamera* camera = entity2->AddComponent<PerspectiveCamera>();

	//scene->Serialize();



	//Ref<SceneNodeEditor> sne0 = CreateRef<SceneNodeEditor>();
	//sne0->name = "test0.scene"; 
	//sne0->path = "go/go/test0.scene";

	//Ref<SceneNodeEditor> sne1 = CreateRef<SceneNodeEditor>();
	//sne1->name = "test1.scene";
	//sne1->path = "go/go/test1.scene";

	//SceneManagerEditor::AddToBuild(0, sne1);
	//SceneManagerEditor::AddToBuild(1, sne0);
	//SceneManagerEditor::Build("Assets/Config/SceneConfig");

	////std::string go1 = "go/go1/test0.scene";
	////std::string go2 = "go/g2o2/test0.scene";
	////go1 = Path::GetFilePath(go1);
	////go2 = Path::GetFileName(go2);

	SceneManager::Initialize();
	Ref<Scene> scene = SceneManager::LoadScene(0);

	//TransformSystem::OnDeserializeFinish();
	//TransformSystem::Tick();

	//std::string go = typeid(Entity).name();
	int a = 0;
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